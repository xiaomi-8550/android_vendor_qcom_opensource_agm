/*
 * Copyright (c) 2019, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of The Linux Foundation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _SESSION_OBJ_H_
#define _SESSION_OBJ_H_

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "agm_priv.h"
#include "list.h"
#include "metadata.h"
#include "graph.h"

enum aif_state {
	AIF_CLOSED,
	AIF_CLOSE,
	AIF_OPEN,
	AIF_OPENED,
	AIF_STOPPED,
	AIF_PREPARED,
	AIF_STARTED,
};

struct aif {
	struct listnode node;
	uint32_t aif_id;
	struct device_obj *dev_obj;
	enum aif_state state;
	struct agm_meta_data_gsl sess_aif_meta;
	void *params;
	size_t params_size;
};

enum session_state {
	SESSION_CLOSED,
	SESSION_OPENED,
	SESSION_PREPARED,
	SESSION_STARTED,
	SESSION_PAUSED,
	SESSION_STOPPED,
};

struct session_cb {
	struct listnode node;
	agm_event_cb cb;
	enum event_type evt_type;
	void *client_data;
};

struct session_obj {
	struct listnode node;
	uint32_t sess_id;
	enum session_state state;
	struct agm_meta_data_gsl sess_meta;
	struct listnode aif_pool;
	struct listnode cb_pool;
	struct graph_obj *graph;
	struct agm_session_config stream_config;
	struct agm_media_config media_config;
	struct agm_buffer_config buffer_config;
	void *params;
	size_t params_size;
	uint32_t loopback_sess_id;
	bool loopback_state;
	uint32_t ec_ref_aif_id;
	bool ec_ref_state;
	pthread_mutex_t lock;
};

struct session_pool {
	struct listnode session_list;
	pthread_mutex_t lock;
};

struct session_pool *sess_pool;

int session_obj_init();
int session_obj_deinit();
int session_obj_get(int session_id, struct session_obj **sess_obj);
int session_obj_open(uint32_t session_id, struct session_obj **sess_obj);
int session_obj_set_config(struct session_obj *session, 
	struct agm_session_config *stream_config,
	struct agm_media_config *media_config,
	struct agm_buffer_config *buffer_config);
int session_obj_prepare(struct session_obj *sess_obj);
int session_obj_start(struct session_obj *sess_obj);
int session_obj_stop(struct session_obj *sess_obj);
int session_obj_close(struct session_obj *sess_obj);
int session_obj_pause(struct session_obj *sess_obj);
int session_obj_resume(struct session_obj *sess_obj);
int session_obj_read(struct session_obj *sess_obj, void *buff, size_t count);
int session_obj_write(struct session_obj *sess_obj, void *buff, size_t count);
int session_obj_sess_aif_connect(struct session_obj *sess_obj,
	uint32_t audio_intf, bool state);
int session_obj_set_sess_metadata(struct session_obj *sess_obj, struct agm_meta_data *metadata);
int session_obj_set_sess_aif_metadata(struct session_obj *sess_obj,
	uint32_t audio_intf, struct agm_meta_data *metadata);
int session_obj_set_sess_params(struct session_obj *sess_obj,
	void* payload, size_t size);
int session_obj_set_sess_aif_params(struct session_obj *sess_obj,
	uint32_t audio_intf,
	void *payload, size_t size);
int session_obj_set_sess_aif_params_with_tag(struct session_obj *sess_obj,
	uint32_t aif_id,
	struct agm_tag_config *tag_config);
int session_obj_set_sess_aif_cal(struct session_obj *sess_obj,
	uint32_t aif_id,
	struct agm_cal_config *cal_config);
int session_obj_get_tag_with_module_info(struct session_obj *sess_obj, uint32_t audio_intf, void *payload, size_t *size);
size_t session_obj_hw_processed_buff_cnt(struct session_obj *sess_obj, enum direction dir);
int session_obj_set_loopback(struct session_obj *sess_obj, uint32_t playback_sess_id, bool state);
int session_obj_set_ec_ref(struct session_obj *sess_obj, uint32_t aif_id, bool state);
int session_obj_register_cb(struct session_obj *sess_obj, agm_event_cb cb, enum event_type evt_type, void *client_data);
int session_obj_register_for_events(struct session_obj *sess_obj, struct agm_event_reg_cfg *evt_reg_cfg);
int session_obj_set_ec_ref(struct session_obj *sess_obj, uint32_t aif_id, bool state);
#endif
