#include "common.h"
#include "54120.h"

void func_80055810(s32 arg0, s32 arg1, s32);

INCLUDE_ASM(s32, "54120", func_80053520);

INCLUDE_ASM(s32, "54120", func_80053A1C);

INCLUDE_ASM(s32, "54120", func_80053D88);

INCLUDE_ASM(s32, "54120", func_80053E8C);

INCLUDE_ASM(s32, "54120", func_80053FEC);

INCLUDE_ASM(s32, "54120", func_8005412C);

INCLUDE_ASM(s32, "54120", func_800541F0);

INCLUDE_ASM(s32, "54120", func_800542D0);

INCLUDE_ASM(s32, "54120", func_800543D8);

INCLUDE_ASM(s32, "54120", func_800544E4);

INCLUDE_ASM(s32, "54120", func_8005456C);

INCLUDE_ASM(s32, "54120", func_80054654);

INCLUDE_ASM(s32, "54120", func_800546B4);

INCLUDE_ASM(s32, "54120", func_80054730);

INCLUDE_ASM(s32, "54120", func_80054744);

INCLUDE_ASM(s32, "54120", func_80054758);

INCLUDE_ASM(s32, "54120", HidePlayerHUDVisibility);

INCLUDE_ASM(s32, "54120", func_80054834);

INCLUDE_ASM(s32, "54120", func_80054868);

INCLUDE_ASM(s32, "54120", func_80054FA8);

INCLUDE_ASM(s32, "54120", func_80054FE4);

INCLUDE_ASM(s32, "54120", func_800550C4);

INCLUDE_ASM(s32, "54120", func_80055228);

INCLUDE_ASM(s32, "54120", func_8005528C);

INCLUDE_ASM(s32, "54120", func_8005546C);

INCLUDE_ASM(s32, "54120", func_80055544);

INCLUDE_ASM(s32, "54120", func_800555D0);

INCLUDE_ASM(s32, "54120", func_80055810);

void func_80055960(s32 arg0, s32 arg1) {
    func_80055810(arg0, arg1, 1);
}

s32 func_8005597C(s32 arg0) {
    if (D_800D84D0[arg0] != 0) {
        return 1;
    } else {
        return 0;
    }
}

void func_80055994(s32 arg0) {
    D_800D83A8[arg0].unk_05 = 0;
}

s32 func_800559A8(void) {
    if (D_800D83A8[0].unk_06 & 1) {
        return 0;
    } else {
        return 1;
    }
}

void func_800559BC(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        D_800D83A8[i].unk_06 |= 1;
    }
}

void func_800559F8(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        D_800D83A8[i].unk_06 &= ~1;
    }
}

void func_80055A34(s32 arg0) {
    D_800C54D4 = arg0;
}

void func_80055A40(s32 arg1) {
    s32 i;
    for (i = 0; i < 4; i++) {
        func_800543D8(i);
        func_80054758(i, D_800C54D8[i + 4][0], D_800C54D8[i + 4][1]);
        func_800546B4(i, gPlayers[i].turn_status);
    }

    D_800D83A0 = InitProcess(func_80053A1C, 0U, 0x2000, 0);
    func_8005D900(D_800D83A0, 0x80);
    D_800D84AA = -1;
    D_800D84A8 = -1;
}

void func_80055AFC(void) {
    func_8005456C(D_800C54D0);
    func_8005456C(D_800C54D4);
    KillProcess(D_800D83A0);
    func_80055228();
    D_800C54D0 = -1;
    D_800C54D4 = -1;
}

void func_80055B50(s32 arg0, s32 arg1) {
    D_800C54D0 = arg0;
    D_800C54D4 = arg1;
}

void func_80055B64(void) {
    func_80054868(0x16);
}

void func_80055B80(void) {
    func_80054868(0x17);
}

INCLUDE_ASM(s32, "54120", func_80055B9C);

void func_80055D28(void) {
    s32 lives;

    while (1) {
        lives = D_800F37BB;
        if (lives < 0) {
            lives = 0;
        }
        
        if (lives >= 100) {
            lives = 99;
        }
        
        func_800672B0(D_800D84E4, 0, 1);
        func_800672DC(D_800D84E4, 0, (lives / 10), 0);
        func_800672B0(D_800D84E4, 1, 1);
        func_800672DC(D_800D84E4, 1, (lives % 10), 0);
        SleepVProcess();      
    }
}

void func_80055E08(void) {
    s32 coins;

    while (1) {
        coins = D_800F37C4;
        if (coins < 0) {
            coins = 0;
        }
        
        if (coins >= 100) {
            coins = 99;
        }
        
        func_800672B0(D_800D855C, 0, 1);
        func_800672DC(D_800D855C, 0, (coins / 10), 0);
        func_800672B0(D_800D855C, 1, 1);
        func_800672DC(D_800D855C, 1, (coins % 10), 0);
        SleepVProcess();      
    }
}

void func_80055EE8(void) {
    void* file;
    s32 i;

    func_80076740(D_800D85D8, &D_800CB298, 0, -1, -1);
    func_80066DC4(D_800D85EC.unk_00, 0, 0xA0, 0x18);
    func_80076740(D_800D85EC.unk_54, &D_800CB2A0, 0, -1, -1);
    func_80066DC4(D_800D85EC.unk_68, 0, 0xA0, 0x28);
    file = ReadMainFS(0x7C);
    D_800D85D6 = func_800678A4(file);
    FreeMainFS(file);
    D_800D85D4 = func_80064EF4(2, 5);
    
    for (i = 0; i < 2; i++) {
        func_80067208(D_800D85D4, i, D_800D85D6, 0);
        func_800672B0(D_800D85D4, i, 1);
        func_80067384(D_800D85D4, i, 0xA);
        func_800674BC(D_800D85D4, i, 0x1000);
        if (i == 0) {
            func_80066DC4(D_800D85D4, 0, 0x90, 0x28);
        } else {
            func_80066DC4(D_800D85D4, i, 0x20, 0);
        }
    }
    
    D_800D86A8 = InitProcess(func_80055B9C, 0U, 0, 0);
    func_8005D900(D_800D86A8, 0x80);
}


void func_8005608C(void) {
    Process* process;
    void* file;
    s32 i;

    D_800D84E0 = func_80064EF4(1, 5);
    func_80066DC4(D_800D84E0, 0, 0x2A, 0x22);
    file = ReadMainFS(D_800C56B0[D_800F37B8]);
    D_800D84E2 = func_800678A4(file);
    FreeMainFS(file);
    func_80067208(D_800D84E0, 0, D_800D84E2, 0);
    func_800672B0(D_800D84E0, 0, 1);
    func_80067384(D_800D84E0, 0, 0x10);
    func_800674BC(D_800D84E0, 0, 0x1000);
    file = ReadMainFS(0xA0163);
    D_800D84EA = func_800678A4(file);
    FreeMainFS(file);
    D_800D84E8 = func_80064EF4(1, 5);
    func_80067208(D_800D84E8, 0, D_800D84EA, 0);
    func_800672B0(D_800D84E8, 0, 1);
    func_80067384(D_800D84E8, 0, 0x11);
    func_800674BC(D_800D84E8, 0, 0x1000);
    func_80067354(D_800D84E8, 0, 0.5f, 0.5f);
    func_80066DC4(D_800D84E8, 0, 0x2A, 0x22);
    func_80076740(D_800D84F0, D_800CB2A4, 0, -1, -1);
    func_80066DC4(D_800D8504, 0, 0x44, 0x22);
    file = ReadMainFS(0x7C);
    D_800D84E6 = func_800678A4(file);
    FreeMainFS(file);
    D_800D84E4 = func_80064EF4(2, 5);
    
    for (i = 0; i < 2; i++) {
        func_80067208(D_800D84E4, i, D_800D84E6, 0);
        func_800672B0(D_800D84E4, i, 1);
        func_80067384(D_800D84E4, i, 0xA);
        func_800674BC(D_800D84E4, i, 0x1000);
        if (i == 0) {
            func_80066DC4(D_800D84E4, 0, 0x54, 0x22);
        } else {
            func_80066DC4(D_800D84E4, i, i * 16, 0);
        }
    }

    D_800D8558 = InitProcess(func_80055D28, 0, 0, 0);
    func_8005D900(D_800D8558, 0x80);
}

void func_80056380(void) {
    Process* process;
    void* file;
    s32 i;

    file = ReadMainFS(0xA0013);
    D_800D8562 = func_800678A4(file);
    FreeMainFS(file);
    D_800D8560 = func_80064EF4(1, 5);
    func_80067208(D_800D8560, 0, D_800D8562, 0);
    func_800672B0(D_800D8560, 0, 1);
    func_80067384(D_800D8560, 0, 0x11);
    func_800674BC(D_800D8560, 0, 0x1000);
    func_80066DC4(D_800D8560, 0, 0xE2, 0x22);
    func_80067284(D_800D8560, 0, 0);
    func_80076740(D_800D8568, D_800CB2A4, 0, -1, -1);
    func_80066DC4(D_800D857C, 0, 0xFC, 0x22);
    file = ReadMainFS(0x7C);
    D_800D855E = func_800678A4(file);
    FreeMainFS(file);
    D_800D855C = func_80064EF4(2, 5);
    
    for (i = 0; i < 2; i++) {
        func_80067208(D_800D855C, i, D_800D855E, 0);
        func_800672B0(D_800D855C, i, 1);
        func_80067384(D_800D855C, i, 0xA);
        func_800674BC(D_800D855C, i, 0x1000);
        if (i == 0) {
            func_80066DC4(D_800D855C, 0, 0x10C, 0x22);
        } else {
            func_80066DC4(D_800D855C, i, i * 16, 0);
        }
    }

    D_800D85D0 = InitProcess(func_80055E08, 0, 0, 0);
    func_8005D900(D_800D85D0, 0x80);
}

void func_800565B4(void) {
    func_8005608C();
    func_80056380();
    func_80055EE8();
}

void func_800565E0(void) {
    func_80077044(D_800D85D8);
    func_80077044(D_800D8640);
    func_80067704(D_800D85D6);
    func_80064D38(D_800D85D4);
    EndProcess(D_800D86A8);
}

void func_80056630(void) {
    func_80067704(D_800D84E2);
    func_80064D38(D_800D84E0);
    func_80077044(D_800D84F0);
    func_80067704(D_800D84E6);
    func_80064D38(D_800D84E4);
    func_80067704(D_800D84EA);
    func_80064D38(D_800D84E8);
    EndProcess(D_800D8558);
}


void func_800566A4(void) {
    func_80077044(D_800D8568);
    func_80067704(D_800D855E);
    func_80064D38(D_800D855C);
    func_80067704(D_800D8562);
    func_80064D38(D_800D8560);
    EndProcess(D_800D85D0);
}

void func_80056700(void) {
    func_80056630();
    func_800566A4();
    func_800565E0();
}
