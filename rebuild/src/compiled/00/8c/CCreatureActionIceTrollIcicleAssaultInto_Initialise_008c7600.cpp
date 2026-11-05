// CCreatureAction_IceTrollIcicleAssaultInto::Initialise() -- animation "ICICLE_ASSAULT_INTO", base Initialise, bump the boss phase when the creature is a boss, then (mode manager present, target alive) push a new CModeIceTrollGroundSmashIcicleAssault(owner, target)
// Leaf: ?Initialise@CCreatureAction_IceTrollIcicleAssaultInto@@UAEXXZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C7600, 292 bytes, virtual Initialise.
// Callees: CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0 ("ICICLE_ASSAULT_INTO" 0x012817DC), SetCombatAnimation 0x008574D0,
//   CCombatActionBase::Initialise 0x00857520 (direct), CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap::LowerBound
//   0x0040F020 (inlined GetInterface: 0xAD = CTCBoss, bit 13 of +0x34; 0x31 = CTCCreatureModeManager, bit 17 of +0x24),
//   CTCBoss::IncrementBossPhase 0x007AED20, operator new 0x00BFEA1A (0x9c bytes),
//   NCreatureMode::CModeIceTrollGroundSmashIcicleAssault(CThing*, CThing*) 0x008C7C00 (landed),
//   CCountedPointer<CModeBase>(CModeBase*) 0x00718A00 (built in the argument area), CTCCreatureModeManager::AddMode 0x0073ACC0.
// Shape: the landed CCreatureAction_BriarRoseIntoInvulnerable::Initialise 0x008C1E80 model (named animation string in its
//   own block, the owner through the intelligent pointer's inline `operator CThing*()` evaluated before the target's
//   GetPItem) with the boss-phase lookup in front; the target is the combat action's +0xa8 pointer.
class CTCBase;
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingCreatureBase : public CThing { public: };

class CBaseObject;
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                            // +0x04
    CBaseObject* GetPItem() const;                   // 0x00A01B50
    operator CThing*() const { return ((CThing*)GetPItem()); }
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>
    CThingCreatureBase* GetPItem() const { return (CThingCreatureBase*)CBaseIntelligentPointer::GetPItem(); }
};

struct CModeBase { };
#include "engine/CCountedPointer.h"
struct CModeIceTrollGroundSmashIcicleAssault : CModeBase {
    unsigned char _pad[0x9c];
    CModeIceTrollGroundSmashIcicleAssault(CThing* thing, CThing* target);   // 0x008C7C00
};
class CTCBoss { public:                                // interface type 0xAD
    void IncrementBossPhase();                  // 0x007AED20
};
class CTCCreatureModeManager { public:
    bool AddMode(CCountedPointer<CModeBase> mode);   // 0x0073ACC0
};

class CCombatActionBase {
public:
    virtual void Initialise();                  // 0x00857520
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CBaseIntelligentPointer PTarget;            // +0xa8
    unsigned char           _padb0[0x114 - 0xb0];
    void SetCombatAnimation(const CCharString& name);   // 0x008574D0
};

class CCreatureAction_IceTrollIcicleAssaultInto : public CCombatActionBase {
public:
    virtual void Initialise();
};

void CCreatureAction_IceTrollIcicleAssaultInto::Initialise()
{
    {
        CCharString name("ICICLE_ASSAULT_INTO");
        SetCombatAnimation(name);
    }
    CCombatActionBase::Initialise();
    CTCBoss* boss;
    if (((CThing*)PCreature.GetPItem())->GetTC(TCI_BOSS, &boss))
        boss->IncrementBossPhase();
    CTCCreatureModeManager* modeManager;
    if (((CThing*)PCreature.GetPItem())->GetTC(TCI_ENTITY_MODE_MANAGER, &modeManager) && PTarget.GetPItem() && !((CThing*)PTarget.GetPItem())->IsDead())
        modeManager->AddMode(CCountedPointer<CModeBase>(new CModeIceTrollGroundSmashIcicleAssault(PCreature, ((CThing*)PTarget.GetPItem()))));
}