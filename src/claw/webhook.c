#include <h2o.h>

#include <jansson.h>
#include <webhook.h>

int parse_json(const char *json, size_t json_len, struct json_t **obj) {
  json_t *root = json_object();
  json_error_t error;

  root = json_loadb(json, json_len, 0, NULL);
  if (!root) {
    fprintf(stderr, "Error parsing json on line %d: %s\n", error.line,
            error.text);
    return -1;
  }

  *obj = root;
  return 0;
}

int hook_listener(h2o_handler_t *self, h2o_req_t *req) {
  if (h2o_memis(req->method.base, req->method.len, H2O_STRLIT("POST"))) {
    static h2o_generator_t generator = {NULL, NULL};

    ssize_t header_index =
        h2o_find_header(&req->headers, H2O_TOKEN_CONTENT_TYPE, 0);
    if (header_index == -1)
      return -1;

    if (!h2o_memis(req->headers.entries[header_index].value.base,
                   req->headers.entries[header_index].value.len,
                   H2O_STRLIT("application/json")))
      return -1;

    struct json_t *json_obj;
    if (parse_json(req->entity.base, req->entity.len, &json_obj) != 0)
      return -1;
    char *json_dumpb = json_dumps(json_obj, JSON_INDENT(2));
    fprintf(stderr, "hook_listener: json_obj: %s\n", json_dumpb);

    json_decref(json_obj);
    free(json_dumpb);

    req->res.status = 200;
    req->res.reason = "OK";

    h2o_add_header(&req->pool, &req->res.headers, H2O_TOKEN_CONTENT_TYPE, NULL,
                   H2O_STRLIT("text/plain; charset=utf-8"));
    h2o_start_response(req, &generator);
    h2o_send(req, &req->entity, 1, 1);
    return 0;
  }
  return -1;
}
