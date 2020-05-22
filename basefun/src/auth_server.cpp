#include "http_protocol.h"
#include "common.h"

typedef struct {
	int nopassword;
} authnz_day_rec;

static authn_status authn_check_day(request_rec *r, const char* user, const char* password)
{
	int y, m ,d;
	apr_time_exp_t today;
	const char* const wdays[7] = {"sun", "mon", "tue", "wed", "thu", "fri", "sat"};
	authz_day_rec *cfg = ap_get_module_config(r->per_dir_config, &authnz_day_module);

	if (apr_time_exp_lt(&today, apr_time_now()) != APR_SUCCESS) {
		return AUTH_GENERAL_ERROR;
	}

	if ((strlen(user) < 3) || strncasecmp(user, wdays[today.tm_wday], 3)) {
		return AUTH_USER_NOT_FOUND;
	}

	if (!cfg->nopassword) {
		if (sscanf(password, "%d-%d-%d", &y, &m, &d) != 3) {
			return AUTH_DENIED;
		}

		if ((y != (today.tm_year + 1900)) ||
			 (m != (today.tm_mon + 1)) ||
			 (d != today.tm_mday)) {
			 return AUTH_DENIED;
		}
	}

	if (strlen(r->user) > 3) {
		r->user[3] = '\0';
	}

	ap_set_module_config(r->request_config, &authnz_day_module, r);
	
	return AUTH_GRANTED;
}

static const authn_provider authn_day_provider = 
{
	&authn_check_day,
	NULL
};

static void register_hooks(apr_pool_t *p) {
	ap_register_provider(p, AUTHN_PROVIDER_GROUP, "day", "0", &authn_day_provider);
}

static int authz_day(request_rec *r)
{
	authnz_day_rec *cfg = ap_get_module_config(r->per_dir_config, &authnz_day_module);

	char *day = r->user;
	int m = r->method_number;
	const apr_array_header_t *reqs_arr = ap_requires(r);
	require_line *reqs = reqs_arr ? (require_line *)reqs_arr->elts : NULL;

	char *w;
	const char *t;
	int i;
	int have_day = 0;

	if (ap_get_module_config(r->request_config, &authnz_day_module) == NULL) {
		return DECLINED;
	}

	if (strlen(day) < 3) {
		return DECLINED;
	}

	if (!reqs_arr) {
		return DECLINED;
	}

	for (i = 0; i < reqs_arr->nelts; ++i) {
		if (!(reqs[i].method_mask & (AP_METHOD_BIT << m))) {
			continue;
		}

		t = req[i].requirement;
		w = ap_getword_white(r->pool, &t);
		if (strcasecmp(w, "day")) {
			continue;
		}

		have_day = 1;

		while(*t) {
			w = ap_getword_white(r->pool, &t);
			if ((strlen(w) >= 3) && !strncasecmp(w, day, 3)) {
				return OK;
			}
		}
	}

	if (!have_day) {
		return DECLINED;
	}

	
    ap_log_error( APLOG_MARK, APLOG_ERR, 0, r->server, "Anonymous usage closed on %s", day);

	ap_note_auth_failure(r);
	return HTTP_UNAUTHORIZED;
}

static void register_hooks(apr_pool_t * p)
{
	static const char *const aszSucc[] = {"mod_authz_user.cpp", NULL};
	ap_hook_auth_checker(authz_day, NULL, aszSucc, APR_HOOK_MIDDLE);
	ap_register_provider(p, AUTHN_PROVIDER_GROUP, "day", "0", &authn_day_provider);
}

static void *authnz_day_cr_conf(apr_pool_t *pool, char *x) {
	return apr_pcalloc(pool, sizeof(authnz_day_rec));
}

static const command_rec authnz_day_cmds[] = {
	AP_INIT_FLAG("AuthnDayIgnorePassword", ap_set_flag_slot, 
		(void*)APR_OFFSETOF(authz_day_rec, nopassword), OR_AUTHCFG,
		"Set 'On' to ignore password; 'Off' (default) to require"
		"current date in 2005-11-03 format as  a password"),
	{NULL}
};

module AP_MODULE_DECLARE_DATA authnz_day_module = {
    STANDARD20_MODULE_STUFF, 
    authnz_day_cr_conf,                  /* create per-dir    config structures */
    NULL,                  /* merge  per-dir    config structures */
    NULL,                  /* create per-server config structures */
    NULL,                  /* merge  per-server config structures */
    authnz_day_cmds,                  /* table of config file commands       */
    register_hooks  /* register hooks                      */
};


