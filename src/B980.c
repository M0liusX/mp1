#include "common.h"

#include "PR/os.h"
#include "PR/libaudio.h"
#include "B980.h"

extern file_1ACF0_struct D_800C18A0;
extern OSMesg D_800CD9C8;
extern OSMesgQueue D_800CDA90;
extern s8 D_800ECB2C;
extern void func_800130A4(Addr*);
extern s32 D_800C1870;
extern Addr D_800C1874;
extern ALCSPlayer* D_800CDAD4;
extern s32 D_800CDAEC;
extern ALSndPlayer* D_800CEA8C;
extern s32 D_800CEAA0;
extern s32 D_800CDACC;

extern FXDO_Unk* D_800CDAC8;
extern s32 D_800C1870;

extern ALHeap D_800CDAA8;
extern FXD0_Unk3 D_800C1898;
extern FXDO_Unk  D_800C18B4;
extern FXDO_Unk* D_800C18B8;

extern unk_D_800CEA94 *D_800CEA94;
extern s32 D_800CEA9C;

extern void func_80061FA0(OSIoMesg*, s32, s32, s32, FXDO_Unk*, s32, OSMesgQueue*); /* TODO: find header */
void func_8000AD80(s32 arg0, void* arg1, s32 arg2) {
    OSIoMesg sp20;

    osInvalDCache(arg1, arg2);
    func_80061FA0(&sp20, 0, 0, arg0, arg1, arg2, &D_800CDA90);
    osRecvMesg(&D_800CDA90, NULL, 1);
}

s32 func_8000ADFC(s8 arg0) {
    if (arg0 <= 0) {
        return 0;
    }
    return (arg0 << 0x8) | 0xFF;
}

void func_8000AE20(s16* arg0, s16 arg1) {
    if (*arg0 < 0) {
        *arg0 = 0;
        return;
    }
    if (*arg0 > arg1) {
        *arg0 = arg1;
    }
}

s32 func_8000AE50(void) {
    s32 temp_s1;

    D_800CDAC8 = func_8000AFA0(8);
    if (D_800CDAC8 == NULL) {
        return 1;
    }
    D_800CDAC8->unk_04 = 0;
    if (D_800C1898.unk00 != 0) {
        func_8000AD80(D_800C1898.unk00, D_800CDAC8, 8);
        if (D_800CDAC8->unk_00.sint != 0x46584430) {
            D_800CDAC8->unk_04 = 0;
        }
    }

    if (D_800C18B4.unk_00.sint >= 0x14) {
        temp_s1 =  D_800C18B4.unk_00.sint - 0x14;
        if (temp_s1 < D_800CDAC8->unk_04) {
            temp_s1 = D_800C1898.unk00 + (temp_s1 * 0x208) + 0x10;
            D_800C1898.unk20 = func_8000AFA0(0x208);
            if (D_800C1898.unk20 == NULL) {
                return 1;
            }
            func_8000AD80(temp_s1, D_800C18B8, 0x208);
            return 0;
        }
        func_8000AFF8(0, 0, 0x20);
        return 0x64;
    }

    if (D_800C18B4.unk_00.sint < 6) {
        D_800C18B4.unk_04 = 0;
        return 0;
    }

    if (D_800C18B4.unk_00.sint == 6) {
        if (D_800C18B4.unk_04 != 0) {
            return 0;
        }
    } 
    
    func_8000AFF8(0, 0, 0x20);
    return 0x64;
}

/* Alloc from Heap. */
void* func_8000AFA0(s32 size) {
    void* temp_v0 = alHeapDBAlloc(0, 0, &D_800CDAA8, 1, size);

    if (temp_v0 == NULL) {
        func_8000AFF8(0, 0, 1);
    }
    return temp_v0;
}

/* [DEBUG] Heap Allocation Failed Error. */
s32 func_8000AFF8(s32 arg0, s32 arg1, s32 arg2) {
    return 0;
}

void func_8000B000(s32 arg0) {
    D_800CDACC = arg0;
}

s32 func_8000B00C(s32 arg0, s32 arg1, Addr* arg2, Addr* arg3) {
    D_800C18A0.unk_48 = arg0;
    D_800C18A0.unk_4C = arg1;
    D_800C18A0.unk_50 = 1;
    D_800C18A0.unk_00 = arg2;
    D_800C18A0.unk_04 = arg3;
    return func_8000B13C();
}

/* Unused Function */
extern unusedX40 D_800C18A8;
void func_8000B044(unusedX40_2* arg0) {
   D_800C18A8.unk04 = arg0->unk04;
   D_800C18A8.unk08 = arg0->unk08;
   D_800C18A8.unk0C = arg0->unk10;
   D_800C18A8.unk10 = arg0->unk14;
   D_800C18A8.unk2C = arg0->unk18;
   D_800C18A8.unk30 = arg0->unk1C;
   D_800C18A8.unk34 = arg0->unk20;
   D_800C18A8.unk38 = arg0->unk24;
   D_800C18A8.unk3C = arg0->unk28;
   D_800C18A8.unk18 = arg0->unk2C;
   D_800C18A8.unk1C = arg0->unk30;
   D_800C18A8.unk24 = arg0->unk34;
   D_800C18A8.unk14 = arg0->unk38;
   D_800C18A8.unk00 = arg0->unk0C;
}

/* Unused Function (Inverse of top) */
void func_8000B0C0(unusedX40_2* arg0) {
   arg0->unk04 = D_800C18A8.unk04;
   arg0->unk08 = D_800C18A8.unk08;
   arg0->unk10 = D_800C18A8.unk0C;
   arg0->unk14 = D_800C18A8.unk10;
   arg0->unk18 = D_800C18A8.unk2C;
   arg0->unk1C = D_800C18A8.unk30;
   arg0->unk20 = D_800C18A8.unk34;
   arg0->unk24 = D_800C18A8.unk38;
   arg0->unk28 = D_800C18A8.unk3C;
   arg0->unk2C = D_800C18A8.unk18;
   arg0->unk30 = D_800C18A8.unk1C;
   arg0->unk34 = D_800C18A8.unk24;
   arg0->unk38 = D_800C18A8.unk14;
   arg0->unk0C = D_800C18A8.unk00;
}

s32 func_8000B13C(void) {
    osCreateMesgQueue(&D_800CDA90, &D_800CD9C8, 50);
    D_800ECB2C = 0;
    if (D_800C18A0.unk_00 != 0) {
        if (D_800C18A0.unk_04 != 0) {
            return func_8000B210();
        }
    }
    return 1;
}

/* Conditional delete seq and snd players. */
s32 func_8000B198(void) {
    if (D_800CDAEC == 0) {
        if (D_800CEAA0 == 0) {
            if (D_800C1870 & 0x8000) {
                alSeqpDelete((ALSeqPlayer*) D_800CDAD4); // This is compatible.
                alSndpDelete(D_800CEA8C);
                func_800130A4(&D_800C1874);
                D_800C1870 = 0;
                return 0;
            }
        } else {
            return 1;
        }
        return 0;
    }
    return 1;
}

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
} unk_D_800CDAB8;
extern s32 func_8000B3E8();                                /* extern */
extern s32 func_8000D65C();                                /* extern */
extern s32 func_80012CF0(u8*, ALHeap*);                    /* extern */
extern void func_80013010(u8*);                            /* extern */
extern unk_D_800CDAB8 D_800CDAB8;
s32 func_8000B210(void) {
    s32 var_v0;

    alHeapInit(&D_800CDAA8, *D_800C18A0.unk_00, (s32) D_800C18A0.unk_04);
    D_800CDAB8.unk0 = D_800CDAB8.unk4 = D_800CDAB8.unk8 = D_800CDAB8.unkC = 0;

    var_v0 = func_8000AE50();
    if (var_v0) { return var_v0; }
    var_v0 = func_80012CF0(D_800C1874, &D_800CDAA8);
    if (var_v0) { return var_v0; }
    var_v0 = func_8000B3E8();
    if (var_v0) { return var_v0; }
    var_v0 = func_8000D65C();
    if (var_v0) { return var_v0; }

    func_80013010(D_800C1874);
    D_800C1870 = 0x8000;
    return 0;
}

Addr* func_8000B2BC(void) {
    return &D_800C1874;
}

s32 func_8000B2C8(void) {
    if (D_800C1870 & 0x8000) {
        return D_800CDAC8->unk_04;
    }
    return 0;
}

s32 func_8000B2F0(void) {
    return 0x610032;
}

/* Get remaining heap size. */
s32 func_8000B2FC(void) {
    return D_800CDAA8.len - (D_800CDAA8.cur - D_800CDAA8.base);
}

/* Getters / Setters */
s32 func_8000B31C() {
    return D_800CDAB8.unk0;
}

void func_8000B328() {
    D_800CDAB8.unk0 = 0;
}

s32 func_8000B334() {
    return D_800CDAB8.unk4;
}

s32 func_8000B340() {
    return D_800CDAB8.unk8;
}

s32 func_8000B34C() {
    return D_800CDAB8.unkC;
}

s32 func_8000B358() {
    return D_800CDACC;
}

void func_8000B364(s32 arg0) {
    s32 i;
    unk_D_800CEA94* temp_v1;

    D_800ECB2C = arg0 & 1;
    for (i = 0; i < D_800CEA9C; i++) {
        temp_v1 = &D_800CEA94[i];
        if (temp_v1->unkC == 1) {
            temp_v1->unk8 =  temp_v1->unk8 | 4;
        }
    }
}

/* [DEBUG] */
s32 func_8000B3E0() {
    return 0;
}



extern ALSeqFile* D_800CDAE0;

/* Load Song file and bank */
/* decomp.me: https://decomp.me/scratch/VLk6f ~80% */
INCLUDE_ASM(s32, "B980", func_8000B3E8);

/* 2-Byte Align And Reserve Heap */
s32 func_8000B7EC(s32 fileAddr) {
    fileAddr = (fileAddr & 1) + fileAddr; // Align
    D_800CDAE0 = func_8000AFA0(fileAddr);
    if (D_800CDAE0 == NULL) {
        return 1;
    }
    func_8000AD80(*(s32*)D_800C1874, D_800CDAE0, fileAddr);
    return 0;
}

/* Set player, song, bank, volume, and play song. */
INCLUDE_ASM(s32, "B980", func_8000B844);


INCLUDE_ASM(s32, "B980", func_8000BB30);

INCLUDE_ASM(s32, "B980", func_8000BE6C);

INCLUDE_ASM(s32, "B980", func_8000BE98);

INCLUDE_ASM(s32, "B980", func_8000BEBC);

INCLUDE_ASM(s32, "B980", func_8000BEEC);

INCLUDE_ASM(s32, "B980", func_8000C144);

INCLUDE_ASM(s32, "B980", func_8000C184);

INCLUDE_ASM(s32, "B980", func_8000C1AC);

INCLUDE_ASM(s32, "B980", func_8000C1B8);

INCLUDE_ASM(s32, "B980", func_8000C250);

INCLUDE_ASM(s32, "B980", func_8000C2D4);

INCLUDE_ASM(s32, "B980", func_8000C390);

INCLUDE_ASM(s32, "B980", func_8000C414);

INCLUDE_ASM(s32, "B980", func_8000C4A0);

INCLUDE_ASM(s32, "B980", func_8000C4BC);

INCLUDE_ASM(s32, "B980", func_8000C544);

INCLUDE_ASM(s32, "B980", func_8000C5C4);

INCLUDE_ASM(s32, "B980", func_8000C64C);

INCLUDE_ASM(s32, "B980", func_8000C748);

INCLUDE_ASM(s32, "B980", func_8000C808);

INCLUDE_ASM(s32, "B980", func_8000CCC0);

INCLUDE_ASM(s32, "B980", func_8000D600);

INCLUDE_ASM(s32, "B980", func_8000D618);

INCLUDE_ASM(s32, "B980", func_8000D65C);

INCLUDE_ASM(s32, "B980", func_8000DA04);

INCLUDE_ASM(s32, "B980", func_8000DA7C);

INCLUDE_ASM(s32, "B980", func_8000DB24);

INCLUDE_ASM(s32, "B980", func_8000DC44);

INCLUDE_ASM(s32, "B980", func_8000DCCC);

void func_8000DDEC();
INCLUDE_ASM(s32, "B980", func_8000DDEC);

INCLUDE_ASM(s32, "B980", func_8000DE5C);

void func_8000DF98(s16, s16*);  
INCLUDE_ASM(s32, "B980", func_8000DF98);

INCLUDE_ASM(s32, "B980", func_8000E21C);

INCLUDE_ASM(s32, "B980", func_8000E2D0);

INCLUDE_ASM(s32, "B980", func_8000E340);

INCLUDE_ASM(s32, "B980", func_8000E448);

INCLUDE_ASM(s32, "B980", func_8000E818);

INCLUDE_ASM(s32, "B980", func_8000E92C);

INCLUDE_ASM(s32, "B980", func_8000EC14);

INCLUDE_ASM(s32, "B980", func_8000F078);

INCLUDE_ASM(s32, "B980", func_8000F118);

INCLUDE_ASM(s32, "B980", func_8000F198);

void func_8000F238();  
INCLUDE_ASM(s32, "B980", func_8000F238);

INCLUDE_ASM(s32, "B980", func_8000F294);

INCLUDE_ASM(s32, "B980", func_8000F4E0);

INCLUDE_ASM(s32, "B980", func_8000F780);

INCLUDE_ASM(s32, "B980", func_8000F844);

INCLUDE_ASM(s32, "B980", func_80010040);

INCLUDE_ASM(s32, "B980", func_80010110);

INCLUDE_ASM(s32, "B980", func_80010148);

INCLUDE_ASM(s32, "B980", func_80010734);

INCLUDE_ASM(s32, "B980", func_8001085C);

INCLUDE_ASM(s32, "B980", func_800108C8);

INCLUDE_ASM(s32, "B980", func_80010998);

void func_80010A68(s16, s16);
INCLUDE_ASM(s32, "B980", func_80010A68);

void func_80010B38(s16, s8);
INCLUDE_ASM(s32, "B980", func_80010B38);

INCLUDE_ASM(s32, "B980", func_80010C08);

INCLUDE_ASM(s32, "B980", func_80010C30);

INCLUDE_ASM(s32, "B980", func_80010C4C);

INCLUDE_ASM(s32, "B980", func_80010C78);

INCLUDE_ASM(s32, "B980", func_80010ED4);

INCLUDE_ASM(s32, "B980", func_80011164);

INCLUDE_ASM(s32, "B980", func_8001136C);

INCLUDE_ASM(s32, "B980", func_800113C4);

INCLUDE_ASM(s32, "B980", func_800114A4);

INCLUDE_ASM(s32, "B980", func_80011530);

INCLUDE_ASM(s32, "B980", func_800115BC);

INCLUDE_ASM(s32, "B980", func_800115C8);

INCLUDE_ASM(s32, "B980", func_80011634);

INCLUDE_ASM(s32, "B980", func_8001165C);

INCLUDE_ASM(s32, "B980", func_800117AC);

INCLUDE_ASM(s32, "B980", func_80011A30);

INCLUDE_ASM(s32, "B980", func_80011A80);

INCLUDE_ASM(s32, "B980", func_80011B2C);

INCLUDE_ASM(s32, "B980", func_80011C04);

INCLUDE_ASM(s32, "B980", func_80011D48);

INCLUDE_ASM(s32, "B980", func_80012140);

INCLUDE_ASM(s32, "B980", func_80012260);

INCLUDE_ASM(s32, "B980", func_80012394);

INCLUDE_ASM(s32, "B980", func_800123DC);

INCLUDE_ASM(s32, "B980", func_8001249C);

typedef struct {
    s16 unk0;
    s8  unk2[0x16];
    u8  unk18;
} unk_func_80012C7C;

extern s16* D_800CEA90;

extern s32 D_800CEAA4;
extern s32 D_800CEAA8;
extern f32 D_800CEAE8;
extern f32 D_800CEAEC;
extern s32 D_800CEAF0;
extern s8 D_800CEAF4;
extern s8 D_800CEAF5;

extern s16 D_800CEAB0;
extern s16 D_800CEAB2;
extern s16 D_800CEAB4;
extern s8  D_800CEAB8;
extern u8  D_800CEAB9;
extern s8  D_800CEABA;
extern s8  D_800CEABB;

// INCLUDE_ASM(s32, "B980", func_80012574);
void func_80012574(s16 arg0, s16 arg1) {
    s16 var_s1;
    s32 temp_v1;
    unk_D_800CEA94* temp_s0;

    temp_s0 = &D_800CEA94[arg0];
    var_s1 = arg1;
    if (temp_s0->unkC == 1) {
        D_800CEAA4 |= 0x10;
        if (arg1 >= 0x4B1) {
            var_s1 = 0x4B0;
        }
        temp_v1 = temp_s0->unk8;
        if (!(temp_v1 & 0x1000)) {
            temp_s0->unk8 = temp_v1 | 1;
            if (temp_s0->unk29 == 2) {
                func_80010A68(arg0, var_s1);
            }
        }
        temp_s0->unk1A = var_s1;
        D_800CEAA4 &= ~0x10;
    }
}

void func_80012654(s16 arg0, s8 arg1) {
    s32 temp_v1;
    s8 temp_a1;
    unk_D_800CEA94* temp_s0;

    temp_s0 = &D_800CEA94[arg0];
    if ((temp_s0->unkC == 1) && !(temp_s0->unk8 & 0x10)) {
        D_800CEAA4 |= 0x10;
        temp_a1 = arg1 & ((s32) ~arg1 >> 0x1F);
        temp_v1 = temp_s0->unk8;
        if (!(temp_v1 & 0x1000)) {
            temp_s0->unk8 = temp_v1 | 8;
            if (temp_s0->unk29 == 2) {
                func_80010B38(arg0, temp_a1);
            }
        }
        temp_s0->unk28 = (u8) temp_a1;
        D_800CEAA4 &= ~0x10;
    }
}


void func_80012738(s8 arg0) {
    s16 temp_v0;

    D_800CEAA4 |= 0x10;
    temp_v0 = func_8000ADFC(arg0);
    D_800CEAB2 = temp_v0;
    D_800CEAB0 = temp_v0;
    func_8000DDEC();
    D_800CEAA8 = 0;
    D_800CEAA4 &= ~0x10;
}

void func_800127A0(s8 arg0) {
    s32 i;
    unk_D_800CEA94* temp_a1;

    D_800CEAA4 |= 0x10;
    D_800CEAB8 = arg0;

    for (i = 0; i < D_800CEA9C; i++) {
        temp_a1 = &D_800CEA94[i];
        if (temp_a1->unkC == 1) {
            if (!(temp_a1->unk8 & 0x1000) && (temp_a1->unk22 != D_800CEAB8)) {
                temp_a1->unk8 = (s32) (temp_a1->unk8 | 2);
            }
            temp_a1->unk22 = D_800CEAB8;
        }
    }

    D_800CEAA4 &= ~0x10;
}

void func_8001286C(u8 arg0) {
    s32 i;
    unk_D_800CEA94* temp_a1;

    D_800CEAA4 |= 0x10;
    arg0 = MIN(arg0, 0x7F);
    for (i = 0; i < D_800CEA9C; i++) {
        temp_a1 = &D_800CEA94[i];
        if (temp_a1->unkC == 1) {
            if (!(temp_a1->unk8 & 0x1000) && (temp_a1->unk25 != arg0)) {
                temp_a1->unk8 = (s32) (temp_a1->unk8 | 4);
            }
            temp_a1->unk25 = arg0;
        }
    }

    D_800CEAB9 = arg0;
    D_800CEAA4 &= ~0x10;
}

void func_8001293C(s16 arg0) {
    s32 i;
    unk_D_800CEA94* temp_a1;

    D_800CEAA4 |= 0x10;
    arg0 = MIN(0x4B0, arg0);
    for (i = 0; i < D_800CEA9C; i++) {
        temp_a1 = &D_800CEA94[i];
        if (temp_a1->unkC == 1) {
            if (!(temp_a1->unk8 & 0x1000) && (temp_a1->unk1A != arg0)) {
                temp_a1->unk8 = (s32) (temp_a1->unk8 | 1);
            }
            temp_a1->unk1A = arg0;
        }
    }

    D_800CEAB4 = arg0;
    D_800CEAA4 &= ~0x10;
}

void func_80012A18(s8 arg0) {
    s32 i;
    unk_D_800CEA94* temp_a1;

    D_800CEAA4 |= 0x10;
    arg0 = MAX(arg0, 0);
    for (i = 0; i < D_800CEA9C; i++) {
        temp_a1 = &D_800CEA94[i];
        if (temp_a1->unkC == 1) {
            if (!(temp_a1->unk8 & 0x10)) {
                if (!(temp_a1->unk8 & 0x1000) && (temp_a1->unk28 != (arg0 & 0xFF))) {
                    temp_a1->unk8 = (s32) (temp_a1->unk8 | 8);
                }
                temp_a1->unk28 = arg0;
            }
        }
    }

    D_800CEABA = arg0;
    D_800CEAA4 &= ~0x10;
}

s8 func_80012AF8() {
    return D_800CEAB8;
}

u8 func_80012B04() {
    return D_800CEAB9;
}

s16 func_80012B10() {
    return D_800CEAB4;
}

s8 func_80012B1C() {
    return D_800CEABA;
}

void func_80012B28(s8 arg0, s8 arg1, s8 arg2, s8 arg3) {
    if ((D_800CEAA4 & 0x8000) && (*D_800CEA90 == 0x5433)) {
        D_800CEAF0 = D_800CEA9C;
        func_8000F238();
        if (((((arg0 << 0x18) == 0) | ((arg1 << 0x18) == 0)) != 0) || ((((arg2 << 0x18) == 0) | ((arg3 << 0x18) == 0)) != 0)) {
            D_800CEAF0 = 0;
        }
        D_800CEAF4 = arg0;
        D_800CEAF5 = arg1;
        D_800CEAE8 = (f32) arg2 / 127.0f;
        D_800CEAEC = (f32) arg3 / 127.0f;
    }
}

void func_80012C70(s8 arg0) {
    D_800CEABB = arg0;
}

u32 func_80012C7C(s16 arg0) {
    unk_func_80012C7C sp10;

    if (D_800CEAA4 & 0x8000) {
        u8 condition = (arg0 < D_800CEA90[1]) ^ 1;
        if (( (condition) | ((u32)arg0 >> 0x1F) ) == 0) {
            func_8000DF98(arg0, &sp10.unk0);
            return sp10.unk18;
        }
    }
    return -1;
}
