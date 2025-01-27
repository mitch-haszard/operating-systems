#include "oslabs.h"

int process_page_access_fifo(struct PTE page_table[TABLEMAX], int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp) {
    // check if in memory
    if (page_table[page_number].is_valid > 0) {
        // update and return
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count += 1;
        return page_table[page_number].frame_number;
    }
        // if frame pool isn't empty
    else if (*frame_cnt > 0) {
        // setup new page
        page_table[page_number].frame_number = frame_pool[*frame_cnt - 1];
        page_table[page_number].arrival_timestamp = current_timestamp;
        page_table[page_number].is_valid = 1;
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count = 1;

        // decrement frame pool
        *frame_cnt -= 1;

        return page_table[page_number].frame_number;
    }
        // if frame pool is empty
    else {
        // find smallest arrival time
        int smallest_at;
        int smallest_at_location;
        for (int i = 0; i < *table_cnt; i++) {
            if (page_table[i].is_valid > 0) {
                smallest_at = page_table[i].arrival_timestamp;
                smallest_at_location = i;
                break;
            }
        }
        for (int i = 0; i < *table_cnt; i++) {
            if (page_table[i].arrival_timestamp < smallest_at && page_table[i].is_valid > 0) {
                smallest_at = page_table[i].arrival_timestamp;
                smallest_at_location = i;
            }
        }
        // setup new page
        page_table[page_number].frame_number = page_table[smallest_at_location].frame_number;
        page_table[page_number].arrival_timestamp = current_timestamp;
        page_table[page_number].is_valid = 1;
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count = 1;

        // clear old page
        page_table[smallest_at_location].frame_number = -1;
        page_table[smallest_at_location].is_valid = 0;
        page_table[smallest_at_location].arrival_timestamp = 0;
        page_table[smallest_at_location].last_access_timestamp = 0;
        page_table[smallest_at_location].reference_count = 0;

        return page_table[page_number].frame_number;
    }
}

int count_page_faults_fifo(struct PTE page_table[TABLEMAX], int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt) {
    int timestamp = 0;
    int page_fault = 0;
    // iterate through reference strings
    for (int i = 0; i < reference_cnt; i++) {
        // increment time
        timestamp += 1;
        // check if reference_number in page table
        if (page_table[refrence_string[i]].is_valid > 0) {
            page_table[refrence_string[i]].reference_count += 1;
            page_table[refrence_string[i]].last_access_timestamp = timestamp;

        }
        // if page is not in memory but free frames exist
        else if (frame_cnt > 0) {
            page_table[refrence_string[i]].frame_number = frame_pool[frame_cnt - 1];
            page_table[refrence_string[i]].is_valid = 1;
            page_table[refrence_string[i]].arrival_timestamp = timestamp;
            page_table[refrence_string[i]].last_access_timestamp = timestamp;
            page_table[refrence_string[i]].reference_count += 1;
            frame_cnt -= 1;
            page_fault += 1;
        }
        // if no free frames exist
        else {
            // find smallest arrival time
            int smallest_at;
            int smallest_at_location;
            for (int j = 0; j < table_cnt; j++) {
                if (page_table[j].is_valid > 0) {
                    smallest_at = page_table[j].arrival_timestamp;
                    smallest_at_location = j;
                    break;
                }
            }
            for (int j = 0; j < table_cnt; j++) {
                if ((page_table[j].is_valid > 0) && (page_table[j].arrival_timestamp < smallest_at)) {
                    smallest_at = page_table[j].arrival_timestamp;
                    smallest_at_location = j;
                }
            }

            // setup new page
            page_table[refrence_string[i]].frame_number = page_table[smallest_at_location].frame_number;
            page_table[refrence_string[i]].arrival_timestamp = timestamp;
            page_table[refrence_string[i]].is_valid = 1;
            page_table[refrence_string[i]].last_access_timestamp = timestamp;
            page_table[refrence_string[i]].reference_count = 1;

            // clear old page
            page_table[smallest_at_location].frame_number = -1;
            page_table[smallest_at_location].is_valid = 0;
            page_table[smallest_at_location].arrival_timestamp = 0;
            page_table[smallest_at_location].last_access_timestamp = 0;
            page_table[smallest_at_location].reference_count = 0;

            page_fault += 1;
        }
    }

    return page_fault;
}

int process_page_access_lru(struct PTE page_table[TABLEMAX], int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp) {
    // check if in memory
    if (page_table[page_number].is_valid > 0) {
        // update and return
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count += 1;
        return page_table[page_number].frame_number;
    }
    // if frame pool isn't empty
    else if (*frame_cnt > 0) {
        // setup new page
        page_table[page_number].frame_number = frame_pool[*frame_cnt - 1];
        page_table[page_number].arrival_timestamp = current_timestamp;
        page_table[page_number].is_valid = 1;
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count = 1;

        // decrement frame pool
        *frame_cnt -= 1;

        return page_table[page_number].frame_number;
    }
    // if frame pool is empty
    else {
        // find smallest last arrival time
        int smallest_at;
        int smallest_at_location;
        for (int i = 0; i < *table_cnt; i++) {
            if (page_table[i].is_valid > 0) {
                smallest_at = page_table[i].last_access_timestamp;
                smallest_at_location = i;
                break;
            }
        }
        for (int i = 0; i < *table_cnt; i++) {
            if (page_table[i].last_access_timestamp < smallest_at && page_table[i].is_valid > 0) {
                smallest_at = page_table[i].last_access_timestamp;
                smallest_at_location = i;
            }
        }
        // setup new page
        page_table[page_number].frame_number = page_table[smallest_at_location].frame_number;
        page_table[page_number].arrival_timestamp = current_timestamp;
        page_table[page_number].is_valid = 1;
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count = 1;

        // clear old page
        page_table[smallest_at_location].frame_number = -1;
        page_table[smallest_at_location].is_valid = 0;
        page_table[smallest_at_location].arrival_timestamp = 0;
        page_table[smallest_at_location].last_access_timestamp = 0;
        page_table[smallest_at_location].reference_count = 0;

        return page_table[page_number].frame_number;
    }
}

int count_page_faults_lru(struct PTE page_table[TABLEMAX], int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt) {
    int timestamp = 0;
    int page_fault = 0;
    // iterate through reference strings
    for (int i = 0; i < reference_cnt; i++) {
        // increment time
        timestamp += 1;
        // check if reference_number in page table
        if (page_table[refrence_string[i]].is_valid > 0) {
            page_table[refrence_string[i]].reference_count += 1;
            page_table[refrence_string[i]].last_access_timestamp = timestamp;

        }
            // if page is not in memory but free frames exist
        else if (frame_cnt > 0) {
            page_table[refrence_string[i]].frame_number = frame_pool[frame_cnt - 1];
            page_table[refrence_string[i]].is_valid = 1;
            page_table[refrence_string[i]].arrival_timestamp = timestamp;
            page_table[refrence_string[i]].last_access_timestamp = timestamp;
            page_table[refrence_string[i]].reference_count += 1;
            frame_cnt -= 1;
            page_fault += 1;
        }
        // if no free frames exist
        else {
            // find smallest last access timestamp
            int smallest_at;
            int smallest_at_location;
            for (int j = 0; j < table_cnt; j++) {
                if (page_table[j].is_valid > 0) {
                    smallest_at = page_table[j].last_access_timestamp;
                    smallest_at_location = j;
                    break;
                }
            }
            for (int j = 0; j < table_cnt; j++) {
                if ((page_table[j].is_valid > 0) && (page_table[j].last_access_timestamp < smallest_at)) {
                    smallest_at = page_table[j].last_access_timestamp;
                    smallest_at_location = j;
                }
            }

            // setup new page
            page_table[refrence_string[i]].frame_number = page_table[smallest_at_location].frame_number;
            page_table[refrence_string[i]].arrival_timestamp = timestamp;
            page_table[refrence_string[i]].is_valid = 1;
            page_table[refrence_string[i]].last_access_timestamp = timestamp;
            page_table[refrence_string[i]].reference_count = 1;

            // clear old page
            page_table[smallest_at_location].frame_number = -1;
            page_table[smallest_at_location].is_valid = 0;
            page_table[smallest_at_location].arrival_timestamp = 0;
            page_table[smallest_at_location].last_access_timestamp = 0;
            page_table[smallest_at_location].reference_count = 0;

            page_fault += 1;
        }
    }

    return page_fault;
}

int process_page_access_lfu(struct PTE page_table[TABLEMAX], int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp) {
    // check if in memory
    if (page_table[page_number].is_valid > 0) {
        // update and return
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count += 1;
        return page_table[page_number].frame_number;
    }
        // if frame pool isn't empty
    else if (*frame_cnt > 0) {
        // setup new page
        page_table[page_number].frame_number = frame_pool[*frame_cnt - 1];
        page_table[page_number].arrival_timestamp = current_timestamp;
        page_table[page_number].is_valid = 1;
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count = 1;

        // decrement frame pool
        *frame_cnt -= 1;

        return page_table[page_number].frame_number;
    }
        // if frame pool is empty
    else {
        // find smallest reference count
        int smallest_at;
        int smallest_at_location;
        for (int i = 0; i < *table_cnt; i++) {
            if (page_table[i].is_valid > 0) {
                smallest_at = page_table[i].reference_count;
                smallest_at_location = i;
                break;
            }
        }
        for (int i = 0; i < *table_cnt; i++) {
            if (page_table[i].reference_count < smallest_at && page_table[i].is_valid > 0) {
                smallest_at = page_table[i].reference_count;
                smallest_at_location = i;
            }
        }
        // find count of smallest reference
        int number_of_smallest_at = 0;
        for (int i = 0; i < *table_cnt; i++) {
            if (page_table[i].is_valid > 0 && smallest_at == page_table[i].reference_count)
                number_of_smallest_at += 1;
        }
        // if count is more than 1
        if (number_of_smallest_at > 1) {
            int smallest_arrival_location;
            int smallest_arrival;
            for (int i = 0; i < *table_cnt; i++) {
                if (page_table[i].is_valid > 0 && smallest_at == page_table[i].reference_count) {
                    smallest_arrival = page_table[i].arrival_timestamp;
                    smallest_arrival_location = i;
                }
            }
            for (int i = 0; i < *table_cnt; i++) {
                if (page_table[i].is_valid > 0 && smallest_at == page_table[i].reference_count) {
                    if (page_table[i].arrival_timestamp < smallest_arrival) {
                        smallest_arrival = page_table[i].arrival_timestamp;
                        smallest_arrival_location = i;
                    }
                }
            }
            // setup new page
            page_table[page_number].frame_number = page_table[smallest_arrival_location].frame_number;
            page_table[page_number].arrival_timestamp = current_timestamp;
            page_table[page_number].is_valid = 1;
            page_table[page_number].last_access_timestamp = current_timestamp;
            page_table[page_number].reference_count = 1;

            // clear old page
            page_table[smallest_arrival_location].frame_number = -1;
            page_table[smallest_arrival_location].is_valid = 0;
            page_table[smallest_arrival_location].arrival_timestamp = 0;
            page_table[smallest_arrival_location].last_access_timestamp = 0;
            page_table[smallest_arrival_location].reference_count = 0;
        }
        else {
            // setup new page
            page_table[page_number].frame_number = page_table[smallest_at_location].frame_number;
            page_table[page_number].arrival_timestamp = current_timestamp;
            page_table[page_number].is_valid = 1;
            page_table[page_number].last_access_timestamp = current_timestamp;
            page_table[page_number].reference_count = 1;

            // clear old page
            page_table[smallest_at_location].frame_number = -1;
            page_table[smallest_at_location].is_valid = 0;
            page_table[smallest_at_location].arrival_timestamp = 0;
            page_table[smallest_at_location].last_access_timestamp = 0;
            page_table[smallest_at_location].reference_count = 0;
        }

        return page_table[page_number].frame_number;
    }
}

int count_page_faults_lfu(struct PTE page_table[TABLEMAX], int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt) {
    int timestamp = 0;
    int page_fault = 0;
    // iterate through reference strings
    for (int i = 0; i < reference_cnt; i++) {
        // increment time
        timestamp += 1;
        // check if reference_number in page table
        if (page_table[refrence_string[i]].is_valid > 0) {
            page_table[refrence_string[i]].reference_count += 1;
            page_table[refrence_string[i]].last_access_timestamp = timestamp;

        }
            // if page is not in memory but free frames exist
        else if (frame_cnt > 0) {
            page_table[refrence_string[i]].frame_number = frame_pool[frame_cnt - 1];
            page_table[refrence_string[i]].is_valid = 1;
            page_table[refrence_string[i]].arrival_timestamp = timestamp;
            page_table[refrence_string[i]].last_access_timestamp = timestamp;
            page_table[refrence_string[i]].reference_count += 1;
            frame_cnt -= 1;
            page_fault += 1;
        }
        // if no free frames exist
        else {
            int smallest_ref_count;
            // get a value for smallest_ref
            for (int j = 0; j < table_cnt; j++) {
                if (page_table[j].is_valid > 0)
                    smallest_ref_count = page_table[j].reference_count;
            }
            // find smallest reference count
            for (int j = 0; j < table_cnt; j++) {
                if (page_table[j].is_valid > 0 && page_table[j].reference_count < smallest_ref_count)
                    smallest_ref_count = page_table[j].reference_count;
            }
            // get value for smallest at
            int smallest_at;
            int smallest_at_location;
            for (int j = 0; j < table_cnt; j++) {
                if (page_table[j].is_valid > 0 && page_table[j].reference_count == smallest_ref_count) {
                    smallest_at = page_table[j].arrival_timestamp;
                    smallest_at_location = j;
                }
            }
            // find smallest_at
            for (int j = 0; j < table_cnt; j++) {
                if (page_table[j].is_valid > 0 && page_table[j].reference_count == smallest_ref_count
                && smallest_at > page_table[j].arrival_timestamp) {
                    smallest_at = page_table[j].arrival_timestamp;
                }
            }
            // find smallest_at_location
            for (int j = 0; j < table_cnt; j++) {
                if (page_table[j].is_valid > 0 && page_table[j].reference_count == smallest_ref_count
                    && smallest_at == page_table[j].arrival_timestamp)
                    smallest_at_location = j;
            }
            // setup new page
            page_table[refrence_string[i]].frame_number = page_table[smallest_at_location].frame_number;
            page_table[refrence_string[i]].arrival_timestamp = timestamp;
            page_table[refrence_string[i]].is_valid = 1;
            page_table[refrence_string[i]].last_access_timestamp = timestamp;
            page_table[refrence_string[i]].reference_count = 1;

            // clear old page
            page_table[smallest_at_location].frame_number = -1;
            page_table[smallest_at_location].is_valid = 0;
            page_table[smallest_at_location].arrival_timestamp = 0;
            page_table[smallest_at_location].last_access_timestamp = 0;
            page_table[smallest_at_location].reference_count = 0;

            page_fault += 1;
        }
    }
    return page_fault;
}
