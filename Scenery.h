#ifndef SCENERY_H
#define	SCENERY_H

#define BOOTH_AMOUNT 3
#define AGENTS_AMOUNT 4

class Scenery {
    
public:
    Scenery();
    void start();    
private:
    int *block_m;   
    unsigned int variations;
    
    int algorithm(int *booth_c, int agent, int condition);
    void init_block();
    void set_block(int *booth);
    void print_block();
    bool is_null(int index);
    int* malloc_block();
    int* cp_pointer(int *p1);
    void set_agent(const int agent, const int index);
    int one_condition(int i);
    int two_condition(int i);
    int three_condition(int i);

};

#endif	/* SCENERY_H */

