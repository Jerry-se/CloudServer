#include <iostream>
#include <fcgi_stdio.h>
#include "fcgi_config.h"
#include <stdlib.h>
#include <sys/types.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#define MAX_CONTENT_LENGTH 4096

using namespace std;

int main(void)
{
  int i;

  while(FCGI_Accept() >= 0){
    char inputBuffer[MAX_CONTENT_LENGTH] = { 0 };
    int len = atoi(getenv("CONTENT_LENGTH"));
    if (len > MAX_CONTENT_LENGTH) len = MAX_CONTENT_LENGTH;
    int iRead = 0, x;
    while (iRead < len) {
      if (EOF == (x = FCGI_fgetc(stdin)))
        break;
      inputBuffer[iRead++] = x;
    }
    printf("Content-type: text/html\r\n\r\n" \
            "<html><head><title>Test</title></head>" \
            "<body>%s %d " \
            "<br> SERVER_SOFTWARE:%s " \
            "<br> QUERY_STRING: %s " \
            "<br> REQUEST_METHOD: %s " \
            "<br> CONTENT_TYPE: %s " \
            "<br> CONTENT_LENGTH: %s " \
            "<br> SCRIPT_FILENAME: %s " \
            "<br> SCRIPT_NAME: %s " \
            "<br> REQUEST_URI: %s " \
            "<br> DOCUMENT_URI: %s " \
            "<br> DOCUMENT_ROOT: %s " \
            "<br> SERVER_PROTOCOL: %s " \
            "<br> REMOTE_ADDR: %s " \
            "<br> REMOTE_PORT: %s " \
            "<br> SERVER_ADDR: %s " \
            "<br> SERVER_PORT: %s " \
            "<br> SERVER_NAME: %s " \
            "<br> POST_CONTENT: %s " \
            "</body></html>",
            "Hello World", i++,
            getenv("SERVER_SOFTWARE"),
            getenv("QUERY_STRING"),
            getenv("REQUEST_METHOD"),
            getenv("CONTENT_TYPE"),
            getenv("CONTENT_LENGTH"),
            getenv("SCRIPT_FILENAME"),
            getenv("SCRIPT_NAME"),
            getenv("REQUEST_URI"),
            getenv("DOCUMENT_URI"),
            getenv("DOCUMENT_ROOT"),
            getenv("SERVER_PROTOCOL"),
            getenv("REMOTE_ADDR"),
            getenv("REMOTE_PORT"),
            getenv("SERVER_ADDR"),
            getenv("SERVER_PORT"),
            getenv("SERVER_NAME"),
            inputBuffer);
  }

  return 0;
}
