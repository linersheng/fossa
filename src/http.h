/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

#ifndef NS_HTTP_HEADER_DEFINED
#define NS_HTTP_HEADER_DEFINED

#include "net.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define NS_MAX_HTTP_HEADERS 40
#define NS_MAX_HTTP_REQUEST_SIZE 8192
#define NS_MAX_PATH 1024
#define NS_MAX_HTTP_SEND_IOBUF 4096
#define NS_WEBSOCKET_PING_INTERVAL_SECONDS 5

/* HTTP message */
struct http_message {
  struct ns_str message;    /* Whole message: request line + headers + body */

  /* HTTP Request line (or HTTP response line) */
  struct ns_str method;     /* "GET" */
  struct ns_str uri;        /* "/my_file.html" */
  struct ns_str proto;      /* "HTTP/1.1" */

  /*
   * Query-string part of the URI. For example, for HTTP request
   *    GET /foo/bar?param1=val1&param2=val2
   *    |    uri    |     query_string     |
   *
   * Note that question mark character doesn't belong neither to the uri,
   * nor to the query_string
   */
  struct ns_str query_string;

  /* Headers */
  struct ns_str header_names[NS_MAX_HTTP_HEADERS];
  struct ns_str header_values[NS_MAX_HTTP_HEADERS];

  /* Message body */
  struct ns_str body;            /* Zero-length for requests with no body */
};

struct websocket_message {
  unsigned char *data;
  size_t size;
  unsigned char flags;
};

/* HTTP and websocket events. void *ev_data is described in a comment. */
#define NS_HTTP_REQUEST                 100   /* struct http_message * */
#define NS_HTTP_REPLY                   101   /* struct http_message * */

#define NS_WEBSOCKET_HANDSHAKE_REQUEST  111   /* NULL */
#define NS_WEBSOCKET_HANDSHAKE_DONE     112   /* NULL */
#define NS_WEBSOCKET_FRAME              113   /* struct websocket_message * */
#define NS_WEBSOCKET_CONTROL_FRAME      114   /* struct websocket_message * */

void ns_set_protocol_http_websocket(struct ns_connection *);
void ns_send_websocket_handshake(struct ns_connection *, const char *,
                                 const char *);
void ns_send_websocket_frame(struct ns_connection *, int, const void *, size_t);
void ns_send_websocket_framev(struct ns_connection *, int,
                              const struct ns_str *, int);
void ns_printf_websocket_frame(struct ns_connection *, int, const char *, ...);
void ns_send_http_chunk(struct ns_connection *, const char *, size_t);
void ns_printf_http_chunk(struct ns_connection *, const char *, ...);

/* Websocket opcodes, from http://tools.ietf.org/html/rfc6455 */
#define WEBSOCKET_OP_CONTINUE  0
#define WEBSOCKET_OP_TEXT      1
#define WEBSOCKET_OP_BINARY    2
#define WEBSOCKET_OP_CLOSE     8
#define WEBSOCKET_OP_PING      9
#define WEBSOCKET_OP_PONG      10

/* Utility functions */
struct ns_str *ns_get_http_header(struct http_message *, const char *);
int ns_parse_http(const char *s, int n, struct http_message *req);
int ns_get_http_var(const struct ns_str *, const char *, char *dst, size_t);
struct ns_connection *ns_connect_http(struct ns_mgr *, ns_event_handler_t,
                                      const char *, const char *);

struct ns_serve_http_opts {
  const char *document_root;
};
void ns_serve_http(struct ns_connection *, struct http_message *,
                   struct ns_serve_http_opts);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif  /* NS_HTTP_HEADER_DEFINED */
