# install nginx
http://nginx.org/en/docs/

# install fastcgi
wget http://archive.ubuntu.com/ubuntu/pool/universe/libf/libfcgi/libfcgi_2.4.0.orig.tar.gz
wget https://github.com/FastCGI-Archives/fcgi2/archive/refs/tags/2.4.0.tar.gz

# install spawn-cgi
wget http://download.lighttpd.net/spawn-fcgi/releases-1.6.x/spawn-fcgi-1.6.4.tar.gz
wget http://www.lighttpd.net/download/spawn-fcgi-1.6.3.tar.bz2

# main fastcgi processer
g++ -o main.cgi main.cpp -lfcgi

# modify nginx configure
sudo vim /etc/nginx/nginx.conf
sudo nginx -tc /etc/nginx/nginx.conf
sudo nginx -s reload

# run spawn-fcgi
spawn-fcgi -a 127.0.0.1 -p 8082 -u nobody -f ./main.cgi

# about firewall
firewall-cmd --zone=public --add-port=8081/tcp --permanent
firewall-cmd --reload
firewall-cmd --list-ports
systemctl restart firewalld.service

