// NScript::CQ_BanditCampBossBattleScript::CBanditForger::Init() -- the forger has not given his pass yet; then: no information, joins FACTION_TWINBLADE_CAMP_BANDITS, not pushable by the hero
// Leaf: ?Init@CBanditForger@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D0C390, 124 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 2).
// Identity (proven): slot 2 (Init) of the RTTI-less entity vtable 0x012C9700, which the landed
//   NScript::CQ_BanditCampBossBattleScript::CBanditForger::Alloc 0x00D0C2C0 stores (site 0x00D0C2F6); Ego_r.pdb has
//   ?Init@CBanditForger@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ.
//   Entity slot order (landed Init / OnPredicateFail rows, Ego_r.pdb CEntityScriptBase): 0 dtor, 1 Main, 2 Init,
//   3 GetParentScript, 4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted.
// Callees: script interface slots 360 SetThingHasInformation (+0x5a0), 601 EntitySetInFaction (+0x964), 844 SetIsPushableByHero (+0xd30);
//   CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 ("FACTION_TWINBLADE_CAMP_BANDITS" 0x012C8620).
// Layout: Ego_r.pdb CBanditForger: ParentClass +0x14, PMasterData +0x18, GivenPass +0x1c.
// Shape: the by-value CScriptThing argument is the inline copy (CBaseClass vtable 0x01231710, then CScriptThing's 0x01238C8C,
//   PImp copied with the count bumped) built in the outgoing argument area (family L138, landed 0x00D07410).
// Header debt: engine/CScriptThing.h flattens the vptr into a data member; the virtual calls and the by-value copy need
//   a polymorphic local model (as the landed entity-script rows use).
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
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#include "engine/CLevelScriptBase.h"
class CScriptBase;
class CPersistContext;
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_BanditCampBossBattleScript {
public:
    class CBanditForger;
};
class CQ_BanditCampBossBattleScript::CBanditForger : public CEntityScriptBase {
public:
    CQ_BanditCampBossBattleScript* ParentClass;   // +0x14
    CQ_SunnyvaleMasterData*       PMasterData;   // +0x18
    bool                          GivenPass;     // +0x1c
private:
    virtual void Init();
};

void CQ_BanditCampBossBattleScript::CBanditForger::Init()
{
    GivenPass = false;
    Game->SetThingHasInformation(Me, false, false, false);
    Game->EntitySetInFaction(Me, "FACTION_TWINBLADE_CAMP_BANDITS");
    Game->SetIsPushableByHero(Me, false);
}
}