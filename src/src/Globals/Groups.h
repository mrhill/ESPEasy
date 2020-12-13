#pragma once

#include <stdint.h>

struct GroupInfo
{
    char name[32] = {0};
};

extern uint8_t Groups[512]; // 0..255: M0, 256..511: M1

extern GroupInfo GroupInfos[8];
