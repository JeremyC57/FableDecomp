// Behaviour test for 0x00895A10 CGameScriptInterface::GetBestFishWeight: the hero's inventory's best weight for the named fish def,
// or 0.0 (retail's pooled 0.0f, 0x0122DEDC) without a living hero / inventory / def.
// NF420 (2026-11-25): LINKED test -- it links the row object (the row function is declared here, not defined); callees are defined
// here under the row spellings (or as thiscall-compatible __fastcall fixtures), so the test observes the float literal the row uses:
// a mutated literal (0.0f -> 0.5f) changes the result and fails.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" void __cdecl NF420TrapHit(const char* sym) { printf("FAIL unexpected call %s\n", sym); exit(3); }
extern "C" char NF420TrapData[4096] = { 0 };
static void* g_creature = 0; static double g_player[1];
extern "C" void* __fastcall NF420_GetMainPlayer(void*, int) { return g_player; }
extern "C" void* __fastcall NF420_GetPControlledCreature(void*, int) { return g_creature; }
#pragma comment(linker, "/alternatename:?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ=@NF420_GetMainPlayer@8")
#pragma comment(linker, "/alternatename:?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ=@NF420_GetPControlledCreature@8")
extern "C" float __fastcall NF420_Row(void* self, int edx, const void* defName);
#pragma comment(linker, "/alternatename:@NF420_Row@12=?GetBestFishWeight@CGameScriptInterface@@UBEMABVCCharString@@@Z")   // the row function, called through the shim
// Row-object externals this test does not model (generated): a call reaches a trap that prints FAIL and exits; a data
// symbol reads a zeroed buffer.
extern "C" void __cdecl NF420Trap0() { NF420TrapHit("?Get@CGameDefinitionManager@@SIAAV1@XZ"); }
#pragma comment(linker, "/alternatename:?Get@CGameDefinitionManager@@SIAAV1@XZ=_NF420Trap0")
extern "C" void __cdecl NF420Trap1() { NF420TrapHit("?GetBestFishWeight@CTCInventory_Retail@@QAEMJ@Z"); }
#pragma comment(linker, "/alternatename:?GetBestFishWeight@CTCInventory_Retail@@QAEMJ@Z=_NF420Trap1")
extern "C" void __cdecl NF420Trap2() { NF420TrapHit("?GetDefGlobalIndexFromName@CDefinitionManager@@QBEJABVCCharString@@@Z"); }
#pragma comment(linker, "/alternatename:?GetDefGlobalIndexFromName@CDefinitionManager@@QBEJABVCCharString@@@Z=_NF420Trap2")
int main()
{
    static unsigned char gsi[0x18]; *(void**)(gsi + 0x14) = (void*)0x1414;
    float none = NF420_Row(gsi, 0, 0);                                   // no controlled creature
    static unsigned char hero[0x98]; hero[0x91] = 1; g_creature = hero;  // a dead hero
    float dead = NF420_Row(gsi, 0, 0);
    hero[0x91] = 0;                                                      // alive, but no inventory interface (bit 0x11 clear)
    float noInv = NF420_Row(gsi, 0, 0);
    bool ok = none == 0.0f && dead == 0.0f && noInv == 0.0f;
    if (ok) printf("00895a10_TEST PASS\n");
    else printf("FAIL none=%f dead=%f noInv=%f\n", none, dead, noInv);
    return ok ? 0 : 1;
}
