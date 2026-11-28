#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

// names this files log source 'pipeline', shows msgs at info level and up (leaves out debugging)
LOG_MODULE_REGISTER(pipeline, LOG_LEVEL_INF);


/* a single button event carries which button was pressed and when it was pressed */
struct button_event {
    int button_id;
    int64_t timestamp_ms;
};


/* queue of max 10 events, 4 = memory alignment */
K_MSGQ_DEFINE(button_queue, sizeof(struct button_event), 10, 4);

void button_timer_handler(struct k_timer *timer) {
    ARG_UNUSED(timer);

    static int next_button = 1;             // static, keeps it's value between calls.

    struct button_event event;
    event.button_id = next_button;
    event.timestamp_ms = k_uptime_get();

    // do this immediately if possible, the timer must not block so drop the even and warn if the queue is full
    if (k_msgq_put(&button_queue, &event, K_NO_WAIT) != 0) {
        LOG_WRN("queue full, event dropped");
    }

    next_button = (next_button % 3) + 1;
}

K_TIMER_DEFINE(button_timer, button_timer_handler, NULL);

/* the consumer sleeps until an event wakes it. said event is logged. p1 to p3 are unused,
    but the zephyr thread signature requires them
*/
void consumer_thread(void *p1, void *p2, void *p3) {
    // this function doesn't use anything from the outside and I don't like seeing compiler warnings
    ARG_UNUSED(p1);
    ARG_UNUSED(p2);
    ARG_UNUSED(p3);

    struct button_event event;

    while (true) {
        // *x = go the the address of x. &x = get me the address of x
        if (k_msgq_get(&button_queue, &event, K_FOREVER) == 0) {
            LOG_INF("button %d pressed at %lld ms", event.button_id, (long long)event.timestamp_ms);
        }
    }
}

K_THREAD_DEFINE(consumer_thread_id, 1024, consumer_thread, NULL, NULL, NULL, 5, 0, 0);

void heartbeat_thread(void *p1, void *p2, void *p3) {
    ARG_UNUSED(p1);
    ARG_UNUSED(p2);
    ARG_UNUSED(p3);

    while (true) {
        LOG_INF("heartbeat at %lld ms", (long long)k_uptime_get());
        k_msleep(500);
    }
}

K_THREAD_DEFINE(heartbeat_thread_id, 1024, heartbeat_thread, NULL, NULL, NULL, 5, 0, 0);

int main(void) {
    LOG_INF("pipeline starting");
    k_timer_start(&button_timer, K_MSEC(700), K_MSEC(700));

    // we can return here because the threads and the timer keep running
    return 0;
}
