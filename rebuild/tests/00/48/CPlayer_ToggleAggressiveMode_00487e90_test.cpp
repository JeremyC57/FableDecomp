#include <stdio.h>

unsigned char g_aggressiveEnabled = 1;

struct CPlayer {
    char pad[0x217];
    unsigned char aggressive;
};

int g_tailCalls = 0;

extern "C" void __fastcall CPlayer_OnAggressiveChanged(CPlayer* player)
{
    g_tailCalls++;
}

void __fastcall ToggleAggressiveMode(CPlayer* player)
{
    if (g_aggressiveEnabled) {
        player->aggressive = (player->aggressive == 0);
        CPlayer_OnAggressiveChanged(player);
    }
}

static void callToggle(CPlayer* p)
{
    __asm {
        mov ecx, p
        call ToggleAggressiveMode
    }
}

int main()
{
    CPlayer p;
    p.aggressive = 0;

    // global enabled: should toggle 0 -> 1 and tail-call
    g_aggressiveEnabled = 1;
    callToggle(&p);
    if (p.aggressive != 1) { printf("FAIL toggle1 %d\n", p.aggressive); return 1; }
    if (g_tailCalls != 1) { printf("FAIL tail1 %d\n", g_tailCalls); return 1; }

    // toggle again 1 -> 0
    callToggle(&p);
    if (p.aggressive != 0) { printf("FAIL toggle2 %d\n", p.aggressive); return 1; }
    if (g_tailCalls != 2) { printf("FAIL tail2 %d\n", g_tailCalls); return 1; }

    // global disabled: no change, no tail-call
    g_aggressiveEnabled = 0;
    p.aggressive = 1;
    callToggle(&p);
    if (p.aggressive != 1) { printf("FAIL noop %d\n", p.aggressive); return 1; }
    if (g_tailCalls != 2) { printf("FAIL notail %d\n", g_tailCalls); return 1; }

    printf("OK_0x00487e90\n");
    return 0;
}