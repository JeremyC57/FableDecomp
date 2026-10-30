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
class CScriptThing { public:                                   // 12 bytes, vtable 0x01238C8C
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual bool MsgIsUsedBy(const CCharString& user) const;   // slot 20 (+0x50) 0x004AAB10
    unsigned char _body[8];
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

class CLadyGreysBed : public CEntityScriptBase {
public:
    void*                 __vftable;          // +0x00
    CGameScriptInterface* Game;               // +0x04
    CScriptThing          Me;                 // +0x08
    void Main();
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