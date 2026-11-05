// CTCActivationReceptorChest::Deactivate() -- the base receptor Deactivate, then Close the thing's chest component (interface 0x3c)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FFCD0, length 85 bytes.
// Leaf: ?Deactivate@CTCActivationReceptorChest@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Callees: CTCActivationReceptorBase::Deactivate @ 0x007F7700 (non-virtual base call; the manifest's labels for 0x007F7700
// are propagated), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020, CTCChest::Close @ 0x007506C0.
// Ego_r.pdb: `?Deactivate@CTCActivationReceptorChest@@UAEXXZ` (slot 29; the chunk's "vfunc_29").
// Interface 0x3c = word 1, bit 28 of the presence bitset (`test dword ptr [eax+24h],10000000h`).
// Shape: the landed out-parameter GetInterface template (0x007504E0 / 0x0069F750 family): the presence bitset word
// test, LowerBound on the +0x44 map with the key in the `push ecx` frame word, end-or-larger-key -> the end entry, the
// component left in eax. The out pointer is NOT initialised: on the absent path retail reads the uninitialised local
// (`mov eax,[esp+N]`) and still makes the call, which the same source reproduces.
struct CTCBaseInterface;
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

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

class CTCChest { public:
    void Open();                               // 0x00750540
    void Close();                              // 0x007506C0
};

struct CTCActivationReceptorChest : CTCActivationReceptorBase {
    virtual void Activate();
    virtual void Deactivate();
};

void CTCActivationReceptorChest::Deactivate()
{
    CTCActivationReceptorBase::Deactivate();
    CTCChest* chest;
    ParentThing->GetTC(TCI_CONTAINER, &chest);
    chest->Close();
}