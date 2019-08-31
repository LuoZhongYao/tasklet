#include <stddef.h>
#include <tasklet.h>

struct tasklet let1;
struct tasklet let2;

static int let1_handler(struct tasklet *tsk)
{
	tasklet_resume(&let2);
	return 0;
}

static int let2_handler(struct tasklet *tsk)
{
	tasklet_suspend(&let1);
	return 1;
}

int main(void)
{
	TASKLET_INIT(&let1, 0);
	TASKLET_INIT(&let2, 0);
	let1.handler = let1_handler;
	let2.handler = let2_handler;

	tasklet_resume(&let1);
	while (1) { tasklet_sched(); }

	return 0;
}
