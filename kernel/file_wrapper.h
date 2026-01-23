#ifndef KSU_FILE_WRAPPER_H
#define KSU_FILE_WRAPPER_H

/* file_operations 结构体兼容性 */
#include <linux/fs.h>
#include <linux/version.h>
#include <linux/poll.h>

/* __poll_t 类型兼容性 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 16, 0)
// 老内核使用 unsigned int 而不是 __poll_t
typedef unsigned int __poll_t;
#endif

/* file_operations 新增字段兼容性 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 1, 0)
// 新内核有这些字段
#else
// 老内核，定义这些字段的访问宏为空

// iopoll 字段（Linux 5.1+）
#ifndef CONFIG_IO_POLL
#define file_ops_iopoll(f_op) NULL
#else
#define file_ops_iopoll(f_op) ((f_op)->iopoll)
#endif

// remap_file_range 字段（Linux 4.20+）
#define file_ops_remap_file_range(f_op) NULL

// fadvise 字段（Linux 5.2+）
#define file_ops_fadvise(f_op) NULL

// mmap_supported_flags 字段（Linux 5.12+）
#define file_ops_mmap_supported_flags(f_op) 0
#endif

/* REMAP_FILE_DEDUP 标志兼容性 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 20, 0)
// 新内核有这个标志
#include <linux/fs.h>
#else
// 老内核没有这个标志
#define REMAP_FILE_DEDUP 0
#endif

/* security_inode_init_security_anon 函数兼容性 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0) && defined(CONFIG_SECURITY)
// 新内核有这个函数
#include <linux/security.h>
#else
// 老内核，提供替代实现或返回错误
static inline int security_inode_init_security_anon(struct inode *inode,
                                                    const struct qstr *name,
                                                    const struct inode *context_inode)
{
    return 0; // 返回成功或使用老版本函数
}
#endif

/* alloc_file_pseudo 函数兼容性 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
// 新内核有这个函数
#include <linux/file.h>
#else
// 老内核，使用 alloc_file
static inline struct file *alloc_file_pseudo(struct inode *inode,
                                             struct vfsmount *mnt,
                                             const char *name,
                                             int flags,
                                             const struct file_operations *fops)
{
    struct path path = { .mnt = mnt, .dentry = dget(inode->i_sb->s_root) };
    return alloc_file(&path, flags, fops);
}
#endif

/* selinux_inode 函数兼容性 */
#if defined(CONFIG_SECURITY_SELINUX) && LINUX_VERSION_CODE >= KERNEL_VERSION(5, 0, 0)
// 新内核有这个函数
#include <linux/selinux.h>
#else
// 老内核，提供替代或返回 NULL
static inline struct inode_security_struct *selinux_inode(struct inode *inode)
{
#ifdef CONFIG_SECURITY_SELINUX
    return inode->i_security;
#else
    return NULL;
#endif
}
#endif

int ksu_install_file_wrapper(int fd);
void ksu_file_wrapper_init(void);

#endif // KSU_FILE_WRAPPER_H
