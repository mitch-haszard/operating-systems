#include "oslabs.h"
#include <stdlib.h>

// helper function to create null rcb
struct RCB create_null_rcb () {
    struct RCB null_rcb = {0, 0, 0, 0, 0};
    return null_rcb;
}

struct RCB handle_request_arrival_fcfs(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp) {
    // if current request is null
    if (current_request.arrival_timestamp == 0 && current_request.process_id == 0 && current_request.request_id == 0
            && current_request.cylinder == 0 && current_request.address == 0) {
        return new_request;
    }
    else {
        request_queue[*queue_cnt] = new_request;
        *queue_cnt += 1;
        return current_request;
    }
}

struct RCB handle_request_completion_fcfs(struct RCB request_queue[QUEUEMAX],int *queue_cnt) {
    // if request queue is empty
    if (*queue_cnt == 0)
        return create_null_rcb();
    else {
        // find earliest arrival time position
        int earliest_counter = 0;
        int earliest_arr_time = request_queue[0].arrival_timestamp;
        for (int i = 0; i < *queue_cnt; i++) {
            if (request_queue[i].arrival_timestamp < earliest_arr_time) {
                earliest_arr_time = request_queue[i].arrival_timestamp;
                earliest_counter = i;
            }
        }
        // create rcb to be returned
        struct RCB temp_rcb = request_queue[earliest_counter];
        // remove from queue
        for (int i = earliest_counter; i < *queue_cnt; i++) {
            request_queue[i] = request_queue[i + 1];
        }
        *queue_cnt -= 1;

        return temp_rcb;
    }
}

struct RCB handle_request_arrival_sstf(struct RCB request_queue[QUEUEMAX],int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp) {
    // if current request is null
    if (current_request.arrival_timestamp == 0 && current_request.process_id == 0 && current_request.request_id == 0
        && current_request.cylinder == 0 && current_request.address == 0) {
        return new_request;
    }
    else {
        request_queue[*queue_cnt] = new_request;
        *queue_cnt += 1;
        return current_request;
    }
}

struct RCB handle_request_completion_sstf(struct RCB request_queue[QUEUEMAX],int *queue_cnt,int current_cylinder) {
    // if request queue is empty
    if (*queue_cnt == 0)
        return create_null_rcb();
    else {
        // find nearest cylinder
        int nearest_cylinder = 0;
        int cylinder_location = 0;
        int cylinder_count[*queue_cnt];
        int curr_cylinder = 0;

        for(int i = 0; i < *queue_cnt; i++) {
            nearest_cylinder = abs(current_cylinder - request_queue[i].cylinder);
            cylinder_location = i;
            break;
        }

        for(int i = 0; i < *queue_cnt; i++) {
            if(abs(current_cylinder - request_queue[i].cylinder) < nearest_cylinder) {
                nearest_cylinder = abs(current_cylinder - request_queue[i].cylinder);
                cylinder_location = i;
            }
        }

        for(int i = 0; i < *queue_cnt; i++) {
            if(abs(current_cylinder - request_queue[i].cylinder) == nearest_cylinder) {
                cylinder_count[curr_cylinder] = i;
                curr_cylinder += 1;
            }
        }

        // if more than one cylinder
        if(curr_cylinder > 1) {
            int smallest_arr = request_queue[cylinder_count[0]].arrival_timestamp;
            int smallest_arr_location = cylinder_count[0];
            for(int i = 0; i < curr_cylinder; i++) {
                if(request_queue[cylinder_count[i]].arrival_timestamp < smallest_arr) {
                    smallest_arr = request_queue[cylinder_count[i]].arrival_timestamp;
                    smallest_arr_location = cylinder_count[i];
                }
            }
            struct RCB temp_rcb = request_queue[smallest_arr_location];
            for(int i = smallest_arr_location; i < *queue_cnt; i++) {
                request_queue[i] = request_queue[i + 1];
            }
            *queue_cnt -= 1;
            return temp_rcb;
        }
        else {
            struct RCB temp_rcb = request_queue[cylinder_location];
            for(int i = cylinder_location; i < *queue_cnt; i++) {
                request_queue[i] = request_queue[i + 1];
            }
            *queue_cnt -= 1;
            return temp_rcb;
        }
    }
}

struct RCB handle_request_arrival_look(struct RCB request_queue[QUEUEMAX],int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp) {
    if (current_request.arrival_timestamp == 0 && current_request.process_id == 0 && current_request.request_id == 0
        && current_request.cylinder == 0 && current_request.address == 0) {
        return new_request;
    }
    else {
        request_queue[*queue_cnt] = new_request;
        *queue_cnt += 1;
        return current_request;
    }
}

struct RCB handle_request_completion_look(struct RCB request_queue[QUEUEMAX],int *queue_cnt, int current_cylinder, int scan_direction) {
    // if request queue is empty
    if (*queue_cnt == 0)
        return create_null_rcb();
    else {
        int same_cylinder_exists = 0;
        for (int i = 0; i < *queue_cnt; i++) {
            if(request_queue[i].cylinder == current_cylinder) {
                same_cylinder_exists = 1;
                break;
            }
        }
        if (same_cylinder_exists != 0) {
            int same_cylinder[*queue_cnt];
            int same_cylinder_count = 0;
            for(int i = 0; i < *queue_cnt; i++) {
                if(request_queue[i].cylinder == current_cylinder) {
                    same_cylinder[same_cylinder_count] = i;
                    same_cylinder_count += 1;
                }
            }
            if (same_cylinder_count > 1) {
                int earliest_arr = request_queue[same_cylinder[0]].arrival_timestamp;
                int earliest_arr_index = same_cylinder[0];
                for (int i = 0; i < same_cylinder_count; i++) {
                    if (request_queue[same_cylinder[i]].arrival_timestamp < earliest_arr) {
                        earliest_arr = request_queue[same_cylinder[i]].arrival_timestamp;
                        earliest_arr_index = same_cylinder[i];
                    }
                }
                struct RCB return_RCB = request_queue[earliest_arr_index];
                for (int i = earliest_arr_index; i < *queue_cnt; i++) {
                    request_queue[i] = request_queue[i + 1];
                }
                *queue_cnt -= 1;
                return return_RCB;
            }
            else {
                struct RCB return_RCB = request_queue[same_cylinder[0]];
                for (int i = same_cylinder[0]; i < *queue_cnt; i++) {
                    request_queue[i] = request_queue[i + 1];
                }
                *queue_cnt -= 1;
                return return_RCB;
            }
        }
        else {
            // if scan direction is 1
            if (scan_direction == 1) {
                int larger_cylinder = 0;
                for (int i = 0; i < *queue_cnt; i++) {
                    if(request_queue[i].cylinder > current_cylinder) {
                        larger_cylinder = 1;
                        break;
                    }
                }
                // if there is a larger cylinder
                if (larger_cylinder > 0) {
                    int larger_cylinder_index[*queue_cnt];
                    int larger_cylinder_index_count = 0;
                    for (int i = 0; i < *queue_cnt; i++) {
                        if (request_queue[i].cylinder > current_cylinder) {
                            larger_cylinder_index[larger_cylinder_index_count] = i;
                            larger_cylinder_index_count += 1;
                        }
                    }
                    // if larger cylinder is greater than one
                    if (larger_cylinder_index_count > 1) {
                        int closest_cylinder = abs(request_queue[larger_cylinder_index[0]].cylinder - current_cylinder);
                        int closest_cylinder_index = larger_cylinder_index[0];
                        for (int i = 0; i < larger_cylinder_index_count; i++) {
                            if (abs(request_queue[larger_cylinder_index[i]].cylinder - current_cylinder) < closest_cylinder) {
                                closest_cylinder = abs(request_queue[larger_cylinder_index[i]].cylinder - current_cylinder);
                                closest_cylinder_index = larger_cylinder_index[i];
                            }
                        }
                        struct RCB return_RCB = request_queue[closest_cylinder_index];
                        for (int i = closest_cylinder_index; i < *queue_cnt; i++) {
                            request_queue[i] = request_queue[i + 1];
                        }
                        *queue_cnt -= 1;
                        return return_RCB;
                    }
                    else {
                        struct RCB return_RCB = request_queue[larger_cylinder_index[0]];
                        for(int i = larger_cylinder_index[0]; i < *queue_cnt; i++) {
                            request_queue[i] = request_queue[i + 1];
                        }
                        *queue_cnt -= 1;
                        return return_RCB;
                    }
                }
                else {
                    int closest_cylinder = abs(current_cylinder - request_queue[0].cylinder);
                    int closest_cylinder_index = 0;
                    for(int i = 0; i < *queue_cnt; i++) {
                        if(abs(current_cylinder - request_queue[i].cylinder) < closest_cylinder) {
                            closest_cylinder = abs(current_cylinder - request_queue[i].cylinder);
                            closest_cylinder_index = i;
                        }
                    }
                    struct RCB return_RCB = request_queue[closest_cylinder_index];
                    for (int i = closest_cylinder_index; i < *queue_cnt; i++) {
                        request_queue[i] = request_queue[i + 1];
                    }
                    *queue_cnt -= 1;
                    return return_RCB;
                }
            }
            // if scan direction equals 0
            else if (scan_direction == 0) {
                int smaller_cylinder_exists = 0;
                for (int i = 0; i < *queue_cnt; i++) {
                    if (request_queue[i].cylinder < current_cylinder) {
                        smaller_cylinder_exists = 1;
                        break;
                    }
                }
                // if a smaller cylinder exists
                if (smaller_cylinder_exists == 1) {
                    int smaller_cylinder_index[*queue_cnt];
                    int curr_smaller_cylinder_index_count = 0;
                    for (int i = 0; i < *queue_cnt; i++) {
                        if (request_queue[i].cylinder < current_cylinder) {
                            smaller_cylinder_index[curr_smaller_cylinder_index_count] = i;
                            curr_smaller_cylinder_index_count += 1;
                        }
                    }
                    // if smaller is greater than one
                    if (curr_smaller_cylinder_index_count > 1) {
                        int closest_cylinder = abs(request_queue[smaller_cylinder_index[0]].cylinder - current_cylinder);
                        int closest_cylinder_index = smaller_cylinder_index[0];
                        for (int i = 0; i < curr_smaller_cylinder_index_count; i++) {
                            if(abs(request_queue[smaller_cylinder_index[i]].cylinder - current_cylinder) < closest_cylinder) {
                                closest_cylinder = abs(request_queue[smaller_cylinder_index[i]].cylinder - current_cylinder);
                                closest_cylinder_index = smaller_cylinder_index[i];
                            }
                        }
                        struct RCB return_RCB = request_queue[closest_cylinder_index];
                        for (int i = closest_cylinder_index; i < *queue_cnt; i++) {
                            request_queue[i] = request_queue[i + 1];
                        }
                        *queue_cnt -= 1;
                        return return_RCB;
                    }
                    else {
                        struct RCB return_RCB = request_queue[smaller_cylinder_index[0]];
                        for (int i = smaller_cylinder_index[0]; i < *queue_cnt; i++) {
                            request_queue[i] = request_queue[i + 1];
                        }
                        *queue_cnt -= 1;
                        return return_RCB;
                    }
                }
                else {
                    int larger_cylinder_index[*queue_cnt];
                    int curr_larger_cylinder_index_count = 0;
                    for (int i = 0; i < *queue_cnt; i++) {
                        if(request_queue[i].cylinder > current_cylinder) {
                            larger_cylinder_index[curr_larger_cylinder_index_count] = i;
                            curr_larger_cylinder_index_count =+ 1;
                        }
                    }
                    // if count is greater than one
                    if (curr_larger_cylinder_index_count > 1) {
                        int closest_cylinder = abs(current_cylinder - request_queue[larger_cylinder_index[0]].cylinder);
                        int closest_cylinder_index = larger_cylinder_index[0];
                        for (int i = 0; i < curr_larger_cylinder_index_count; i++) {
                            if (abs(current_cylinder - request_queue[larger_cylinder_index[i]].cylinder) < closest_cylinder) {
                                closest_cylinder = abs(current_cylinder - request_queue[larger_cylinder_index[i]].cylinder);
                                closest_cylinder_index = larger_cylinder_index[i];
                            }
                        }
                        struct RCB return_RCB = request_queue[closest_cylinder_index];
                        for (int i = closest_cylinder_index; i < *queue_cnt; i++) {
                            request_queue[i] = request_queue[i + 1];
                        }
                        *queue_cnt -= 1;
                        return return_RCB;
                    }
                    else {
                        struct RCB return_RCB = request_queue[larger_cylinder_index[0]];
                        for (int i = larger_cylinder_index[0]; i < *queue_cnt; i++) {
                            request_queue[i] = request_queue[i + 1];
                        }
                        queue_cnt -= 1;
                        return return_RCB;
                    }
                }
            }
        }
    }
}
