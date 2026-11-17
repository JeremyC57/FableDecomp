// CTCKraken::~CTCKraken() -- destroy the tentacle list, the beam/head/target/marker/chamber pointers, the explosion list and the def
// Leaf: ??1CTCKraken
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E1437, length 88 bytes (size-optimised def/factory module -> #pragma
// optimize("s", on)). Called by the scalar deleting destructor 0x004E141B (CTCKraken vtable 0x01241B94 slot 0); the
// manifest label "~CTCDExplosion" is propagated.
// Callees: the STLport list<CIntelligentPointer<CThing> > destructor @ 0x004B8940 (x2), CBaseIntelligentPointer::
// ~CBaseIntelligentPointer @ 0x00A01C10 (x5), CTCBase::~CTCBase @ 0x00686830 (tail jump).
// Shape: reverse member order with the RETAIL offsets: the STLport lists are 4 bytes (PDB 12), so PBeamWeaponEffect sits at
// +0x34 and Tentacles at +0x44 (PDB +0x3c / +0x4c); the CDefPointer release of PKrakenDef (+0x0c) is inlined last.
// NF72 (round 7, q10902): the two ~list calls @ 0x004B8940 now bind the STLport
//   ??1?$list@V?$CIntelligentPointer@VCThing@@@@V?$allocator@V?$CIntelligentPointer@VCThing@@@@@std@@@std@@QAE@XZ (ported this round): the local
//   CThingPointerList became std::list<CIntelligentPointer<CThing> > from the shared stlport/fable_containers.h (LIST_DTOR_OUTOFLINE, the
//   0x005A887D form), and the local CIntelligentPointer stand-in the shared engine/CIntelligentPointer.h (retail branch), which the list needs.
#include "fable_compiler.h"
#pragma optimize("s", on)
#define FORCEINLINE FABLE_FORCEINLINE

class CThing;

class CTCBase { public:                            // CTCBase root: vptr, ParentThing, two flags (0x0c bytes)
    void* __vftable;                        // +0x00
    CThing* ParentThing;                    // +0x04
    bool ToBeRemoved;                       // +0x08
    bool IsInGlobalUpdateTCs;               // +0x09
    ~CTCBase();                             // 0x00686830 (stores the CTCBase vtable, then the CBaseObject destructor)
};

class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                 // slot 1
    long RefCount;                          // +0x04
};

struct CDefPointer {                        // CDefPointer<T>: the pointee's count at +4, destroyed through slot 1
    CDefPointeeBase* Object;
    FORCEINLINE ~CDefPointer()
    {
        CDefPointeeBase* object = Object;
        if (object) {
            if (--object->RefCount == 0)
                object->Destroy();
            Object = 0;
        }
    }
};

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"   // CIntelligentPointer<CThing>: vptr +0x0, PData +0x4; ~CBaseIntelligentPointer 0x00A01C10
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#define FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE
#include "stlport/fable_containers.h"
typedef std::list<CIntelligentPointer<CThing> > CThingPointerList;   // STLport list (4 bytes); ~list 0x004B8940

class CTCKraken : public CTCBase { public:
    CDefPointer PKrakenDef;                 // +0x0c
    CIntelligentPointer<CThing> PChamber;           // +0x10
    CIntelligentPointer<CThing> PCentreMarker;      // +0x18
    CIntelligentPointer<CThing> PTarget;            // +0x20
    CIntelligentPointer<CThing> PHeadEffect;        // +0x28
    CThingPointerList ExplosionList;        // +0x30
    CIntelligentPointer<CThing> PBeamWeaponEffect;  // +0x34
    long DelayBeforeBeam;                   // +0x3c
    void* PTCBoss;                          // +0x40
    CThingPointerList Tentacles;            // +0x44
    void* AttackingTentacleIterator;        // +0x48
    ~CTCKraken();
};

CTCKraken::~CTCKraken()
{
}