#include "oslabs.h"

void run_new_process(struct PCB* n_process, int time) {
    // helper function that sets up new process
    n_process->execution_starttime = time;
    n_process->execution_endtime = time + n_process->total_bursttime;
    n_process->remaining_bursttime = n_process->total_bursttime;
}

void load_new_process(struct PCB* n_process, int time) {
    // helper function to load new process into queue
    n_process->execution_starttime = 0;
    n_process->execution_endtime = 0;
    n_process->remaining_bursttime = n_process->total_bursttime;
}

struct PCB create_null_pcb() {
    // helper function that creates null pcb
    struct PCB null_pcb;
    null_pcb.process_id = 0;
    null_pcb.arrival_timestamp = 0;
    null_pcb.total_bursttime = 0;
    null_pcb.remaining_bursttime = 0;
    null_pcb.process_priority = 0;
    null_pcb.execution_starttime = 0;
    null_pcb.execution_endtime = 0;

    return null_pcb;
}

void queue_cleanup(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int counter) {
    // helper function for queue cleanup
    for (int i = counter; i < *queue_cnt; i++) {
    ready_queue[i] = ready_queue[i + 1];
    }
    *queue_cnt -= 1;
}

struct PCB handle_process_arrival_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp) {
    // check if current process is null
    if (current_process.process_priority == 0  && current_process.process_id == 0 && current_process.remaining_bursttime == 0
    && current_process.arrival_timestamp == 0  && current_process.execution_starttime == 0  && current_process.total_bursttime == 0
    && current_process.execution_endtime == 0) {
        // calculate new process execution time
        run_new_process(&new_process, timestamp);

        return new_process;
    }
    // if new process priority is equal to or less than current process
    else if (new_process.process_priority >= current_process.process_priority) {
        load_new_process(&new_process, timestamp);
        *queue_cnt += 1;
        ready_queue[*queue_cnt] = new_process;

        return current_process;
    }
    // if new process priority is greater
    else {
        // calculate execution time
        run_new_process(&new_process, timestamp);

        current_process.execution_endtime = 0;
        current_process.remaining_bursttime = current_process.total_bursttime;
        ready_queue[*queue_cnt] = current_process;
        *queue_cnt += 1;

        return new_process;
        }
}


struct PCB handle_process_completion_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp) {
    // if ready queue is empty
    if (*queue_cnt == 0)
        return create_null_pcb();
    else {
        // Find highest priority
        struct PCB next_in_queue = ready_queue[0];
        int counter = 0;
        for (int i = 0; i < *queue_cnt; i++) {
            if (ready_queue[i].process_priority < next_in_queue.process_priority) {
                next_in_queue = ready_queue[i];
                counter = i;
            }
        }
        // clean up queue
        queue_cleanup(ready_queue, queue_cnt, counter);

        // return next pcb
        next_in_queue.execution_starttime = timestamp;
        next_in_queue.execution_endtime = timestamp + next_in_queue.remaining_bursttime;

        return next_in_queue;
    }
}

struct PCB handle_process_arrival_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int time_stamp) {
    // check if current process is null
    if (current_process.process_priority == 0  && current_process.process_id == 0 && current_process.remaining_bursttime == 0
        && current_process.arrival_timestamp == 0  && current_process.execution_starttime == 0  && current_process.total_bursttime == 0
        && current_process.execution_endtime == 0) {
        // calculate new process execution time
        run_new_process(&new_process, time_stamp);

        return new_process;
    }
    // if new process has more burst time
    else if (current_process.remaining_bursttime < new_process.total_bursttime) {
        load_new_process(&new_process, time_stamp);
        ready_queue[*queue_cnt] = new_process;
        *queue_cnt += 1;

        return current_process;
    }
    // if new process has less burst time
    else {
        // calculate execution time
        run_new_process(&new_process, time_stamp);

        current_process.execution_endtime = 0;
        current_process.execution_starttime = 0;
        current_process.remaining_bursttime = current_process.total_bursttime;
        ready_queue[*queue_cnt] = current_process;
        *queue_cnt += 1;

        return new_process;
    }
}

struct PCB handle_process_completion_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp) {
    // if ready queue is empty
    if (*queue_cnt == 0)
        return create_null_pcb();
    else {
        // find shortest remaining burst time
        struct PCB next_in_queue = ready_queue[0];
        int counter = 0;
        for (int i = 0; i < *queue_cnt; i++) {
            if (next_in_queue.remaining_bursttime > ready_queue[i].remaining_bursttime) {
                next_in_queue = ready_queue[i];
                counter = i;
            }
        }
        // clean up queue
        queue_cleanup(ready_queue, queue_cnt, counter);

        // return next pcb
        next_in_queue.execution_starttime = timestamp;
        next_in_queue.execution_endtime = timestamp + next_in_queue.remaining_bursttime;

        return next_in_queue;
    }
}

struct PCB handle_process_arrival_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp, int time_quantum) {
    // check if current process is null
    if (current_process.process_priority == 0  && current_process.process_id == 0 && current_process.remaining_bursttime == 0
        && current_process.arrival_timestamp == 0  && current_process.execution_starttime == 0  && current_process.total_bursttime == 0
        && current_process.execution_endtime == 0) {
        // calculate new process execution time
        new_process.execution_starttime = timestamp;
        if (time_quantum > new_process.total_bursttime)
            new_process.execution_endtime = timestamp + new_process.total_bursttime;
        else
            new_process.execution_endtime = timestamp + time_quantum;

        new_process.remaining_bursttime = new_process.total_bursttime;

        return new_process;
    }
    else {
        load_new_process(&new_process, timestamp);
        ready_queue[*queue_cnt] = new_process;
        *queue_cnt += 1;

        return current_process;
    }
}

struct PCB handle_process_completion_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp, int time_quantum) {
    // if ready queue is empty
    if (*queue_cnt == 0)
        return create_null_pcb();
    else {
        struct PCB next_in_queue = ready_queue[0];
        int counter = 0;
        for (int i = 0; i < *queue_cnt; i++) {
            if (next_in_queue.arrival_timestamp > ready_queue[i].arrival_timestamp) {
                next_in_queue = ready_queue[i];
                counter = i;
            }
        }
        // clean up queue
        queue_cleanup(ready_queue, queue_cnt, counter);

        // return next pcb
        next_in_queue.execution_starttime = timestamp;
        if (time_quantum > next_in_queue.total_bursttime)
            next_in_queue.execution_endtime = timestamp + next_in_queue.total_bursttime;
        else
            next_in_queue.execution_endtime = timestamp + time_quantum;

        return next_in_queue;
    }
}
