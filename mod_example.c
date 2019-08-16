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

#include <stdio.h>
#include "apr_hash.h"
#include "ap_config.h"
#include "ap_provider.h"
#include "httpd.h"
#include "http_core.h"
#include "http_config.h"
#include "http_log.h"
#include "http_protocol.h"
#include "http_request.h"

/* Our configuration prototype and declaration: */
typedef struct {
    int         enabled;      /* Enable or disable our module */
    const char *path;         /* Some path to...something */
    int         typeOfAction; /* 1 means action A, 2 means action B and so on */
} example_config;

static example_config config;

/* Handler for the "examplePath" directive */
const char *example_set_path(cmd_parms *cmd, void *cfg, const char *arg)
{
    config.path = arg;
    return NULL;
}

/* Handler for the "exampleAction" directive */
/* Let's pretend this one takes one argument (file or db), and a second (deny or allow), */
/* and we store it in a bit-wise manner. */
const char *example_set_action(cmd_parms *cmd, void *cfg, const char *arg1, const char *arg2)
{
    if(!strcasecmp(arg1, "file")) config.typeOfAction = 0x01;
    else config.typeOfAction = 0x02;
    
    if(!strcasecmp(arg2, "deny")) config.typeOfAction += 0x10;
    else config.typeOfAction += 0x20;
    return NULL;
}

/* The directive structure for our name tag: */
static const command_rec        example_directives[] =
{
    AP_INIT_TAKE1("exampleEnabled", example_set_enabled, NULL, RSRC_CONF, "Enable or disable mod_example"),
    AP_INIT_TAKE1("examplePath", example_set_path, NULL, RSRC_CONF, "The path to whatever"),
    AP_INIT_TAKE2("exampleAction", example_set_action, NULL, RSRC_CONF, "Special action value!"),
    { NULL }
};

/* The sample content handler */
static int example_handler(request_rec *r)
{
    if(!r->handler || strcmp(r->handler, "example-handler")) return(DECLINED);
    ap_set_content_type(r, "text/plain");
    ap_rprintf(r, "Enabled: %u\n", config.enabled);
    ap_rprintf(r, "Path: %s\n", config.path);
    ap_rprintf(r, "TypeOfAction: %x\n", config.typeOfAction);
    return OK;
}

/*
 *  ==============================================================================
 *   The hook registration function (also initializes the default config values):
 *  ==============================================================================
 */
static void example_register_hooks(apr_pool_t *p)
{
    config.enabled = 1;
    config.path = "/foo/bar";
    config.typeOfAction = 3;
    ap_hook_handler(example_handler, NULL, NULL, APR_HOOK_LAST);
}

/*
 *  ==============================================================================
 *   Our module name tag:
 *  ==============================================================================
 */
module AP_MODULE_DECLARE_DATA example_module = {
    STANDARD20_MODULE_STUFF, 
    NULL,                  /* create per-dir    config structures */
    NULL,                  /* merge  per-dir    config structures */
    NULL,                  /* create per-server config structures */
    NULL,                  /* merge  per-server config structures */
    NULL,                  /* table of config file commands       */
    example_directives,    /* Any directives we may have for httpd */
    example_register_hooks  /* register hooks                      */
};

