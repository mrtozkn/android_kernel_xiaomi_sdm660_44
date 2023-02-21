#include <linux/export.h>
#include <linux/bug.h>
#include <linux/uaccess.h>
#include <linux/nospec.h>

void copy_from_user_overflow(void)
{
	unsigned long res = n;
	might_fault();
	if (likely(access_ok(VERIFY_READ, from, n))) {
		/*
		 * Ensure that bad access_ok() speculation will not
		 * lead to nasty side effects *after* the copy is
		 * finished:
		 */
		barrier_nospec();
		kasan_check_write(to, n);
		res = raw_copy_from_user(to, from, n);
	}
	if (unlikely(res))
		memset(to + (n - res), 0, res);
	return res;
}
EXPORT_SYMBOL(copy_from_user_overflow);
