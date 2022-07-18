#include "common.h"

extern GameStatus D_800ED5C0;
extern s16 D_800F9910[];

extern board_overlay_entrypoint D_800F9890[];

extern mpSource_f2b7cstruct *D_800F2B7C;

typedef struct mystery_struct_ret_func_80048224 {
    struct mpSource_object* unk0;
    s8 pad[4];
    s16 unk8; // window id
} mystery_struct_ret_func_80048224;

#define DK_STAR_COUNT 7
#define DK_BOO_COUNT 2
#define DK_COIN_GATE_COUNT 2
#define DK_THWOMP_COUNT 3

extern s16 D_800F98D0[]; //ov054_func_800F663C_data0
    //3, 5, 6, 0, 1, 2, 4

extern s16 D_800F98E0[]; //ov054_func_800F663C_data1
    //0, 0, 0, 1, 1, 1, 3

extern s16 D_800F98F0[]; //ov054_data_mystery_40s
    //0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C

extern s16 D_800F9900[]; //ov054_star_space_indices
    //0x77, 0x80, 0x7F, 0x84, 0x83, 0x75, 0x76

extern s16 D_800F9910[]; //ov054_toad_space_indices
    //0x61, 0x6E, 0x6D, 0x72, 0x71, 0x5F, 0x60

extern s16 D_800F9920[]; //ov054_data_star_related_800F9920
    //0, 1, 7, 3

extern s16 D_800F9928[]; //ov054_toad_space_indices_repeat
    //0x61, 0x6E, 0x6D, 0x72, 0x71, 0x5F, 0x60

extern s16 D_800F9938[]; //ov054_data_mystery_40s_2
    //0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C
    
// board_overlay_entrypoint D_800F9890[] = {
//     {0, &func_800F6F0C_2421CC },
//     {1, &func_800F6F44_242204 },
//     {2, &func_800F7190_242450 },
//     {3, &func_800F7224_2424E4 },
//     {4, &func_800F983C_244AFC },
//     {-1, 0},
// };