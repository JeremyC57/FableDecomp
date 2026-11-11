// CThingAICreature::OnCollision(ECollideType type, C3DVector* position, C3DVector* normal, CThing* other) -- base collision handling, then (both alive) a collision with the thing the creature perceives makes it directly aware of it
// Leaf: OnCollision
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00832840, 141 bytes (`ret 10h`).
// Identity: slot 41 of vtable 0x0127293C = CThingAICreature (rtti-vtables.tsv; manifest "vfunc_41"); the same slot of
//   CThingCreatureBase (0x0125AC0C) is 0x00667310 = CThingCreatureBase::OnCollision, called first with the same arguments.
// Callees: CThingCreatureBase::OnCollision @ 0x00667310, CInterfaceMap::LowerBound @ 0x0040F020,
//   CTCPerceiveThing::GetPerceivedThing @ 0x007446A0 (label "_Cons_val<...>" propagated),
//   CTCPerceiveThing::SetAsDirectlyAwareOfPerceivedThing @ 0x007449B0, CThing::GetWorld @ 0x004C7AA0 (a world getter
//   whose result is unused; label "CViewBase::GetDisplayEngine" propagated).
// Layout: retail CThing: Dead = +0x91 bit 0, interface bitset +0x20 (TC 0x34 = CTCPerceiveThing: word +0x24 bit 20),
//   CVectorMap +0x44.
// Shape: the agent160 GetTC model with the component in a named local (esi) used twice; the type test sits inside the
//   alive block.
#include "engine/C3DVector.h"
enum ECollideType { COLLIDE_NONE = 0, COLLIDE_LANDSCAPE = 1 };
class CTCBase;
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CWorld { public:
};
class CThing;
class CTCPerceiveThing { public:
    const CThing* GetPerceivedThing() const;                // 0x007446A0 Ego_r/FableWin ?GetPerceivedThing@CTCPerceiveThing@@QBEPBVCThing@@XZ (agentNF16)
    void SetAsDirectlyAwareOfPerceivedThing();  // 0x007449B0
};

class CThingCreatureBase : public CThing { public:
    void OnCollision(ECollideType type, C3DVector* position, C3DVector* normal, CThing* other);     // 0x00667310
};

class CThingAICreature : public CThingCreatureBase { public:
    void OnCollision(ECollideType type, C3DVector* position, C3DVector* normal, CThing* other);
};

void CThingAICreature::OnCollision(ECollideType type, C3DVector* position, C3DVector* normal, CThing* other)
{
    CThingCreatureBase::OnCollision(type, position, normal, other);
    if (!Dead && other && !other->IsDead()) {
        if (HasTC(TCI_PERCEIVE_THING)) {
            CTCPerceiveThing* perceive = ((CTCPerceiveThing*)&GetTC(TCI_PERCEIVE_THING));
            if (perceive->GetPerceivedThing() == other)
                perceive->SetAsDirectlyAwareOfPerceivedThing();
        }
        if (type == COLLIDE_LANDSCAPE)
            GetWorld();
    }
}