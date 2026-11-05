// CTCLight::AttachParticles() -- when the light's thing has a particle attacher (interface 0x82) that holds no particles yet, attach them
// Leaf: ?AttachParticles@CTCLight@
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00641260, 70 bytes (+ int3 padding), plain `ret`.
// Identity: Ego_r.pdb `?AttachParticles@CTCLight@@IAEXXZ` (protected); the caller at 0x00641394 is in the CTCLight TU
// (0x00640BB0..). Catalog label "CTCUserAction_AttachParticlesIfNeeded" is descriptive.
// Callees: the thing's interface-map LowerBound @ 0x0040F020 (as the landed CThingSwitch 0x0083CA90 model),
// CTCParticleAttacher::AttachParticles() @ 0x007F8890.
// Layout: CTCBase ParentThing +4 (Ego_r.pdb); retail CThing interface bitset +0x20 (interface 0x82 = bit 2 of +0x30) and
// interface map +0x44 {first, last} of {type, component} entries (the landed 0x0083CA90 model); CTCParticleAttacher
// ParticleList vector<CIntelligentPointer<CThing>> +0x10 (Ego_r.pdb; only start/finish are read).
// Shape: the thing's inline HasTC / GetTC pair (the key a frame word, `type < it->type` folding to `cmp [eax],82h ; jle`),
// the attacher result used without a null test, `empty()` as start == finish.
class CTCBase;

#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct CIntelligentPointerThing {                       // CIntelligentPointer<CThing>, opaque here
    void* Data[2];
};

class CTCBase { public:
    void*   __vftable;                                  // +0x00
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
};

class CTCParticleAttacher : public CTCBase { public:
    void*                     PDef;                     // +0x0C CDefPointer<const CParticleAttacherDef>
    CIntelligentPointerThing* ParticleList_start;       // +0x10 vector<CIntelligentPointer<CThing>>
    CIntelligentPointerThing* ParticleList_finish;      // +0x14
    CIntelligentPointerThing* ParticleList_eos;         // +0x18
    bool ParticleListEmpty() const { return ParticleList_start == ParticleList_finish; }
    void AttachParticles();                             // 0x007F8890
};

class CTCLight : public CTCBase { public:
    void AttachParticles();
};

void CTCLight::AttachParticles()
{
    CThing* thing = ParentThing;
    if (thing->HasTC(TCI_PARTICLE_ATTACHER)) {
        CTCParticleAttacher* attacher = (CTCParticleAttacher*)((CTCParticleAttacher*)&thing->GetTC(TCI_PARTICLE_ATTACHER));
        if (attacher->ParticleListEmpty())
            attacher->AttachParticles();
    }
}