// CTCActivationReceptorCreatureGenerator::Activate() -- the base receptor Activate, then the thing's creature generator (interface 0x3d) is either externally triggered (TriggerOnActivate) or switched active
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F7650, length 94 bytes.
// Leaf: ?Activate@CTCActivationReceptorCreatureGenerator
// Callees: CTCActivationReceptorBase::Activate @ 0x007F76E0, CVectorMap::LowerBound @ 0x0040F020, CTCCreatureGenerator::
// SetAsActive(bool) @ 0x00749DD0. Ego_r.pdb: `?Activate@CTCActivationReceptorCreatureGenerator@@UAEXXZ` (slot 28),
// TriggerOnActivate +0x2c / ActivateOnActivate +0x2d in retail (PDB +0x30/+0x31, the 12-byte vector shift);
// CTCCreatureGenerator::ExternalTrigger is +0x4f in retail (PDB +0x5f: CArray, list and vector are narrower in retail,
// so engine/CTCCreatureGenerator.h is not used). Interface 0x3d = word 1, bit 29 (`test [eax+24h],20000000h`).
// Shape: the landed out-parameter GetInterface template (0x007504E0 / 0x0069F750 family): the presence bitset word
// test, LowerBound on the +0x44 map with the key in the `push ecx` frame word, end-or-larger-key -> the end entry, the
// component left in eax. The out pointer is NOT initialised: on the absent path retail reads the uninitialised local
// (`mov eax,[esp+N]`) and still makes the call, which the same source reproduces.
// Layout (supersedes any local-model note above): engine/CTCCreatureGenerator.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CTCCreatureGenerator CTCCreatureGeneratorDonor
#include "engine/CTCCreatureGenerator.h"
#undef CTCCreatureGenerator
struct CTCBaseInterface;
struct CInterfaceMap {                         // a thing's sorted {type, component} vector map at +0x44
    struct SEntry { long type; CTCBaseInterface* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);      // 0x0040F020
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];            // +0x20 std::bitset<274> UsingComponents
    CInterfaceMap m_interfaces;                // +0x44 Components
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

class CTCBase { public:                               // engine/CTCBase.h layout, modelled polymorphic
    virtual void v00();
    CThing* ParentThing;                       // +0x04
    bool    ToBeRemoved;                       // +0x08
    bool    IsInGlobalUpdateTCs;               // +0x09
};

class CTCActivationReceptorBase : public CTCBase { public:   // retail 0x2c bytes (see 0x007F7710; engine header has the PDB layout)
    unsigned char _body[0x20];                 // +0x0c Activated .. +0x28 FrameAtLastActivation
    virtual void Activate();                   // slot 28: 0x007F76E0
    virtual void Deactivate();                 // slot 29: 0x007F7700
};

class CTCCreatureGenerator : public CTCCreatureGeneratorDonor {
 public:
    void SetAsActive(bool active);             // 0x00749DD0
};

struct CTCActivationReceptorCreatureGenerator : CTCActivationReceptorBase {
    bool TriggerOnActivate;                    // +0x2c
    bool ActivateOnActivate;                   // +0x2d
    virtual void Activate();
};

void CTCActivationReceptorCreatureGenerator::Activate()
{
    CTCActivationReceptorBase::Activate();
    CTCCreatureGenerator* generator;
    ParentThing->GetInterface(&generator, 0x3d);
    if (TriggerOnActivate)
        generator->ExternalTrigger = true;
    else
        generator->SetAsActive(true);
}