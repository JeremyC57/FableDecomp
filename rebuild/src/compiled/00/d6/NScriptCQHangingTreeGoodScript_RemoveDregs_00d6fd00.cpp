// CQ_HangingTreeGoodScript::RemoveDregs() -- empty CurrentEnemyVector from the back: every enemy still alive is faded out and killed (0.5 s), then the element is popped
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D6FD00, length 101 bytes (padded to 112).
// Callees: CScriptThing::IsAlive (virtual slot 75, +0x12c; 0x004AB130), FadeOutAndKillEntity @ 0x00CBE716 (`__fastcall`
// (thing in ecx, the first bool in dl, float and second bool on the stack)), CScriptThing's destructor through vtable
// slot 0 with 0 (the STLport pop_back `_Destroy`).
// Layout (retail = Ego_r PDB offsets -0x14; the retail STLport vector is 12 bytes): CurrentEnemyVector +0xa0 (_M_start)
// / +0xa4 (_M_finish).
// Shape: `while (!empty())` with the inline back() (`*(end() - 1)`, re-read for every use) and pop_back
// (`--_M_finish; _M_finish->~CScriptThing();`, the explicit virtual destructor call passes 0).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
void __fastcall FadeOutAndKillEntity(const CScriptThing& thing, float time, bool kill, bool removeOnFade);   // 0x00CBE716
template <class T> struct CRetailVector {          // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    bool empty() const { return _M_start == _M_finish; }
    T& back() { return *(_M_finish - 1); }
    void pop_back() { --_M_finish; _M_finish->~T(); }
};
namespace NScript {
class CQ_HangingTreeGoodScript {
public:
    unsigned char              _pad_0x00[0xa0];
    CRetailVector<CScriptThing> CurrentEnemyVector;    // +0xa0
    void RemoveDregs();
};
} // namespace NScript
using NScript::CQ_HangingTreeGoodScript;
void NScript::CQ_HangingTreeGoodScript::RemoveDregs()
{
    while (!CurrentEnemyVector.empty()) {
        if (CurrentEnemyVector.back().IsAlive())
            FadeOutAndKillEntity(CurrentEnemyVector.back(), 0.5f, true, true);
        CurrentEnemyVector.pop_back();
    }
}