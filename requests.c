#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

char *compute_get_request(char *host, char *url, char *query_params,
                          char **cookies, int cookies_count, char *jwtToken)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char line[LINELEN];

    if (query_params != NULL) {
        sprintf(line, "GET %s?%s HTTP/1.1\r\n", url, query_params);
    } else {
        sprintf(line, "GET %s HTTP/1.1\r\n", url);
    }
    strcat(message, line);

    sprintf(line, "Host: %s\r\n", host);
    strcat(message, line);

    if (cookies != NULL) {
        strcpy(line, "Cookie: ");
        for (int i = 0; i < cookies_count - 1; i++) {
            sprintf(line + strlen(line), "%s;", cookies[i]);
        }
        sprintf(line + strlen(line), "%s\r\n", cookies[cookies_count - 1]);
        strcat(message, line);
    }

    if (jwtToken != NULL) {
        sprintf(line, "Authorization: Bearer %s\r\n", jwtToken);
        strcat(message, line);
    }

    strcat(message, "\r\n");
    return message;
}

char *compute_post_request(char *host, char *url, char* content_type, char **body_data,
                            int body_data_fields_count, char **cookies, int cookies_count,
                            char *jwtToken)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));
    char *body_data_buffer = calloc(LINELEN, sizeof(char));

    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);
    
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    sprintf(line, "Content-Type: %s", content_type);
    compute_message(message, line);
    int content_length = 0;
    for (int i = 0; i < body_data_fields_count; i++) {
        content_length += strlen(body_data[i]);
    }
    if (body_data_fields_count > 1) {
        content_length += body_data_fields_count - 1;
    }


    sprintf(line, "Content-Length: %d", content_length);
    compute_message(message, line);

    if (cookies != NULL) {
       for (int i = 0; i < cookies_count; i++) {
            compute_message(message, cookies[i]);
        }
    }

    if (jwtToken != NULL) {
        sprintf(line, "Authorization: Bearer %s", jwtToken);
        compute_message(message, line);
    }

    compute_message(message, "");
    for (int i = 0; i < body_data_fields_count; i++) {
        strcat(body_data_buffer, body_data[i]);
        if (i != body_data_fields_count - 1) {
            strcat(body_data_buffer, "&");
        }
    }

    memset(line, 0, LINELEN);
    strcat(message, body_data_buffer);

    free(line);
    free(body_data_buffer);
    return message;
}

char *compute_delete_request(char *host, char *url, char *query_params,
                          char **cookies, int cookies_count, char *jwtToken)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char line[LINELEN];

    if (query_params != NULL) {
        sprintf(line, "DELETE %s?%s HTTP/1.1", url, query_params);
    } else {
        sprintf(line, "DELETE %s HTTP/1.1", url);
    }
    compute_message(message, line);

    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    if (cookies != NULL) {
        sprintf(line, "Cookie: %s", cookies[0]);
        compute_message(message, line);
    }

    if (jwtToken != NULL) {
        sprintf(line, "Authorization: Bearer %s", jwtToken);
        compute_message(message, line);
    }

    strcat(message, "\r\n");
    return message;
}
