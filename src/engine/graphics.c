#include "common.h"


typedef struct {
    u8 pad[0x14];
} graphicsMessage; // OSMesg

extern s16 D_800ECB2E;
extern graphicsMessage D_800ECE20[]; // Message ring buffer

extern u8 *D_800ECB34;
extern u8 *D_800F09F0;
extern u8 *D_800F2CBC;
extern u8 *gThread3Stack;

extern s32 D_800D5FE8;
extern s16 D_800D5FEC;
extern s16 D_800D5FEE;

extern s16 D_800D5FEC;
extern s32 D_800F317C;
extern s32 D_800D5FF0;


INCLUDE_ASM(s32, "../src/engine/graphics", func_8001A600);

INCLUDE_ASM(s32, "../src/engine/graphics", func_8001A750);

INCLUDE_ASM(s32, "../src/engine/graphics", func_8001A7DC);


/* Retrieve graphics OSMesg from ring buffer */
graphicsMessage* func_8001AA70(void) {
    if (D_800ECB2E >= 0x20) {
        D_800ECB2E = 0;
    }
    return &D_800ECE20[D_800ECB2E++];
}


INCLUDE_ASM(s32, "../src/engine/graphics", func_8001AAC4);


/* Graphics state init? */
void func_8001AB84(s32 arg0, s16 arg1, s16 arg2) {
    D_800D5FE8 = arg0;
    D_800D5FEC = arg1;
    D_800D5FEE = arg2;
}

/* Graphics state init? */
void func_8001ABA0(s16 arg0) {
    D_800D5FEC = arg0;
}

/* Initialize gThreadStacks. */
void func_8001ABAC(u8 **arg0) {
    gThread3Stack = *arg0++;
    D_800F2CBC = *arg0++;
    D_800ECB34 = *arg0++;
    D_800F09F0 = *arg0;
}

/* Graphics state init? */
void func_8001ABE8(s32 arg0) {
    D_800F317C = arg0;
}

/* Graphics state init? */
void func_8001ABF4(s32 arg0) {
    D_800D5FF0 = arg0;
}
