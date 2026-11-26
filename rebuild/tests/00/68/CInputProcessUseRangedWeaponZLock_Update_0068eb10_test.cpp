// Behaviour test for 0x0068EB10 CInputProcessUseRangedWeaponZLock::Update (NF478 respell): links the row object, supplies
// CPlayer::GetPControlledCreature, and checks Update asks MyPlayer (+0x18) exactly once and leaves the processed input alone.
#include <cstdio>
#include <cstring>
class CThingPlayerCreature;
static int g_calls = 0; static const void* g_this = 0;
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;
};
CThingPlayerCreature* CPlayer::GetPControlledCreature() const { ++g_calls; g_this = this; return 0; }
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#include "engine/CInputProcessUseRangedWeaponZLock.h"
static unsigned char g_process[0x40], g_input[0x40], g_copy[0x40];
static CPlayer g_player;
int main()
{
    std::memset(g_input, 0x33, sizeof(g_input)); std::memcpy(g_copy, g_input, sizeof(g_input));
    CInputProcessUseRangedWeaponZLock* p = (CInputProcessUseRangedWeaponZLock*)g_process;
    p->MyPlayer = &g_player;
    p->CInputProcessUseRangedWeaponZLock::Update(*(CProcessedInput*)g_input);
    if (g_calls != 1 || g_this != &g_player || std::memcmp(g_copy, g_input, sizeof(g_input))) { std::printf("0068eb10_TEST FAIL\n"); return 1; }
    std::printf("0068eb10_TEST PASS\n");
    return 0;
}
