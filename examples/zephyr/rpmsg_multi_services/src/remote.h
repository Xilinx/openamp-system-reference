/*
 * Copyright (c) 2025 Advanced Micro Devices, Inc. (AMD)
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __MAILBOX_REMOTE_H__
#define __MAILBOX_REMOTE_H__
int mailbox_notify(void *priv, uint32_t id);
int mailbox_setup(void);
void mailbox_cleanup(void);
#endif /* __MAILBOX_REMOTE_H__ */
