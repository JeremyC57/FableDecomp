// CTCHeroCentreDoorMarker::OnInitialActivate() -- when the parent thing has the hero-centre link component (interface 0x33), point PHeroCentre at the hero centre that component refers to
// Leaf: OnInitialActivate
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: map pointer made const (the row grades the const LowerBound).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077F750, length 129 bytes (+15 int3 padding). Vtable 0x0126B044 slot 6.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x004365B0 (inlined interface lookup), CBaseIntelligentPointer::
// GetPItem @ 0x00A01B50 (the component's pointer at +0xd, then the temporary), CBaseIntelligentPointer ctor @ 0x00A01B10 /
// SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10.
// Shape: the component (bit 19 of CThing +0x24 = interface 0x33, `test dword ptr`) is fetched through the inline GetInterface;
// `PHeroCentre = CIntelligentPointer<CThing>(item);` builds, assigns and destroys a frame temporary (no user dtor on
// CIntelligentPointer, so no vtable store before the base dtor).
// Layout (packed): PHeroCentre +0x1d; the interface-0x33 component keeps a packed CIntelligentPointer<CThing> at +0x0d.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#include "engine/CIntelligentPointer.h"

class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                        // +0x04
    bool    ToBeRemoved;                        // +0x08
    bool    IsInGlobalUpdateTCs;                // +0x09
};
#pragma pack(push, 1)
class CTCHeroCentreLink {                       // interface 0x33
public:
    unsigned char _pad00[0x0d];
    CIntelligentPointer<CThing> PHeroCentre;    // +0x0d
};
class CTCHeroCentreDoorMarker : public CTCBase {    // vtable 0x0126B044
public:
    float Radius;                               // +0x0c
    long  DoorType;                             // +0x10
    bool  HeroInsideRadius;                     // +0x14
    CIntelligentPointer<CThing> PParticle;      // +0x15
    CIntelligentPointer<CThing> PHeroCentre;    // +0x1d
    virtual void OnInitialActivate();
};
#pragma pack(pop)

void CTCHeroCentreDoorMarker::OnInitialActivate()
{
    const CTCHeroCentreLink* link;
    if (ParentThing->PeekTC(TCI_OWNED_ENTITY, &link))
        PHeroCentre = CIntelligentPointer<CThing>(link->PHeroCentre.Get());
}