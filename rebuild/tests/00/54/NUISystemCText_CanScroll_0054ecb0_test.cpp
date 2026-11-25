// Behaviour test for 0x0054ECB0 NUISystem::CText::CanScroll: how far the text overhangs the bounds (below when scrolling down, above
// when scrolling up), else 0.0 (retail's pooled 0.0f, 0x0122DEDC).
// NF420 (2026-11-25): LINKED test -- it links the row object (the row function is declared here, not defined); callees are defined
// here under the row spellings (or as thiscall-compatible __fastcall fixtures), so the test observes the float literal the row uses:
// a mutated literal (0.0f -> 0.5f) changes the result and fails.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" void __cdecl NF420TrapHit(const char* sym) { printf("FAIL unexpected call %s\n", sym); exit(3); }
extern "C" char NF420TrapData[4096] = { 0 };
class C2DVector { public: float X, Y; C2DVector() { } };
class C2DBoxF { public: float TLX, TLY, BRX, BRY; };
namespace NUISystem {
class CManager { public: C2DVector GetUIScale() const; static CManager* __fastcall GetInstance(); };
class CText { public: float CanScroll(bool down, const C2DBoxF& bounds); };
}
static double g_mgr[1]; static int g_refresh = 0; static float g_posY = 0.0f;
NUISystem::CManager* __fastcall NUISystem::CManager::GetInstance() { return (CManager*)g_mgr; }
C2DVector NUISystem::CManager::GetUIScale() const { C2DVector v; v.X = 1.0f; v.Y = 2.0f; return v; }
// CText vtable slots 112 GetPosition (C2DVector by hidden pointer) and 135 (refresh before measuring)
extern "C" void* __fastcall NF420_GetPosition(void*, int, C2DVector* ret) { ret->X = 0.0f; ret->Y = g_posY; return ret; }
extern "C" void __fastcall NF420_Refresh(void*, int, void* context) { if (context != (void*)0x5150) exit(5); ++g_refresh; }
extern "C" void __cdecl NF420_VirtualTrap() { NF420TrapHit("CText virtual"); }
// Row-object externals this test does not model (generated): a call reaches a trap that prints FAIL and exits; a data
// symbol reads a zeroed buffer.
// (none)
int main()
{
    static void* vtbl[136]; for (int i = 0; i < 136; ++i) vtbl[i] = (void*)NF420_VirtualTrap;
    vtbl[112] = (void*)NF420_GetPosition; vtbl[135] = (void*)NF420_Refresh;
    static unsigned char obj[0x150]; *(void**)obj = vtbl; *(float*)(obj + 0xd0) = 10.0f; *(void**)(obj + 0x148) = (void*)0x5150;
    NUISystem::CText* text = (NUISystem::CText*)obj;
    C2DBoxF b; b.TLX = 0.0f; b.TLY = 5.0f; b.BRX = 100.0f; b.BRY = 50.0f;
    g_posY = 40.0f; float down = text->CanScroll(true, b);       // 10 * 2 + 40 = 60 > 50: -10
    g_posY = 5.0f; float none = text->CanScroll(false, b);       // at the top: nothing to scroll
    g_posY = 2.0f; float up = text->CanScroll(false, b);         // 3 above the top
    g_posY = 10.0f; float none2 = text->CanScroll(true, b);      // 30 < 50
    bool ok = down == -10.0f && none == 0.0f && up == 3.0f && none2 == 0.0f && g_refresh == 4;
    if (ok) printf("0054ecb0_TEST PASS\n");
    else printf("FAIL down=%f none=%f up=%f none2=%f\n", down, none, up, none2);
    return ok ? 0 : 1;
}
