// CCreatureAction_SummonerLightningOrbInto::PerformAction() -- when the target is present and alive and the summoner has a mode manager, add a new CModeSummonerLightningOrb(owner, target)
// Leaf: ?PerformAction@CCreatureAction_SummonerLightningOrbInto@@UAEXXZ
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pCount->Info are the header CCountedPointer's (same offsets).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CB1A0, length 175 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0x31 =
// CTCCreatureModeManager), operator new @ 0x00BFEA1A (0x90 bytes), CModeSummonerLightningOrb::CModeSummonerLightningOrb(CThing*,
// CThing*) @ 0x008CC180 (vtable 0x01282BDC), CCountedPointer<CModeBase>::CCountedPointer(CModeBase*) @ 0x00718A00 (out of
// line, built in the argument area), CTCCreatureModeManager::AddMode(CCountedPointer<CModeBase>) @ 0x0073ACC0.
// Shape: the landed CCreatureAction_BriarRoseIntoInvulnerable::Initialise (0x008C1E80) without the animation prelude and with
// the target tests first: the owner argument goes through the intelligent pointer's inline `operator CThing*()` (fetched
// before the target), the `new` null test selects 0 for the counted pointer.
// Layout: owner CIntelligentPointer +8, target +0xa8.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                            // +0x04
    CBaseObject* GetPItem() const;                   // 0x00A01B50
    operator CThing*() const { return ((CThing*)GetPItem()); }
};

struct CModeBase { };
#include "engine/CCountedPointer.h"
struct CModeSummonerLightningOrb : CModeBase {
    unsigned char _pad[0x90];
    CModeSummonerLightningOrb(CThing* owner, CThing* target);   // 0x008CC180
};
class CTCCreatureModeManager { public:
    bool AddMode(CCountedPointer<CModeBase> mode);   // 0x0073ACC0
};

class CCombatActionBase {
public:
    virtual void PerformAction();
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CBaseIntelligentPointer PTarget;            // +0xa8
    unsigned char           _padb0[0x114 - 0xb0];
};

class CCreatureAction_SummonerLightningOrbInto : public CCombatActionBase {
public:
    virtual void PerformAction();
};

void CCreatureAction_SummonerLightningOrbInto::PerformAction()
{
    CTCCreatureModeManager* modeManager;
    if (PTarget.GetPItem() && !((CThing*)PTarget.GetPItem())->IsDead() && ((CThing*)PCreature.GetPItem())->GetTC(TCI_ENTITY_MODE_MANAGER, &modeManager))
        modeManager->AddMode(CCountedPointer<CModeBase>(new CModeSummonerLightningOrb(PCreature, ((CThing*)PTarget.GetPItem()))));
}