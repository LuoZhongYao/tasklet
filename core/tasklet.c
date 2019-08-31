#include "defs.h"
#include "tasklet.h"

enum {
	TASKLET_DESTROY = 0,
	TASKLET_READY,
	TASKLET_UNREADY,
};

struct tasklet_ready
{
	struct tasklet *head, *tail;
};

static struct tasklet_ready tsklet_ready[3];

static void tasklet_ready(struct tasklet *let)
{
	struct tasklet_ready *rdy = tsklet_ready + let->priority;
	if (let->flags == TASKLET_READY)
		return ;

	if (rdy->head == NULL) {
		rdy->head = let;
		rdy->tail = let;
	} else {
		rdy->tail->next = let;
		rdy->tail = let;
	}
	let->flags = TASKLET_READY;
}

static void tasklet_unready(struct tasklet *let)
{
    struct tasklet **xt;
	struct tasklet_ready *rdy = tsklet_ready + let->priority;

	if (let->flags != TASKLET_READY)
		return ;

	xt = &rdy->head;
	while (*xt) {
		if (*xt == let) {
			*xt = let->next;
			let->flags = TASKLET_UNREADY;
			return ;
		} else {
			xt = &(*xt)->next;
		}
	}
}

static void tasklet_destroy(struct tasklet *let)
{
	if (let->flags == TASKLET_DESTROY)
		return ;

	if (let->flags == TASKLET_READY)
		tasklet_unready(let);

	let->flags = TASKLET_DESTROY;
	if (let->destroy)
		let->destroy(let);
}

static struct tasklet *tasklet_pick(void)
{
	int i = 0;
	for (i = 0;i < ARRAY_SIZE(tsklet_ready);i++) {
		if (tsklet_ready[i].head) {
			struct tasklet *let = tsklet_ready[i].head;
			tsklet_ready[i].head = let->next;
			tasklet_ready(let);
			return let;
		}
	}

	return NULL;
}

void tasklet_sched(void)
{
	struct tasklet *let;

	if ((let = tasklet_pick())) {
		if (let->handler(let)) {
			tasklet_kill(let);
		}
	}
}

int tasklet_create(struct tasklet *let, int priority)
{
	if (priority < 0 || priority >= ARRAY_SIZE(tsklet_ready))
		return -1;

	let->next = NULL;
	let->priority = priority;
	let->flags = TASKLET_READY;

	tasklet_ready(let);

	return 0;
}

void tasklet_kill(struct tasklet *let)
{
	if (let->flags != TASKLET_READY && let->flags != TASKLET_UNREADY)
		return ;

	tasklet_destroy(let);
}

void tasklet_suspend(struct tasklet *let)
{
	tasklet_unready(let);
}

void tasklet_resume(struct tasklet *let)
{
	tasklet_ready(let);
}
