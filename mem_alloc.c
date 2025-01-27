#include "oslabs.h"

struct MEMORY_BLOCK create_null_block() {
    // helper function to return null block
    struct MEMORY_BLOCK null_block = {0,0,0,0};
    return  null_block;
}

struct MEMORY_BLOCK best_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id) {
    // iterate through memory map to find best fit
    int best_fit = -1;
    for (int i = 0; i < *map_cnt; i++) {
        if (memory_map[i].segment_size >= request_size && memory_map[i].process_id == 0) {
            if (best_fit == -1)
                best_fit = i;
            else if (memory_map[best_fit].segment_size > memory_map[i].segment_size)
                best_fit = i;
        }
    }

    struct MEMORY_BLOCK new_block = {memory_map[best_fit].start_address, (memory_map[best_fit].start_address + request_size - 1),
                                     request_size, process_id};

    if (best_fit == -1)
        // if no room in memory map
        return create_null_block();
    else if (best_fit == (*map_cnt - 1)) {
        // if block is at end of array
        if ((memory_map[best_fit].start_address + request_size) == memory_map[best_fit].end_address) {
            // if best fit equals end of memory map

            memory_map[best_fit] = new_block;
            return new_block;
        }
        else {
            // if best fit is less than the end of the memory map
            struct MEMORY_BLOCK free_memory_block = {new_block.end_address + 1, memory_map[best_fit].end_address,
                    memory_map[best_fit].end_address - new_block.end_address, 0};

            memory_map[best_fit] = new_block;
            memory_map[best_fit + 1] = free_memory_block;
            *map_cnt += 1;
            return new_block;

        }
    }
    else {
        // if block is in middle of array
        for (int i = *map_cnt - 1; i > best_fit; i--) {
            memory_map[i] = memory_map[i - 1];
        }

        struct MEMORY_BLOCK free_memory_block = {new_block.end_address + 1, memory_map[best_fit].end_address,
                memory_map[best_fit].end_address - new_block.end_address, 0};

        memory_map[best_fit] = new_block;
        memory_map[best_fit + 1] = free_memory_block;
        *map_cnt += 1;
        return new_block;
    }
}

struct MEMORY_BLOCK first_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id) {
    // iterate through memory map to find first fit
    int first_fit = -1;
    for (int i = 0; i < *map_cnt; i++) {
        if (memory_map[i].segment_size >= request_size && memory_map[i].process_id == 0) {
            first_fit = i;
            break;
        }
    }

    struct MEMORY_BLOCK new_block = {memory_map[first_fit].start_address, (memory_map[first_fit].start_address + request_size - 1),
                                     request_size, process_id};

    if (first_fit == -1)
        // if no room in memory map
        return create_null_block();
    else {
        // if block is in middle of array
        for (int i = *map_cnt - 1; i > first_fit; i--) {
            memory_map[i] = memory_map[i - 1];
        }

        struct MEMORY_BLOCK free_memory_block = {new_block.end_address + 1, memory_map[first_fit].end_address,
                                                 memory_map[first_fit].end_address - new_block.end_address, 0};

        memory_map[first_fit] = new_block;
        memory_map[first_fit + 1] = free_memory_block;
        *map_cnt += 1;
        return new_block;
    }

}
struct MEMORY_BLOCK worst_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id) {
    // iterate through memory map to find worst fit
    int worst_fit = -1;
    int max_size = memory_map[0].segment_size;
    if (max_size >= request_size) {
        worst_fit = 0;
    }
        for (int i = 0; i < *map_cnt; i++) {
        if (memory_map[i].segment_size > max_size && memory_map[i].process_id == 0) {
            worst_fit = i;
            max_size = memory_map[i].segment_size;
        }
    }

    struct MEMORY_BLOCK new_block = {memory_map[worst_fit].start_address, (memory_map[worst_fit].start_address + request_size - 1),
                                     request_size, process_id};

    if (max_size >= request_size) {
        // if block is in middle of array
        for (int i = *map_cnt - 1; i > worst_fit; i--) {
            memory_map[i] = memory_map[i - 1];
        }

        struct MEMORY_BLOCK free_memory_block = {new_block.end_address + 1, memory_map[worst_fit].end_address,
                                                 memory_map[worst_fit].end_address - new_block.end_address, 0};

        memory_map[worst_fit] = new_block;
        memory_map[worst_fit + 1] = free_memory_block;
        *map_cnt += 1;
        return new_block;
    }
    else
        return create_null_block();

}

struct MEMORY_BLOCK next_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX],int *map_cnt, int process_id, int last_address) {
    // iterate through memory map to find next fit
    int next_fit = -1;
    for (int i = last_address; i < *map_cnt; i++) {
        if (memory_map[i].segment_size >= request_size && memory_map[i].process_id == 0) {
            next_fit = i;
            break;
        }
    }

    struct MEMORY_BLOCK new_block = {memory_map[next_fit].start_address, (memory_map[next_fit].start_address + request_size - 1),
                                     request_size, process_id};

    if (next_fit == -1)
        // if no room in memory map
        return create_null_block();
    else {
        // if block is in middle of array
        for (int i = *map_cnt - 1; i > next_fit; i--) {
            memory_map[i] = memory_map[i - 1];
        }

        struct MEMORY_BLOCK free_memory_block = {new_block.end_address + 1, memory_map[next_fit].end_address,
                                                 memory_map[next_fit].end_address - new_block.end_address, 0};

        memory_map[next_fit] = new_block;
        memory_map[next_fit + 1] = free_memory_block;
        *map_cnt += 1;
        return new_block;
    }
}

void release_memory(struct MEMORY_BLOCK freed_block, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt) {
    // iterate through memory map to find free block
    int free_block = -1;
    for (int i = 0; i < *map_cnt; i++) {
        if (memory_map[i].start_address == freed_block.start_address)
            free_block = i;
    }

    // If memory is free
    if (memory_map[free_block + 1].process_id == 0) {
        memory_map[free_block - 1].end_address = memory_map[free_block + 1].end_address;
        memory_map[free_block - 1].segment_size = (memory_map[free_block - 1].end_address - memory_map[free_block - 1].start_address) + 1;

        for (int i = free_block; i < *map_cnt; i++)
            memory_map[i] = memory_map[i + 2];

        *map_cnt -= 2;
    }
    // if memory is not free
    else {
        memory_map[free_block - 1].end_address = memory_map[free_block].end_address;
        memory_map[free_block - 1].segment_size = (memory_map[free_block - 1].end_address - memory_map[free_block - 1].start_address) + 1;

        for (int i = free_block; i < *map_cnt; i++)
            memory_map[i] = memory_map[i + 1];

        *map_cnt -= 1;
    }
}
