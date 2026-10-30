// NScript::CV_TempleOfLightScript::CTOL_AltarGood::Main() -- the good altar of the Temple of Light: while it exists, each time the hero talks to it switch betting on and count the bet money down
// Leaf: ?Main@CTOL_AltarGood@CV_TempleOfLightScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EDC390, 149 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 1).
// Identity (proven): slot 1 (Main) of the RTTI-less entity vtable 0x012F3EB0, which the landed
//   NScript::CV_TempleOfLightScript::CTOL_AltarGood::Alloc 0x00EDC2D0 stores (site 0x00EDC304); Ego_r.pdb has ?Main@CTOL_AltarGood@CV_TempleOfLightScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CTOL_AltarGood (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18, members from +0x1c;
//   the parent level script's members sit at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: CScriptThing slots 75 IsAlive, 27 MsgIsTalkedToBy ("SCRIPT_NAME_HERO" 0x0125D1C8 temporary); interface slots 7
//   NewScriptFrame, 868 SetBettingActive, 874 SetCountBetMoneyDown; CCharString(const char*, long) 0x0099EBF0 /
//   ~CCharString 0x0099EAE0; IsActiveThreadTerminating 0x00F35B30 (the trailing check's result is unused).
class CCharString {
public:
    void* PData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CCPPointerInfo {
public:
    unsigned long RefCount;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase;
class CPersistContext;
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_TempleOfLightScript {
public:
    class CTOL_AltarGood;
};
class CV_TempleOfLightScript::CTOL_AltarGood : public CEntityScriptBase {
public:
    CV_TempleOfLightScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData* PMasterData;   // +0x18

private:
    virtual void Main();
};

void CV_TempleOfLightScript::CTOL_AltarGood::Main()
{
    while (Me.IsAlive()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            Game->SetBettingActive(true);
            Game->SetCountBetMoneyDown(true);
        }
    }
    if (IsActiveThreadTerminating())
        return;
}
}