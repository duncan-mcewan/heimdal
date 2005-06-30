/*
 * Copyright (c) 1997-2003 Kungliga Tekniska H�gskolan
 * (Royal Institute of Technology, Stockholm, Sweden). 
 * All rights reserved. 
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met: 
 *
 * 1. Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the distribution. 
 *
 * 3. Neither the name of the Institute nor the names of its contributors 
 *    may be used to endorse or promote products derived from this software 
 *    without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND 
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE 
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS 
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY 
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
 * SUCH DAMAGE. 
 */

/* 
 * $Id$ 
 */

#ifndef __KDC_LOCL_H__
#define __KDC_LOCL_H__

#include "headers.h"
#include "kdc.h"

extern sig_atomic_t exit_flag;
extern size_t max_request;
extern const char *port_str;
extern krb5_addresses explicit_addresses;

extern int enable_http;

#define DETACH_IS_DEFAULT FALSE

extern int detach_from_console;

#define _PATH_KDC_CONF		HDB_DB_DIR "/kdc.conf"
#define DEFAULT_LOG_DEST	"0-1/FILE:" HDB_DB_DIR "/kdc.log"

extern struct timeval now;
#define kdc_time (now.tv_sec)

krb5_error_code as_rep (krb5_context context, 
			struct krb5_kdc_configuration *config,
			KDC_REQ*, krb5_data*, const char*, struct sockaddr*);
struct krb5_kdc_configuration *configure(krb5_context context, int argc, char **argv);
krb5_error_code
db_fetch(krb5_context, struct krb5_kdc_configuration *,
	 krb5_principal, hdb_entry **);
void free_ent(krb5_context context, hdb_entry *);
void kdc_log (krb5_context context, 
	      struct krb5_kdc_configuration *config,
	      int, const char*, ...)
    __attribute__ ((format (printf, 4,5)));

char* kdc_log_msg (krb5_context context, 
		   struct krb5_kdc_configuration *config,
		   int, const char*, ...)
    __attribute__ ((format (printf, 4,5)));
char* kdc_log_msg_va (krb5_context context, 
		      struct krb5_kdc_configuration *config,
		      int, const char*, va_list)
    __attribute__ ((format (printf, 4,0)));
void
kdc_openlog(krb5_context context, 
	    struct krb5_kdc_configuration *config);
void
loop(krb5_context context, 
     struct krb5_kdc_configuration *config);
void set_master_key (EncryptionKey);
krb5_error_code tgs_rep (krb5_context context, 
			 struct krb5_kdc_configuration *config,
			 KDC_REQ*, krb5_data*, const char*, struct sockaddr *);
Key* unseal_key (Key*);
krb5_error_code
check_flags(krb5_context context, 
	    struct krb5_kdc_configuration *config,
	    hdb_entry *client, const char *client_name,
			    hdb_entry *server, const char *server_name,
			    krb5_boolean is_as_req);

krb5_error_code get_des_key(krb5_context context, hdb_entry*, krb5_boolean, krb5_boolean, Key**);
krb5_error_code
encode_v4_ticket(krb5_context context, 
		 struct krb5_kdc_configuration *config,
		 void *buf, size_t len, const EncTicketPart *et,
		 const PrincipalName *service, size_t *size);
krb5_error_code
do_524(krb5_context context, 
       struct krb5_kdc_configuration *config,
       const Ticket *t, krb5_data *reply,
       const char *from, struct sockaddr *addr);

#ifdef HAVE_OPENSSL
#define des_new_random_key des_random_key
#endif

#ifdef PKINIT
typedef struct pk_client_params pk_client_params;
krb5_error_code pk_initialize(const char *, const char *);
krb5_error_code pk_rd_padata(krb5_context, KDC_REQ *,
			     PA_DATA *, pk_client_params **);
krb5_error_code	pk_mk_pa_reply(krb5_context,
			       pk_client_params *,
			       const hdb_entry *,
			       const KDC_REQ *,
			       krb5_keyblock **,
			       METHOD_DATA *);
krb5_error_code pk_check_client(krb5_context, krb5_principal,
				const hdb_entry *, 
				pk_client_params *, char **);
void pk_free_client_param(krb5_context, pk_client_params *);
#endif

/*
 * Kerberos 4
 */

krb5_error_code db_fetch4 (krb5_context context, 
			   struct krb5_kdc_configuration *config,
			   const char*, const char*, const char*, hdb_entry**);
krb5_error_code do_version4 (krb5_context context, 
			     struct krb5_kdc_configuration *config,
			     unsigned char*, size_t, krb5_data*, const char*, 
			     struct sockaddr_in*);
int maybe_version4 (unsigned char*, int);

krb5_error_code do_kaserver (krb5_context context, 
			     struct krb5_kdc_configuration *config,
			     unsigned char*, size_t, krb5_data*, const char*, 
			     struct sockaddr_in*);

int kdc_process_generic_request(krb5_context context, 
				struct krb5_kdc_configuration *config,
				unsigned char *buf, 
				size_t len, 
				krb5_data *reply,
				krb5_boolean *prependlength,
				const char *from,
				struct sockaddr *addr);

#endif /* __KDC_LOCL_H__ */
