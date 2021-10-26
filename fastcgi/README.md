# install nginx
http://nginx.org/en/docs/

# install fastcgi
使用命令安装
```
sudo apt-get install libfcgi-dev
```
使用源码安装
```
wget http://archive.ubuntu.com/ubuntu/pool/universe/libf/libfcgi/libfcgi_2.4.0.orig.tar.gz
```
或者参考[Github镜像](https://github.com/FastCGI-Archives/fcgi2/archive/refs/tags/2.4.0.tar.gz)

# install spawn-cgi
```
wget http://download.lighttpd.net/spawn-fcgi/releases-1.6.x/spawn-fcgi-1.6.4.tar.gz
```
~~wget~~ ~~http://www.lighttpd.net/download/spawn-fcgi-1.6.3.tar.bz2~~

# main fastcgi processer
```
g++ -o main.cgi main.cpp -lfcgi
```
参考[main.cpp](./main.cpp)

# modify nginx configure
```
sudo mkdir /usr/share/nginx/cgi
sudo cp main.cgi /usr/share/nginx/cgi

sudo vim /etc/nginx/nginx.conf
sudo nginx -tc /etc/nginx/nginx.conf
sudo nginx -s reload
```
具体的配置参考 [nginx.conf](./nginx.conf)<br/>
nginx代理将cgi请求转发给8082端口

# run spawn-fcgi
```
cd /usr/share/nginx/cgi
spawn-fcgi -a 127.0.0.1 -p 8082 -u nobody -f ./main.cgi
```
此处可能报错`spawn-fcgi: child exited with: 127`
查看依赖
```
[ec2-user@ip-172-31-44-213 cgi]$ ldd ./main.cgi
linux-vdso.so.1 (0x00007ffcd9ed3000)
libfcgi.so.0 => not found
libstdc++.so.6 => /lib64/libstdc++.so.6 (0x00007f2386ac6000)
libm.so.6 => /lib64/libm.so.6 (0x00007f2386786000)
libgcc_s.so.1 => /lib64/libgcc_s.so.1 (0x00007f2386570000)
libc.so.6 => /lib64/libc.so.6 (0x00007f23861c5000)
/lib64/ld-linux-x86-64.so.2 (0x00007f2386e48000)
```
有三种解决方案<br/>
第一种是在链接的时候指定libfcgi.so.0的路径<br/>
```
g++ -o main.cgi main.cpp -lfcgi -Wl,--rpath=/usr/local/lib
```
第二种需要在/usr/lib目录下建立软链接
```
ln -s /usr/local/lib/libfcgi.so.0  /usr/lib/libfcgi.so.0
ln -s /usr/local/lib/libfcgi.so.0  /usr/lib64/libfcgi.so.0
```
第三种方案是在`/etc/ld.so.conf`文件加入`/usr/local/lib`这个目录<br/>
然后执行刷新`sudo ldconfig`

# about firewall on centos7
```
firewall-cmd --zone=public --add-port=8081/tcp --permanent
firewall-cmd --reload
firewall-cmd --list-ports
systemctl restart firewalld.service
```

# 参考资料
1. [Source Package: libfcgi (2.4.0-10build1)](https://packages.ubuntu.com/source/focal/s390x/libfcgi)
2. [WikiStart - spawn-fcgi - lighty labs](https://redmine.lighttpd.net/projects/spawn-fcgi/wiki)
3. [Nginx&C++实现简单的fastcgi程序](https://blog.csdn.net/liukang325/article/details/78782994)
4. [Nginx FastCGI 开发（C/C++）](https://my.oschina.net/u/989096/blog/887078https://my.oschina.net/u/989096/blog/887078)
