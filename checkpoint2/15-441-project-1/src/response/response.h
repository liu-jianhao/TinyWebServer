#ifndef __RESPONSE__H
#define __RESPONSE__H

#include "../parse/parse.h"
#include "../client_pool/client_pool.h"
#include "../io/io.h"

int handle_http_request(int clientfd, char* buf, int nbytes);

int response_get(int clientfd, Request* request);
int response_head(int clientfd, Request* request);
int response_post(int clientfd, Request* request);

void get_mime_type(const char *mime, char *type);
void get_header_value(Request *request, const char *hname, char *hvalue);

void response404(int clientfd, char *response);
void response501(int clientfd);
void response505(int clientfd);
void response400(int clientfd);


#endif
