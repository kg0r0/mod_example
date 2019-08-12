/* 
**  mod_example.c -- Apache sample example module
**  [Autogenerated via ``apxs -n example -g'']
**
**  To play with this sample module first compile it into a
**  DSO file and install it into Apache's modules directory 
**  by running:
**
**    $ apxs -c -i mod_example.c
**
**  Then activate it in Apache's httpd.conf file for instance
**  for the URL /example in as follows:
**
**    #   httpd.conf
**    LoadModule example_module modules/mod_example.so
**    <Location /example>
**    SetHandler example
**    </Location>
**
**  Then after restarting Apache via
**
**    $ apachectl restart
**
**  you immediately can request the URL /example and watch for the
**  output of this module. This can be achieved for instance via:
**
**    $ lynx -mime_header http://localhost/example 
**
**  The output should be similar to the following one:
**
**    HTTP/1.1 200 OK
**    Date: Tue, 31 Mar 1998 14:42:22 GMT
**    Server: Apache/1.3.4 (Unix)
**    Connection: close
**    Content-Type: text/html
**  
**    The sample page from mod_example.c
*/ 

#include "httpd.h"
#include "http_config.h"
#include "http_protocol.h"
#include "ap_config.h"

/* The sample content handler */
static int example_handler(request_rec *r)
{
    apr_table_t *GET;

    if (strcmp(r->handler, "example")) {
        return DECLINED;
    }

    /* Parse the GET and, optionally, the POST data sent to us */ 
    ap_args_to_table(r, &GET);

    r->content_type = "text/html";

    /* Get the "q" key from the query string, if any. */
    const char *query = apr_table_get(GET, "q");
    if (!query) query = "Not Found";

    if (!r->header_only)
        ap_rprintf(r, "%s \n", query);
    return OK;
}

static void example_register_hooks(apr_pool_t *p)
{
    ap_hook_handler(example_handler, NULL, NULL, APR_HOOK_MIDDLE);
}

/* Dispatch list for API hooks */
module AP_MODULE_DECLARE_DATA example_module = {
    STANDARD20_MODULE_STUFF, 
    NULL,                  /* create per-dir    config structures */
    NULL,                  /* merge  per-dir    config structures */
    NULL,                  /* create per-server config structures */
    NULL,                  /* merge  per-server config structures */
    NULL,                  /* table of config file commands       */
    example_register_hooks  /* register hooks                      */
};

