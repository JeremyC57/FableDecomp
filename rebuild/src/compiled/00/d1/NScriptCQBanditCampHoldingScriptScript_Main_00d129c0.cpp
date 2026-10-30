// CQ_BanditCampHoldingScriptScript::Main() -- once per save, wait for the BanditCampPath_1 level and record PathReached; then wait for the BanditCampPathEntrance region and open the OakValeBanditRaidGate door if it is shut
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D129C0, 416 bytes, plain `ret` (vtable slot at 0x012C9E1C).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 ("BanditCampPath_1" 0x012C85D8,
//   "BanditCampPathEntrance" 0x012C83D0, "OakValeBanditRaidGate" 0x012C9E28), CScriptBase::IsActiveThreadTerminating
//   0x00CB7940, ~CScriptThing 0x004AA840; CGameScriptInterface (+0x40) slots 7 NewScriptFrame, 11 IsLevelLoaded
//   (+0x2c), 12 IsRegionLoaded (+0x30), 72 GetThingWithScriptName (+0x120), 425 OpenDoor (+0x6a4); the thing's
//   implementation slot 64 IsOpenDoor (+0x100) behind the inlined null-safe wrapper.
// Shape: WAIT_UNTIL(x) is `while (!x) { NewScriptFrame(); if (terminating) return; }` (neg/sbb/inc on the bool), as
//   the landed CV_SingingStonesScript::WatchForRegionLeaving 0x00ED2A50.
// Layout (retail = Ego_r.pdb - 0x14): PathReached +0x48.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
void operator delete(void* p);                              // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:                                     // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);              // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct CScriptThingImpl {
    virtual void i000(); virtual void i001(); virtual void i002(); virtual void i003(); virtual void i004(); virtual void i005(); virtual void i006(); virtual void i007();
    virtual void i008(); virtual void i009(); virtual void i010(); virtual void i011(); virtual void i012(); virtual void i013(); virtual void i014(); virtual void i015();
    virtual void i016(); virtual void i017(); virtual void i018(); virtual void i019(); virtual void i020(); virtual void i021(); virtual void i022(); virtual void i023();
    virtual void i024(); virtual void i025(); virtual void i026(); virtual void i027(); virtual void i028(); virtual void i029(); virtual void i030(); virtual void i031();
    virtual void i032(); virtual void i033(); virtual void i034(); virtual void i035(); virtual void i036(); virtual void i037(); virtual void i038(); virtual void i039();
    virtual void i040(); virtual void i041(); virtual void i042(); virtual void i043(); virtual void i044(); virtual void i045(); virtual void i046(); virtual void i047();
    virtual void i048(); virtual void i049(); virtual void i050(); virtual void i051(); virtual void i052(); virtual void i053(); virtual void i054(); virtual void i055();
    virtual void i056(); virtual void i057(); virtual void i058(); virtual void i059(); virtual void i060(); virtual void i061(); virtual void i062(); virtual void i063();
    virtual bool IsOpenDoor() const;   // slot 64 (+0x100)
};
class CScriptThing : public CBaseClass { public:                          // vtable 0x01238C8C, 12 bytes
    CCountedPointer<CScriptThingImpl> PImp;                 // +0x04 Data, +0x08 Info
    CScriptThing(const CScriptThing& other);                // 0x004ABE90
    ~CScriptThing();                                        // 0x004AA840
    bool IsOpenDoor() const                                 // 0x004AB000
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->IsOpenDoor();
    }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct CQuestScriptBase : CScriptBase {
    CGameScriptInterface* Game;                             // +0x40
    void*                 Data;                             // +0x44
};
namespace NScript {
class CQ_BanditCampHoldingScriptScript : public CQuestScriptBase {
public:
    bool PathReached;                                       // +0x48
    void Main();
};
} // namespace NScript
using NScript::CQ_BanditCampHoldingScriptScript;

void NScript::CQ_BanditCampHoldingScriptScript::Main()
{
    if (!PathReached) {
        if (IsActiveThreadTerminating())
            return;
        while (!Game->IsLevelLoaded("BanditCampPath_1")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        PathReached = true;
    }
    while (!Game->IsRegionLoaded("BanditCampPathEntrance")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing gate = Game->GetThingWithScriptName("OakValeBanditRaidGate");
    if (!gate.IsOpenDoor()) {
        if (IsActiveThreadTerminating())
            return;
        Game->OpenDoor(gate);
    }
}