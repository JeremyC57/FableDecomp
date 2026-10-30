// NScript::CQ_HeroSoulsGuildmasterScript::CHSG_DistanceChecker::Main() -- the Guildmaster-woods distance checker: wait until the hero is within HS_GMLongDistance of Me and set the quest's DistanceReached to 1, then wait until he is within HS_GMShortDistance and set it to 2
// Leaf: ?Main@CHSG_DistanceChecker@CQ_HeroSoulsGuildmasterScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D86A30, 258 bytes, thiscall, plain `ret`.
// Identity (proven): the row is referenced once, from slot 1 (Main) of the entity-script vtable 0x012D4C04, which the landed
//   NScript::CQ_HeroSoulsGuildmasterScript::CHSG_DistanceChecker::Alloc 0x00D86980 stores (slot 0 = scalar deleting dtor);
//   Ego_r.pdb ?Main@CHSG_DistanceChecker@CQ_HeroSoulsGuildmasterScript@NScript@@EAEXXZ (0x00DAA290, right before its Alloc
//   0x00DAA3A0, the same pair order as retail). Catalog label sub_00d86a30 (gap scan).
// Callees: IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall, Ego_r), CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30
//   (Ego_r `QBE`); CGameScriptInterface slots 7 NewScriptFrame, 70 GetHero.
// Data: pGlobalScriptDef 0x0143E90C (Ego_r `pGlobalScriptDef`). Retail CScriptDef +0xc90 / +0xc94 = Ego_r HS_GMShortDistance
//   0xd40 / HS_GMLongDistance 0xd44: the Ego_r layout is 0xb0 larger here (0xa8 before FHPatterns, as the landed FireHeart rows'
//   FHPadRadius 0xcb8 -> 0xc10, plus 4 for each of the two STLport vectors FHPatterns / FHTimeLimit; OVBRBanditSightRadius
//   0xe0c -> 0xd5c confirms 0xb0 past them). The landed CWaitingGuardInWoods::Main 0x00D86680 reads +0xc94 / +0xc98 the same way.
// Layout: Ego_r CHSG_DistanceChecker (0x1c = the Alloc's new size): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14,
//   PMasterData +0x18. CQ_HeroSoulsGuildmasterScript at Ego_r - 0x14 (as the landed siblings 0x00D86150 / 0x00D86680):
//   DistanceReached +0x54.
// Shape: two rotated script waits (`while (!cond) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }` then a
//   termination test); the long range is converted with fild and stored straight into the float argument slot.
class CCharString;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
template <class T> class CCountedPointer {
public:
    T* Data;
    void* Info;
};
class CScriptThing : public CBaseClass {                // 12 bytes, vtable 0x01238C8C
public:
    CCountedPointer<CScriptThing> PImp;                 // +0x04
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
class CScriptDef {                                      // retail layout (Ego_r 0x1154 with Dinkumware containers)
public:
    unsigned char _before[0xc90];
    long HS_GMShortDistance;                            // +0xc90 (Ego_r 0xd40)
    long HS_GMLongDistance;                             // +0xc94 (Ego_r 0xd44)
    long HS_GMGuardDistance;                            // +0xc98 (Ego_r 0xd48)
};
extern CScriptDef* pGlobalScriptDef;                    // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`)
class CEntityScriptBase {                               // retail 0x14 bytes
public:
    virtual ~CEntityScriptBase();
    CGameScriptInterfaceBase* Game;                     // +0x04
    CScriptThing Me;                                    // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
};
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_HeroSoulsGuildmasterScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool PlayedBriarCS;                                 // +0x48
    bool PlayedGuildMasterCS;                           // +0x49
    bool PlayedGuildCS;                                 // +0x4a
    bool OutsideWoodGuardsAttack;                       // +0x4b
    bool GiveWarningCutscene;                           // +0x4c
    long GuardInWoodsCount;                             // +0x50
    long DistanceReached;                               // +0x54
    class CHSG_DistanceChecker : public CEntityScriptBase {
    public:
        CQ_HeroSoulsGuildmasterScript* ParentClass;     // +0x14
        CQ_SunnyvaleMasterData* PMasterData;            // +0x18
    private:
        virtual void Main();                            // slot 1
    };
};

void CQ_HeroSoulsGuildmasterScript::CHSG_DistanceChecker::Main()
{
    while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), (float)pGlobalScriptDef->HS_GMLongDistance)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    ParentClass->DistanceReached = 1;
    while (!IsDistanceBetweenThingsUnder(Me, Game->GetHero(), (float)pGlobalScriptDef->HS_GMShortDistance)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    ParentClass->DistanceReached = 2;
}
}   // namespace NScript