#ifndef SRC_B980_H
#define SRC_B980_H

//FXDO related
typedef struct FXDO_Unk {
/* 0x04 */  union {
                char charray[4];
                s32 sint;
            } unk_00;
/* 0x04 */  s32 unk_04;
} FXDO_Unk; //unk size

typedef struct FXD0_Unk3 {
    s32 unk00;
    u8  unk04[0x1C];
    FXDO_Unk* unk20;
} FXD0_Unk3;

FXDO_Unk* func_8000AFA0(s32 arg0);
s32 func_8000AFF8(s32, s32, s32);

s32 func_8000B13C(void);
s32 func_8000B210(void);


/* Unused */
typedef struct unusedX40 {
    s32 unk00;
    s32 unk04;
    s32 unk08;
    s32 unk0C;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
} unusedX40;

typedef struct unusedX40_2 {
    s32 unk00;
    s32 unk04;
    s32 unk08;
    s32 unk0C;
    u8  unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    u16 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
} unusedX40_2;

#endif //SRC_B980_H
