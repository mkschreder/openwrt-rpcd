/*
 * rpcd - UBUS RPC server
 *
 *   Copyright (C) 2013-2014 Jo-Philipp Wich <jow@openwrt.org>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <sys/types.h>
#include <dirent.h>
#include <libubus.h>
#include <rpcd/plugin.h>

enum {
	RPC_D_DEVICE,
	__RPC_D_MAX,
};

static const struct blobmsg_policy rpc_device_policy[__RPC_D_MAX] = {
	[RPC_D_DEVICE] = { .name = "device", .type = BLOBMSG_TYPE_STRING },
};

static int rpc_test_info(struct ubus_context *ctx, struct ubus_object *obj, struct ubus_request_data *req, const char *method, struct blob_attr *msg){
	struct blob_buf buf = {0};
	void *c; 
	
	blob_buf_init(&buf, 0);

	c = blobmsg_open_array(&buf, "hardware");
	blobmsg_add_u32(&buf, "test", 32); 
	blobmsg_close_array(&buf, c);

	ubus_send_reply(ctx, req, buf.head);

	blob_buf_free(&buf); 
	
	return UBUS_STATUS_OK;
}


static int rpc_test_api_init(const struct rpc_daemon_ops *o, struct ubus_context *ctx) {
	static const struct ubus_method test_methods[] = {
		UBUS_METHOD_NOARG("info",        rpc_test_info)
	};

	static struct ubus_object_type test_type =
		UBUS_OBJECT_TYPE("rpcd-test-type", test_methods);

	static struct ubus_object obj = {
		.name = "test",
		.type = &test_type,
		.methods = test_methods,
		.n_methods = ARRAY_SIZE(test_methods),
	};

	return ubus_add_object(ctx, &obj);
}

struct rpc_plugin rpc_plugin = {
	.init = rpc_test_api_init
};
