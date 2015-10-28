#include <stdio.h>
#include <stdlib.h>
#include <string.h>


enum CMSignals {
    NOSIG, Q, PAID,
    CB, CD, CS,
    TAKE_BOTTLE
};

enum CMStates {
    IDLE, GATHER_CASH,
    DISPENSE_PRODUCT
};

typedef struct CokeMachine{
    uint8_t state;
    uint8_t currBal;
} CokeMachine;

typedef struct EventTag {
    uint16_t sig;
} Event;


#define TRAN(target_) (cm->state = (uint8_t)(target_))


void CM_constructor(CokeMachine *cm){
    cm->currBal = 0;
}

void CM_init(CokeMachine *cm){
    TRAN(IDLE);
}

void CM_dispatch (CokeMachine *cm, Event const *e){
    switch (cm->state){
        case IDLE:
            if (e->sig == Q){
                    cm->currBal += 25;
                    TRAN(GATHER_CASH);
            }
            break;

        case GATHER_CASH:
            switch (e->sig){
                case Q:
                    cm->currBal += 25;
                    break;
                case PAID:
                    TRAN(DISPENSE_PRODUCT);
                    break;
            }
            break;
        case DISPENSE_PRODUCT:
            switch (e->sig){
                case CB:
                    printf("Coke Dispensed\n");
                    break;
                case CD:
                    printf("Diet Coke Dispensed\n");
                    break;
                case CS:
                    printf("Sprite Dispensed\n");
                    break;
                case TAKE_BOTTLE:
                    TRAN(IDLE);
                    cm->currBal = 0;
                    break;
            }
            break;
        }
}

static CokeMachine cm;

int main() {
    CM_constructor(&cm);
    CM_init(&cm);
    uint8_t sigin;

    for (;;) {
        char s[20];
        static Event event = {NOSIG};

        printf("Please input a signal: ");
        scanf("%s", s);
        sigin = (!strcmp(s,"Q") << 4) | (!strcmp(s, "TAKE_BOTTLE") << 3)\
         | (!strcmp(s, "CB") << 2) | (!strcmp(s, "CD") << 1) \
         | !strcmp(s, "CS");

        printf("%x\n", sigin);
        switch (sigin){
            case 0x10:
                event.sig = Q;
                break;
            case 0x8:
                event.sig = TAKE_BOTTLE;
                break;
            case 0x4:
                event.sig = CB;
                break;
            case 0x2:
                event.sig = CD;
                break;
            case 0x1:
                event.sig = CS;
                break;
            default:
                if (cm.state == GATHER_CASH && cm.currBal == 50)
                    event.sig = PAID;
                break;
        }
    printf("%d\n", event.sig);
    
    CM_dispatch(&cm, &event);
    }
}
