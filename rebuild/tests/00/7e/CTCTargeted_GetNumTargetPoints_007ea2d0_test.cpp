// Behaviour test for 0x007EA2D0 CTCTargeted::GetNumTargetPoints (NF415: linked): links the row object (the function is declared here,
// not defined) and defines its callee under the row's spelling. With no object at +4 -> +0x64 it returns 0; otherwise it returns the
// object's answer for (TARGETING_CURSOR_POSITION_NAME (0x013BAC50), 1.0f).
#include <cstdio>

class CCharString { public: int Tag; };
extern const CCharString TARGETING_CURSOR_POSITION_NAME = { 1 };      // the row's data symbol, defined here
extern const CCharString ACTION_PAUSE_ON_THIS_FRAME = { 2 };          // a decoy (the mutant control passes this one)
struct Inner {
    char pad[0x64];
    Inner* sub; // +0x64
};
// Naming (NF442, ruling q2): the row now calls the callee by its PDB spelling, so this double is respelt with it (callee only).
namespace NGraphicAppearance { class CTCGraphicAppearance { public: long GetNoHelperPoints(CCharString const&, float) const; }; }
class CTCTargeted { public:
    void* v;      // +0
    Inner* obj;   // +4
};
long __fastcall CTCTargeted_GetNumTargetPoints(CTCTargeted* targeted);   // the row (linked)

static long g_ret = 0; static void* g_self = 0; static void* g_p = 0; static float g_f = 0.0f; static int g_calls = 0;
long NGraphicAppearance::CTCGraphicAppearance::GetNoHelperPoints(CCharString const& p, float f) const { ++g_calls; g_self = const_cast<NGraphicAppearance::CTCGraphicAppearance*>(this); g_p = const_cast<CCharString*>(&p); g_f = f; return g_ret; }

int main()
{
    Inner outer; Inner inner; outer.sub = 0;
    CTCTargeted t; t.v = 0; t.obj = &outer;
    bool ok = CTCTargeted_GetNumTargetPoints(&t) == 0 && g_calls == 0;
    outer.sub = &inner; g_ret = 7;
    ok = ok && CTCTargeted_GetNumTargetPoints(&t) == 7 && g_calls == 1 && g_self == &inner
         && g_p == (void*)&TARGETING_CURSOR_POSITION_NAME && g_f == 1.0f;
    std::printf(ok ? "PASS_GNTP_OK\n" : "FAIL_GNTP\n");
    return ok ? 0 : 1;
}
