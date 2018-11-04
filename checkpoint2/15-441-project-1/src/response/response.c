#include "response.h"
#include <string.h>

char *WWW;

const char* server_name = "liso/1.0";
const char* default_index_file = "index.html";
const char* http_version = "HTTP/1.1";

// 处理http请求
int handle_http_request(int clientfd, char* buf, int nbytes)
{
    Request *request = parse(buf, nbytes, clientfd);
    // 错误的请求
    if (request == NULL) {
        response400(clientfd);
    }

    // 不支持的HTTP版本
    if(strcmp(http_version, request->http_version))
    {
        response505(clientfd);
    }


    if(!strcmp(request->http_method, "POST"))
    {
        response_post(clientfd, request);
    }
    else if(!strcmp(request->http_method, "GET"))
    {
        response_get(clientfd, request);
    }
    else if(!strcmp(request->http_method, "HEAD"))
    {
        response_head(clientfd, request);
    }
    else
    {
        response501(clientfd);
    }

    return 0;
}



int response_get(int clientfd, Request* request)
{
    char response[SIZE];
    char fullpath[1024];
    char extension[16];
    char mime_type[64];
    char curr_time[256];
    char last_modified[256];
    size_t content_length;
    char content_len_str[16];
    char connection_header_val[32];

    strcpy(fullpath, WWW);
    strcat(fullpath, request->http_uri);

    if (is_dir(fullpath))
        strcat(fullpath, default_index_file);

    // 如果没有那个文件就返回404
    if (access(fullpath, F_OK) < 0)
    {
        dump_log("[response] request file not found\n");
        response404(clientfd, response);
        return 0;
    }

    // 得到 content type
    get_extension(fullpath, extension);
    get_mime_type(extension, mime_type);

    // 得到 content length
    content_length = get_file_len(fullpath);
    sprintf(content_len_str, "%zu", content_length);

    // 得到当前时间
    get_curr_time(curr_time, 256);

    // 得到最后修改时间
    get_flmodified(fullpath, last_modified, 256);

    // 构造 response
    strcat(response, http_version);
    strcat(response, " 200 OK\r\n");

    strcat(response, "Server: ");
    strcat(response, server_name);
    strcat(response, "\r\n");

    strcat(response, "Date: ");
    strcat(response, curr_time);
    strcat(response, "\r\n");

    strcat(response, "Content-Length: ");
    strcat(response, content_len_str);
    strcat(response, "\r\n");

    strcat(response, "Content-type: ");
    strcat(response, mime_type);
    strcat(response, "\r\n");

    strcat(response, "Last-modified: ");
    strcat(response, last_modified);
    strcat(response, "\r\n");

    // 检查connect是什么值
    memset(connection_header_val, 0, sizeof(connection_header_val));
    get_header_value(request, "Connection", connection_header_val);
    if (!strcmp(connection_header_val, "close"))
    {
        strcat(response, "Connection: close\r\n");
    }
    else
    {
        strcat(response, "Connection: keep-alive\r\n");
    }

    strcat(response, "\r\n");

    // printf("[response] response = \n%s\n", response);
    // printf("[response] fullpath = %s\n", fullpath);

    Sendn(clientfd, response, strlen(response));

    send_file_to_client(clientfd, fullpath);

    return 0;
}

int response_head(int clientfd, Request *request)
{
    char response[SIZE];
    char fullpath[1024];
    char extension[16];
    char mime_type[64];
    char curr_time[256];
    char last_modified[256];
    size_t content_length;
    char content_len_str[16];
    char connection_header_val[32];

    strcpy(fullpath, WWW);
    strcat(fullpath, request->http_uri);

    if (is_dir(fullpath))
        strcat(fullpath, default_index_file);

    // 如果没有那个文件就返回404
    if (access(fullpath, F_OK) < 0)
    {
        dump_log("[response] request file not found\n");
        response404(clientfd, response);
        return 0;
    }

    // 得到 content type
    get_extension(fullpath, extension);
    get_mime_type(extension, mime_type);

    // 得到 content length
    content_length = get_file_len(fullpath);
    sprintf(content_len_str, "%zu", content_length);

    // 得到当前时间
    get_curr_time(curr_time, 256);

    // 得到最后修改时间
    get_flmodified(fullpath, last_modified, 256);

    // 构造 response
    strcat(response, http_version);
    strcat(response, " 200 OK\r\n");

    strcat(response, "Server: ");
    strcat(response, server_name);
    strcat(response, "\r\n");

    strcat(response, "Date: ");
    strcat(response, curr_time);
    strcat(response, "\r\n");

    strcat(response, "Content-Length: ");
    strcat(response, content_len_str);
    strcat(response, "\r\n");

    strcat(response, "Content-type: ");
    strcat(response, mime_type);
    strcat(response, "\r\n");

    strcat(response, "Last-modified: ");
    strcat(response, last_modified);
    strcat(response, "\r\n");

    // 检查connect是什么值
    memset(connection_header_val, 0, sizeof(connection_header_val));
    get_header_value(request, "Connection", connection_header_val);
    if (!strcmp(connection_header_val, "close"))
    {
        strcat(response, "Connection: close\r\n");
    }
    else
    {
        strcat(response, "Connection: keep-alive\r\n");
    }

    strcat(response, "\r\n");

    Sendn(clientfd, response, strlen(response));

    return 0;
}

int response_post(int clientfd, Request *request)
{
    char response[SIZE];
    char content_length[32];

    memset(content_length, 0, sizeof(content_length));
    get_header_value(request, "Content-Length", content_length);
    if (strlen(content_length) == 0)
    {
        strcat(response, http_version);
        strcat(response, " 401 Length Reqired\r\n");
        strcat(response, "\r\n");

        Sendn(clientfd, response, strlen(response));
        return 0;
    }

    strcat(response, http_version);
    strcat(response, " 200 OK\r\n");
    strcat(response, "\r\n");

    Sendn(clientfd, response, strlen(response));
    return 0;
}


// 根据扩展名确定MIME类型
void get_mime_type(const char *mime, char *type)
{
    if (!strcmp(mime, "html"))
    {
        strcpy(type, "text/html");
    }
    else if (!strcmp(mime, "css"))
    {
        strcpy(type, "text/css");
    }
    else if (!strcmp(mime, "png"))
    {
        strcpy(type, "image/png");
    }
    else if (!strcmp(mime, "jpeg"))
    {
        strcpy(type, "image/jpeg");
    }
    else if (!strcmp(mime, "gif"))
    {
        strcpy(type, "image/gif");
    }
    else
    {
        strcpy(type, "application/octet-stream");
    }
}

// 根据给定键，设置值
// 如果不存在就不设置hvalue
void get_header_value(Request *request, const char *hname, char *hvalue)
{
    for (int i = 0; i < request->header_count; i++)
    {
        if (!strcmp(request->headers[i].header_name, hname))
        {
            strcpy(hvalue, request->headers[i].header_value);
            return;
        }
    }
}

// 没有请求的指定文件，返回404
void response404(int clientfd, char *response)
{
    strcat(response, http_version);
    strcat(response, " 404 Not Found\r\n\r\n");
    printf("[response] response = \n%s\n", response);
    Sendn(clientfd, response, strlen(response));
}

// 未实现的请求功能，返回501
void response501(int clientfd)
{
    char response[1024];
    strcat(response, http_version);
    strcat(response, " 501 Not Implemented\r\n\r\n");
    // printf("[response] response = \n%s\n", response);
    Sendn(clientfd, response, strlen(response));
}

// 不支持的HTTP版本
void response505(int clientfd)
{
    char response[1024];
    strcat(response, http_version);
    strcat(response, " 505 HTTP Version not supported\r\n");
    strcat(response, "Connection: close\r\n");
    // printf("[response] response = \n%s\n", response);
    Sendn(clientfd, response, strlen(response));
}


void response400(int clientfd)
{
    char response[1024];
    strcat(response, http_version);
    strcat(response, " 400 Bad request\r\n");
    // printf("[response] response = \n%s\n", response);
    Sendn(clientfd, response, strlen(response));
}
