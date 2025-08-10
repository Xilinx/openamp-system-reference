/*
 * Copyright (c) 2025 Advanced Micro Devices, Inc. (AMD)
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <zephyr/kernel.h>
#include <zephyr/drivers/mbox.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(openamp_rsc_table, LOG_LEVEL_DBG);

extern struct k_sem data_sem;

const struct mbox_dt_spec rx_channel = MBOX_DT_SPEC_GET(DT_PATH(mbox_consumer), rx);
const struct mbox_dt_spec tx_channel = MBOX_DT_SPEC_GET(DT_PATH(mbox_consumer), tx);

static void mbox_callback(const struct device *dev, mbox_channel_id_t id,
		     void *user_data, struct mbox_msg *data)
{
	LOG_DBG("%s: msg received from mb %d\n", __func__, id);
	k_sem_give(&data_sem);
}

void mailbox_cleanup(void)
{
	int ret;

	ret = mbox_set_enabled_dt(&rx_channel, false);
	if (ret < 0) {
		LOG_ERR("%s: Could not disable RX channel %d (%d)\n", __func__, rx_channel.channel_id, ret);
		return;
	}

	ret = mbox_register_callback_dt(&rx_channel, NULL, NULL);
	if (ret < 0)
		 LOG_ERR("%s: Could not wipe RX callback %d (%d)\n", __func__, rx_channel.channel_id, ret);
}

int mailbox_notify(void *priv, uint32_t id)
{
	ARG_UNUSED(priv);

	mbox_send_dt(&tx_channel, NULL);

	return 0;
}

int mailbox_setup(void)
{
	int ret;

	/* set up rx mbox */
	ret = mbox_register_callback_dt(&rx_channel, mbox_callback, NULL);
	if (ret < 0) {
		LOG_ERR("Could not register callback (%d)\n", ret);
		return ret;
	}

	ret = mbox_set_enabled_dt(&rx_channel, true);
	if (ret < 0) {
		LOG_ERR("Could not enable RX channel. (%d)\n", ret);
		return ret;
	}

	return 0;
}
