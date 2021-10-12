#include <stdio.h>
#include <openssl/md5.h>
#include <iostream>
#include <sstream>
#include <iomanip>

#define READ_DATA_SIZE  1024
#define MD5_SIZE        16

int compute_file_md5(const char* file, std::string& md5);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("format: compute_file_md5 filepath.\n");
        return -1;
    }
    std::string md5;
    if (compute_file_md5(argv[1], md5) < 0) {
        printf("compute file md5 failed");
        return -1;
    }
    printf("md5: %s\n", md5.c_str());
    return 0;
}

int compute_file_md5(const char* file, std::string& md5) {
    FILE* fd = fopen(file, "r");
    if (fd == NULL) {
        printf("open file failed.\n");
        return -1;
    }
    MD5_CTX ctx;
    unsigned char md5_value[MD5_SIZE] = {0};
    unsigned char data[READ_DATA_SIZE];
    int ret;
    MD5_Init(&ctx);
    while(0 != (ret = fread(data, 1, READ_DATA_SIZE, fd))) {
        MD5_Update(&ctx, data, ret);
    }
    fclose(fd);
    if (ret < 0) {
        printf("read error\n");
        return -1;
    }
    MD5_Final(md5_value, &ctx);
    std::stringstream ss;
    for (int i = 0; i < MD5_SIZE; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (unsigned int)md5_value[i];
    }
    md5 = ss.str();
    return 0;
}
