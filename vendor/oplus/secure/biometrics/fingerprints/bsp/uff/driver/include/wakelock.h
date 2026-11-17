/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2018-2020 Oplus. All rights reserved.
 */

#ifndef _LINUX_WAKELOCK_H
#define _LINUX_WAKELOCK_H

#include <linux/ktime.h>
#include <linux/device.h>
#include <linux/pm_wakeup.h>

/* A wake_lock prevents the system from entering suspend or other low power
 * states when active. If the type is set to WAKE_LOCK_SUSPEND, the wake_lock
 * prevents a full system suspend.
 */

enum {
    WAKE_LOCK_SUSPEND, /* Prevent suspend */
    WAKE_LOCK_TYPE_COUNT
};

struct wake_lock {
    struct wakeup_source *ws;
};

static inline void wake_lock_init(struct wake_lock *lock, int type,
                    const char *name)
{
    if (lock)
        lock->ws = wakeup_source_register(NULL, name);
}

static inline void wake_lock_destroy(struct wake_lock *lock)
{
    if (!lock || !lock->ws)
        return;
    __pm_relax(lock->ws);
    wakeup_source_unregister(lock->ws);
    lock->ws = NULL;
}

static inline void wake_lock(struct wake_lock *lock)
{
    if (lock && lock->ws)
        __pm_stay_awake(lock->ws);
}

static inline void wake_lock_timeout(struct wake_lock *lock, long timeout)
{
    if (lock && lock->ws)
        __pm_wakeup_event(lock->ws, jiffies_to_msecs(timeout));
}

static inline void wake_unlock(struct wake_lock *lock)
{
    if (lock && lock->ws)
        __pm_relax(lock->ws);
}

static inline int wake_lock_active(struct wake_lock *lock)
{
    return lock && lock->ws && lock->ws->active;
}
#endif //_LINUX_WAKELOCK_H
