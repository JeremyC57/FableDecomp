// CQ_OakValeBanditRaidScript::CanThingSeeAnythingInVector(const CScriptThing& thing, std::vector<CScriptThing>& things) -- index of the first element of `things` that `thing` can see (CanThingBe_Seen_ByOtherThing(element, thing)), or -1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DC2170, length 128 bytes, `ret 8`.
// Callees: CGameScriptInterface::CanThingBe_Seen_ByOtherThing (virtual slot 632, +0x9e0; 0x0089CD10) on the script's
// Game (+0x40, re-read from the saved `this` every pass).
// Layout: STLport vector<CScriptThing> (12-byte elements; _M_start, _M_finish).
// Shape: `for (unsigned long i = 0; i < things.size(); ++i)` with size() as the signed pointer difference / 12 (imul by
// 0x2AAAAAAB) recomputed every pass, and the element as `*(begin() + i)` (the STLport operator[]; the offset is
// strength-reduced into ebp while begin() is re-read).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CScriptThing { public:
    unsigned char _body[12];
};
template <class T> struct CRetailVector {          // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) { return *(begin() + n); }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CQ_OakValeBanditRaidScript {
public:
    unsigned char         _pad_0x00[0x40];
    CGameScriptInterface* Game;                    // +0x40
    long CanThingSeeAnythingInVector(const CScriptThing& thing, CRetailVector<CScriptThing>& things);
};
} // namespace NScript
using NScript::CQ_OakValeBanditRaidScript;
long NScript::CQ_OakValeBanditRaidScript::CanThingSeeAnythingInVector(const CScriptThing& thing, CRetailVector<CScriptThing>& things)
{
    for (unsigned long i = 0; i < things.size(); ++i) {
        if (Game->CanThingBe_Seen_ByOtherThing(things[i], thing))
            return i;
    }
    return -1;
}