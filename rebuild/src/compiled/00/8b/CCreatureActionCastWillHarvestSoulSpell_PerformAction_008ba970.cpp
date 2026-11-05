// CCreatureAction_CastWillHarvestSoulSpell::PerformAction() -- if the target is alive, kill it and tell the caster's special-abilities component (interface 0xa7) that a soul was harvested
// Leaf: ?PerformAction@CCreatureAction_CastWillHarvestSoulSpell@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008BA970, length 138 bytes, plain `ret`. Vtable slot of
//   CCreatureAction_CastWillHarvestSoulSpell (referenced from 0x0127D2C0).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::Kill(bool) @ 0x004C9B80, CVectorMap::LowerBound
//          @ 0x0040F020, CTCSpecialAbilities::OnSoulHarvested @ 0x0072A840 (an empty out-of-line method in retail;
//          the manifest label "$E2" is propagated, the name is descriptive).
// Shape: PTarget's address is taken once (esi); IS_THING_ALIVE inlined (null test, dead bit +0x91); Kill(true) on
//   the target; then the out-parameter GetInterface template on PCreature (+8): bit 0xa7 is bit 7 of the presence
//   dword +0x34 (`mov cl,[eax+34h] ; test cl,cl ; jns`), the out pointer is NOT initialised (the absent path reads
//   the dead frame word, landed 0x007504E0 shape) and the component call is tail-duplicated into both paths.
// Layout: CCreatureActionBase (retail 0xa8 bytes, PCreature at +8) followed by the target pointer at +0xa8; the class
//   is not in Ego_r.pdb and has no engine header.
class CTCBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.Get() != 0) {
        if (!p.Get()->IsDead())
            return true;
    }
    return false;
}

class CTCSpecialAbilities { public:                // interface type 0xa7 component
    void OnSoulHarvested();                 // 0x0072A840
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual void PerformAction();
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];
};

class CCreatureAction_CastWillHarvestSoulSpell : public CCreatureActionBase { public:
    CIntelligentPointer<CThing> PTarget;        // +0xa8 CIntelligentPointer<CThing>
    virtual void PerformAction();
};

void CCreatureAction_CastWillHarvestSoulSpell::PerformAction()
{
    if (IS_THING_ALIVE(PTarget)) {
        PTarget.Get()->Kill(true);
        CTCSpecialAbilities* abilities;
        PCreature.Get()->GetTC(TCI_SPECIAL_ABILITIES, &abilities);
        abilities->OnSoulHarvested();
    }
}