/*
 * Copyright (c) 1997 Kungliga Tekniska H�gskolan
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
 * 3. All advertising materials mentioning features or use of this software 
 *    must display the following acknowledgement: 
 *      This product includes software developed by Kungliga Tekniska 
 *      H�gskolan and its contributors. 
 *
 * 4. Neither the name of the Institute nor the names of its contributors 
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

/* $Id$ */

#ifndef GSSAPI_LOCL_H
#define GSSAPI_LOCL_H

#include <stdlib.h>
#include <string.h>

#ifdef HAVE_NETDB_H
#include <netdb.h>
#endif

#include <gssapi.h>

#include <krb5.h>
#include <des.h>
#include <krb5_locl.h>
#include <der.h>

extern krb5_context gssapi_krb5_context;

void gssapi_krb5_init (void);

krb5_error_code
gssapi_krb5_create_8003_checksum (
		      const gss_channel_bindings_t input_chan_bindings,
		      OM_uint32 flags,
		      Checksum *result);

OM_uint32
gssapi_krb5_encapsulate(
			krb5_data *in_data,
			gss_buffer_t output_token,
			u_char *type);

OM_uint32
gssapi_krb5_decapsulate(
			gss_buffer_t input_token_buffer,
			krb5_data *out_data,
			u_char *type);

void
gssapi_krb5_encap_length (size_t data_len,
			  size_t *len,
			  size_t *total_len);

u_char *
gssapi_krb5_make_header (u_char *p,
			 size_t len,
			 u_char *type);

OM_uint32
gssapi_krb5_verify_header(u_char **str,
			  size_t total_len,
			  u_char *type);

#endif
