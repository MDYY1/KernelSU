#ifndef __KSU_UTIL_H
#define __KSU_UTIL_H

#include <linux/types.h>
#include <linux/uaccess.h>
#include <linux/mm.h>
#include <asm/pgtable.h>

// 老内核没有 strncpy_from_user_nofault，提供替代实现
static long strncpy_from_user_nofault(char *dst, const char __user *src, long count)
{
    long res;
    
    pagefault_disable();
    res = strncpy_from_user(dst, src, count);
    pagefault_enable();
    
    return res;
}

#ifndef preempt_enable_no_resched_notrace
#define preempt_enable_no_resched_notrace()                                    \
    do {                                                                       \
        barrier();                                                             \
        __preempt_count_dec();                                                 \
    } while (0)
#endif

#ifndef preempt_disable_notrace
#define preempt_disable_notrace()                                              \
    do {                                                                       \
        __preempt_count_inc();                                                 \
        barrier();                                                             \
    } while (0)
#endif

bool try_set_access_flag(unsigned long addr);

#endif
