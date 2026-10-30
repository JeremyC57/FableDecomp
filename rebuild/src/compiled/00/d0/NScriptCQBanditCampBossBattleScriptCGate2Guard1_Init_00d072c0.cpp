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
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them inline or implicit): defined inline after the header.
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
class CScriptBase;
class CPersistContext;
// CEntityScriptBase: the retail model from engine/CEntityScriptBase.h (vtable 0x012C3224, 7 slots, 0x14: Game +0x04, Me +0x08).
#include "engine/CEntityScriptBase.h"

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