#include <stdlib.h>
#include <stdio.h>


enum CMSignals{
    Q,
    PAID,
    CB, CD, CS,
    TAKE_BOTTLE
};

enum CMStates{
    GATHER_CASH, DISPENSE_PRODUCT,
    STUCK, IDLE
};

typedef struct CokeMachine{
    uint8_t state;
    uint8_t currBal;
} CokeMachine;

typedef struct EventTag {
    uint16_t sig;
} Event;

void CM_constructor (CokeMachine *cm, uint8_t defuse);

void CM_init (Cokemachine *cm);

void CM_dispatch (CokeMachine *cm, Event const *e);

static Cokemachine cm;

void CM_dipatch (CokeMachine *cm, ){
    switch (cm->state){
        case GATHER_CASH: 
            switch (e->sig){
                case Q:
                    cm->currentBal += 25;
                    break;
                case PAID：
                    cm->state = DISPENSE_PRODUCT;
                    break;
            }
            break;
        case DISPENSE_PRODUCT: 
            switch (e->sig){
                case CB:
                case CD:
                case CS:
                    
                case TAKE_BOTTLE:
                    cm->state = IDLE;
                    break;
            }
        }
        }
    }
    
}

void CM_constructor(Bomb1 *me, uint8_t defuse) {
    me->defuse = defuse; }

void CM_init(Bomb1 *me) {
    me->timeout = INIT_TIMEOUT;
    TRAN(SETTING_STATE);
}

static Bomb1 l_bomb;

int main() {
    Bomb1_constructor(&l_bomb, 0x0D);
    for (;;) { 
        static TickEvt tick_evt = { TICK_SIG, 0};

        usleep(100000);
        if (++tick_evt.fine_time == 10) {
            tick_evt.fine_time = 0;
        }

        Bomb1_dispatch(&l_bomb, (Event*)&tick_evt);
        if (kbhit())
            Bomb1_dispatch(&l_bomb, e);
    }
}

void CM_dispatch(Bomb1 *me, Event const *e) {
    switch (me->state) {
        case SETTING_STATE: {
            switch (e->sig) {
                case UP_SIG: {
                    if (me->timeout < 60)
                        display(++me->timeout);
                    break;
                }
                case DOWN_SIG: {
                    if (me->timeout > 1)
                        display(--me->timeout);
                    break;
                }
                case ARM_SIG: {
                    me->code = 0;
                    TRAN(TIMING_STATE);           
                    break;
                }
             }
             break;
        } /* end case SETTING_STATE */
