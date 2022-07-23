#include "common.h"
#include "ovl_71.h"

static board_overlay_entrypoint D_800F6690[] = {
{0, &func_800F6610_2F6FE0},
{1, &func_800F6668_2F7038},
{-1, 0},
};

void func_800F65E0_2F6FB0(void) {
    ExecBoardScene(&D_800F6690, D_800C597A);
}
