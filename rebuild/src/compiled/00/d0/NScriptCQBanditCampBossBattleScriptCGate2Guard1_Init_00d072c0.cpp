// NScript::CQ_BanditCampBossBattleScript::CGate2Guard1::Init() -- reset the guard's AI state, then: no information, joins FACTION_TWINBLADE_CAMP_BANDITS, not pushable by the hero
// Leaf: ?Init@CGate2Guard1@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D072C0, 127 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 2).
// Identity (proven): slot 2 (Init) of the RTTI-less entity vtable 0x012C8E80, which the landed
//   CQ_BanditCampBossBattleScript::CGate2Guard1::Alloc 0x00D071F0 stores (site 0x00D07226); Ego_r.pdb has
//   ?Init@CGate2Guard1@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ. Same body as the landed CGate3Guard::Init 0x00D07410
//   (family L138) plus the AIState reset.
// Callees: the game script interface slots 360 SetThingHasInformation, 601 EntitySetInFaction and 844 SetIsPushableByHero
//   (by-value CScriptThing: the inline copy stores the CBaseClass vtable 0x01231710, then CScriptThing's 0x01238C8C, and
//   bumps the PImp count); CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 ("FACTION_TWINBLADE_CAMP_BANDITS" 0x012C8620).
// Layout: Ego_r.pdb CGate2Guard1: ParentClass +0x14, PMasterData +0x18, AIState +0x1c.
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
    CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
    virtual ~CScriptThing();                            // slot 0
    CCountedPointer<CScriptThing> PImp;                 // +0x04 Data, +0x08 Info
};
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
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
    CGameScriptInterfaceBase* Game;                     // +0x04
    CScriptThing              Me;                       // +0x08
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_BanditCampBossBattleScript {
public:
    class CGate2Guard1;
};
class CQ_BanditCampBossBattleScript::CGate2Guard1 : public CEntityScriptBase {
public:
    CQ_BanditCampBossBattleScript* ParentClass;         // +0x14
    CQ_SunnyvaleMasterData*        PMasterData;         // +0x18
    long                           AIState;             // +0x1c
private:
    virtual void Init();
};

void CQ_BanditCampBossBattleScript::CGate2Guard1::Init()
{
    AIState = 0;
    Game->SetThingHasInformation(Me, false, true, false);
    Game->EntitySetInFaction(Me, "FACTION_TWINBLADE_CAMP_BANDITS");
    Game->SetIsPushableByHero(Me, false);
}
}