#include "include_asm.h"
#include "graphics.h"

typedef struct {
    s32 unk00;
    s16 unk04;
    s32 unk08;
    s32 unk0C;
    s32 unk10;
} graphicsMessage; // OSMesg


extern graphicsMessage D_800ECE20[0x20]; // OSMesg ring buffer
extern s16 D_800ECB2E; // ring buffer index

extern OSMesgQueue D_800D5F18;
extern s16 D_800ED3C2; // Amount of total messages D_800D5F18 currently has

extern void **D_800D5FE8; // Current frame buffer pool
extern u16 D_800D5FEC; // Frame buffer pool size
extern u16 D_800D5FEE;

extern s32 D_800F317C;
extern s32 D_800D5FF0;

/* gThread stacks? */
extern u8 *D_800ECB34;
extern u8 *D_800F09F0;
extern u8 *D_800F2CBC;
extern u8 *gThread3Stack;

/* FIX */
extern s32 D_800D5F10[];
extern u8 D_800B1760;
extern u8 D_800B1830;
extern u32 D_800ED398;

extern s32 D_800ED0C4;
extern s32 D_800F3838;
extern s32 D_800ECB1C;

extern void* D_800D5F30; // Initial Mesg

extern OSMesgQueue D_800D5FB0; // RSP Message Queue
extern void* D_800D5FC8; // Initial Mesg

extern OSThread D_800D1D20; // OSThread
extern u8 D_800D26D0[]; // thread stack pointer
/* FIX */


/* Initialize Graphics System */
void func_8001A600(void **arg1, s32 arg2, s32 arg3, u8 **arg4, s32 arg5) {
    func_8001AB84(arg1, arg2 & 0xFFFF, arg3 & 0xFFFF);
    func_8001ABAC(arg4);
    func_8001ABE8(arg5);
    func_8001ABF4(0);
    D_800D5F10[0] = (s32) &D_800D5FB0;
    D_800D5F10[-0x10] = 1;
    D_800D5F10[-0xF] = 2;
    D_800D5F10[-0xE] = (s32) &D_800B1760;
    D_800D5F10[-0xD] = (s32) (&D_800B1830 - &D_800B1760);
    D_800D5F10[-0xB] = 0x1000;
    D_800D5F10[-0x9] = 0x800;
    D_800D5F10[-0x7] = 0x400;
    D_800D5F10[-0x4] = (s32) &D_800ED398;
    D_800D5F10[-0x3] = 2;
    D_800D5F10[-0x1] = 0xC00;
    D_800ED3C2 = 0;
    D_800ECB2E = 0;
    D_800ED0C4 = 0;
    D_800F3838 = 0;
    D_800ECB1C = 0;
    osCreateMesgQueue(&D_800D5F18, &D_800D5F30, 0x20);
    osCreateMesgQueue(&D_800D5FB0, &D_800D5FC8, 8);
    osCreateThread(&D_800D1D20, 0x10, &func_8001A7DC, NULL, &D_800D26D0, 0x64);
    osStartThread(&D_800D1D20);
}

/* Retrieve available frame buffer (triple buffering?) */
void *func_8001A750(void) {
    void *current;
    void *next;
    void *fBuffer;
    s16 i;

    current = osViGetCurrentFramebuffer();
    next = osViGetNextFramebuffer();
    
    for (i = 0; i < D_800D5FEC; i++) {
        fBuffer = D_800D5FE8[i];
        if (fBuffer != current) {
            if (fBuffer != next) {
                return fBuffer;
            }
        }
    }
    return NULL;
}

INCLUDE_ASM(s32, "../src/engine/graphics", func_8001A7DC);


/* Retrieve graphics OSMesg from ring buffer */
graphicsMessage* func_8001AA70(void) {
    if (D_800ECB2E >= 0x20) {
        D_800ECB2E = NULL;
    }
    return &D_800ECE20[D_800ECB2E++];
}

/* Send graphics OSMesg */
u8 func_8001AAC4(s32 arg0, s16 arg1, s32 arg2, s32 arg3, s32 arg4) {
    graphicsMessage* mesg;
    u32 temp_v0;

    if (D_800ED3C2 >= 0x20) {
        return FALSE;
    }

    temp_v0 = osSetIntMask(1);
    D_800ED3C2++;
    osSetIntMask(temp_v0);
    mesg = func_8001AA70(); // Get OSMesg from ring buffer

    /* Initialize message. */
    mesg->unk00 = arg0;
    mesg->unk04 = arg1;
    mesg->unk08 = arg2;
    mesg->unk0C = arg3;
    mesg->unk10 = arg4;

    osSendMesg(&D_800D5F18, mesg, 1);
    return TRUE;
}

/* Init graphics frame buffer pool */
void func_8001AB84(void **frameBufferPool, u16 size, u16 arg2) {
    D_800D5FE8 = frameBufferPool;
    D_800D5FEC = size;
    D_800D5FEE = arg2;
}

/* Graphics state init? */
void func_8001ABA0(u16 size) {
    D_800D5FEC = size;
}

/* Initialize gThreadStacks */
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
