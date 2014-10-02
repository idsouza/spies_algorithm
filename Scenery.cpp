#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "Scenery.h"

Scenery::Scenery() {
    this->variations = 0;
}

void Scenery::init_block() {    
    for (int i = 0; i < BOOTH_AMOUNT; i++) {
        *(this->block_m + i) = 0;
    }
}

void Scenery::set_agent(const int agent, const int index) {
    *(this->block_m + index) = agent;
}

bool Scenery::is_null(int index) {
    return *(this->block_m + index) == 0;
}

void Scenery::set_block(int* booth) {
    this->block_m = booth;
}

int* Scenery::malloc_block() {
    return (int*) malloc(sizeof(int) * BOOTH_AMOUNT);
}

int* Scenery::cp_pointer(int *p1) {
    int *p2 = malloc_block();
    for (int i = 0; i < BOOTH_AMOUNT; i++) {
        *(p2 + i) = *(p1 + i);
    }
    return p2;
}

int Scenery::one_condition(int i) {
    int cr = i + 1, cl = i - 1, index = -1;
    
    if (i < BOOTH_AMOUNT) {
        if ((i == 0) && is_null(i) && is_null(cr)) {
            index = i;
        } else if (cr == BOOTH_AMOUNT) {
            if(is_null(cl) && is_null(i)) {
                index = i;
            }
        } else if (is_null(cl) && is_null(i) && is_null(cr)) {
            index = i;        
        } else {
            index = one_condition(i + 1);
        }
    }
    
    return index;    
}

int Scenery::two_condition(int i) {
    int cr = i + 1, cl = i - 1, index = -1;
    
    if (i < BOOTH_AMOUNT) {
        if (cr == (BOOTH_AMOUNT)) {
            if (is_null(i)) {
                index = i;
            }
        } else if ((i == 0) && is_null(i)) {
            index = i;
        } else if (is_null(i) && (is_null(cl) || (is_null(cr)))) {
            index = i;
        } else { 
            index = two_condition(i + 1);
        }
    }
    
    return index;
}

int Scenery::three_condition(int i) {
    int index = -1;
    
    if (i < BOOTH_AMOUNT) {
        if (i == (BOOTH_AMOUNT - 1)) {
            if (is_null(i)) {
                index = i;
            }
        } else if (is_null(i)) {
            index = i;
        } else {
            index = three_condition(i + 1);
        }
    }
    
    return index;
}

void Scenery::remove_agents_disable() {
    for (int i = 0; i < BOOTH_AMOUNT; i++) {
        if (*(block_m + i) == -1)
            *(block_m + i) = 0;
    }
}

int Scenery::algorithm(int *block_c, int agent, int condition) {
    int index_bkp = -1, index = -1, *block_cp;
    
    set_block(block_c);
    remove_agents_disable();
    block_cp = cp_pointer(block_c);
    set_block(block_cp);
    if (agent == AGENTS_AMOUNT) 
        this->variations++;  
    
    if (agent <= AGENTS_AMOUNT) {
        do {
            
//            printf("\nAgent %d in process...", agent);
//            printf("\nblock_c: %u ", block_c);
//            set_block(block_c);
//            print_block();
//            printf("\nblock_cp: %u ", block_cp);            
//            set_block(block_cp);
//            print_block();
            
            index =  (condition == 1 || condition == 0) ? one_condition(index_bkp + 1) : -1;
            if (index < 0) {
                index = (condition == 2 || condition == 0) ? two_condition(index_bkp + 1) : -1;                
                if (index < 0) {
                    index = (condition == 3 || condition == 0) ? three_condition(index_bkp + 1) : -1;
                    if (index < 0) {
                        set_block(block_c);
                        block_cp = NULL;
                        remove_agents_disable();
                        delete block_cp;
                        return 0;
                    } else {
                        condition = 3;
                    }
                } else {
                    condition = 2;
                }
            } else {
                condition = 1;
            }
            
            index_bkp = index;
            
            set_block(block_c);
            set_agent(agent, index);
            set_block(block_cp);
            set_agent(agent, index);
            
            algorithm(block_cp, agent + 1, 0);
            
            set_block(block_c);
            set_agent(0, index);
            set_block(block_cp);
            set_agent(0, index);
            
        } while (index >= 0); 
    }
}

void Scenery::start() {
    int *booth_1 = malloc_block(), index;
    clock_t t1, t2;
    
    set_block(booth_1);
    init_block();
    t1 = clock();
    algorithm(booth_1, 1, 1);
    t2 = clock();
    
    printf("\nAmount Variations: %d", this->variations);
    printf("\nMilliseconds: %fms", (((float) t2 - (float) t1) / 1000000.0F) * 1000);
}

void Scenery::print_block() {
    for (int i = 0; i < BOOTH_AMOUNT; i++) {
        printf("%d ", *(this->block_m + i));        
    }
    printf("\n");
}