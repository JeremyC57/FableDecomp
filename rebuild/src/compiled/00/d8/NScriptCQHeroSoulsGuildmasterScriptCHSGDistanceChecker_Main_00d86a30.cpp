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
class CGameScriptInterfaceBase { public:                // the script interface (slots: ghidra_out/cgsi_slots_aligned.tsv, CGameScriptInterface vtable 0x01260F0C)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual CScriptThing& GetHero();   // slot 70 (+0x118)
};
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
class CScriptBase {                                     // retail 0x40 bytes
public:
    virtual ~CScriptBase();
    unsigned char _body[0x3c];                          // +0x04 .. +0x40
};
template <class TData> class CLevelScriptBase : public CScriptBase {
public:
    CGameScriptInterfaceBase* Game;                     // +0x40
    TData* MasterScriptData;                            // +0x44
};
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