/*
 * Copyright (c) 2025 Advanced Micro Devices, Inc. (AMD)
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <zephyr/kernel.h>
#include <zephyr/drivers/ipm.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(openamp_rsc_table, LOG_LEVEL_DBG);

#include "remote.h"

extern struct k_sem data_sem;

static const struct device *const ipm_handle =
	DEVICE_DT_GET(DT_CHOSEN(zephyr_ipc));

static void platform_ipm_callback(const struct device *dev, void *context,
				  uint32_t id, volatile void *data)
{
	LOG_DBG("%s: msg received from mb %d\n", __func__, id);
	k_sem_give(&data_sem);
}

void mailbox_cleanup(void)
{
	ipm_set_enabled(ipm_handle, 0);
}

int mailbox_notify(void *priv, uint32_t id)
{
	ARG_UNUSED(priv);

	LOG_DBG("%s: msg received\n", __func__);
	ipm_send(ipm_handle, 0, id, NULL, 0);

	return 0;
}

int mailbox_setup(void)
{
	/* setup IPM */
	if (!device_is_ready(ipm_handle)) {
		LOG_DBG("IPM device is not ready\n");
		return -1;
	}

	ipm_register_callback(ipm_handle, platform_ipm_callback, NULL);

	status = ipm_set_enabled(ipm_handle, 1);
	if (status) {
		LOG_DBG("ipm_set_enabled failed\n");
		return -1;
	}
	return 0;
}
