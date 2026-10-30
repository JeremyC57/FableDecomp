// NScript::CV_TravellingHeroesScript::CTH_SnowTroll::OnPredicateFail() -- the snow troll is gone: when the hero killed it, switch the LostBay creature generators back on and reactivate the region entrance and exit
// Leaf: ?OnPredicateFail@CTH_SnowTroll@CV_TravellingHeroesScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE0350, 131 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 5).
// Identity (proven): slot 5 (OnPredicateFail) of the RTTI-less entity vtable 0x012F42B0, which the landed
//   constructor CTH_SnowTroll::CTH_SnowTroll 0x00EE02C0 (factory 0x00EE0240) stores (site 0x00EE02EA); Ego_r.pdb has ?OnPredicateFail@CTH_SnowTroll@CV_TravellingHeroesScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Layout: Ego_r.pdb CTH_SnowTroll (entity offsets equal retail): ParentClass +0x14, PMasterData +0x18, members from +0x1c;
//   the parent level script's members sit at Ego_r offset - 0x14 (retail CScriptBase is 0x40).
// Callees: CScriptThing slot 18 MsgIsKilledBy ("" 0x0122D70E temporary); interface slots 558 SetCreatureGeneratorsEnabled
//   ("LostBay" 0x012C4744 temporary), 568 SetRegionEntranceAsActive, 567 SetRegionExitAsActive; CCharString(const char*,
//   long) 0x0099EBF0 / ~CCharString 0x0099EAE0.
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
class CScriptThing : public CBaseClass {                // 12 bytes, vtable 0x01238C8C
public:
    virtual ~CScriptThing();                            // slot 0
    virtual void t01(); virtual void t02(); virtual void t03(); virtual void t04(); virtual void t05(); virtual void t06(); virtual void t07(); virtual void t08();
    virtual void t09(); virtual void t10(); virtual void t11(); virtual void t12(); virtual void t13(); virtual void t14(); virtual void t15(); virtual void t16();
    virtual void t17();
    virtual bool MsgIsKilledBy(const CCharString& killer) const;   // slot 18 (+0x48)
    CCountedPointer<CScriptThing> PImp;                 // +0x04 Data, +0x08 Info
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase;
class CPersistContext;
class CEntityScriptBase {                               // Ego_r.pdb: vptr, Game +0x04, Me +0x08 (20 bytes)
public:
    virtual ~CEntityScriptBase();                       // slot 0
    virtual void Main() = 0;                            // slot 1
    virtual void Init();                                // slot 2
    virtual CScriptBase* GetParentScript() const = 0;   // slot 3
    virtual void OnPersist(CPersistContext& context);   // slot 4
    virtual void OnPredicateFail();                     // slot 5
    virtual void OnInterrupted();                       // slot 6
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_TravellingHeroesScript {
public:
    class CTH_SnowTroll;
};
class CV_TravellingHeroesScript::CTH_SnowTroll : public CEntityScriptBase {
public:
    CV_TravellingHeroesScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData* PMasterData;   // +0x18
    CScriptThing Entrance;                              // +0x1c
    CScriptThing Exit;                                  // +0x28
private:
    virtual void OnPredicateFail();
};

void CV_TravellingHeroesScript::CTH_SnowTroll::OnPredicateFail()
{
    if (Me.MsgIsKilledBy("")) {
        Game->SetCreatureGeneratorsEnabled("LostBay", true);
        Game->SetRegionEntranceAsActive(Entrance, true);
        Game->SetRegionExitAsActive(Exit, true);
    }
}
}