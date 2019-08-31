/*************************************************
 * Anthor  : LuoZhongYao@gmail.com
 * Modified: 2019/06/10
 ************************************************/
#ifndef __TASKLET_H__
#define __TASKLET_H__

#include <stdint.h>

struct tasklet
{
	unsigned	flags:8;
	unsigned	priority:8;
	struct tasklet *next;
	void (*destroy)(struct tasklet *task);
	int (*handler)(struct tasklet *task) __attribute__((nonnull));
};

#define TASKLET_INIT(let, prior) do {\
	(let)->next = NULL;\
	(let)->priority = prior;\
	(let)->flags = 0;\
} while(0)

void tasklet_sched(void);
int tasklet_create(struct tasklet *task, int priority);
void tasklet_kill(struct tasklet *task);
void tasklet_suspend(struct tasklet *task);
void tasklet_resume(struct tasklet *task);

#endif /* __TASKLET_H__*/

