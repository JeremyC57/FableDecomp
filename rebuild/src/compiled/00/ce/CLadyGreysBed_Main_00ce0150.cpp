// CLadyGreysBed::Main() -- Lady Grey's bed entity script: wait (a script frame at a time) until the hero uses the bed, then until the hero has slept in it, then give the hero "OBJECT_SILVER_KEY"
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CE0150, 269 bytes (+ int3 padding). Entity-script slot 1 (Main).
// Callees: CCharString 0x0099EBF0 / ~CCharString 0x0099EAE0, IsDone 0x00F35B30 (on `this`, propagated label), Me's
//   CScriptThing slot 20 MsgIsUsedBy (+0x50, 0x004AAB10), CGameScriptInterface slots 7 NewScriptFrame, 50 MsgOnHeroSlept
//   (+0xc8), 121 GiveHeroObject (+0x1e4).
// Shape: two rotated `while (!X) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }` waits; the string temporary in the first loop
//   condition dies before the branch, so the negated result is materialised (`neg ; sbb ; inc` into bl); the out bool of
//   MsgOnHeroSlept is a byte in the frame's padding; the call on the Me member dispatches through its vtable.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

class CLadyGreysBed : public CEntityScriptBase {
public:
private:
        virtual void Main();   // PDB ?Main@<entity>@..@@EAEXXZ: every entity override is private (slot 1)
public:
};

void CLadyGreysBed::Main()
{
    while (!Me.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    bool slept;
    while (!Game->MsgOnHeroSlept(slept)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (!IsActiveThreadTerminating())
        Game->GiveHeroObject("OBJECT_SILVER_KEY", -1, false);
}