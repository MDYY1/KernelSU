#include <linux/version.h>
#include <linux/fs.h>
#include <linux/nsproxy.h>
#include <linux/sched/task.h>
#include <linux/uaccess.h>
#include <linux/filter.h>
#include "klog.h" // IWYU pragma: keep
#include "seccomp_cache.h"

void ksu_seccomp_clear_cache(struct seccomp_filter *filter, int nr)
{

}

void ksu_seccomp_allow_cache(struct seccomp_filter *filter, int nr)
{

}
