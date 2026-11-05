// CAIStateGroup_Guildmaster::CAIStateGroup_Guildmaster() -- default constructor.
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009030CB, length 63 bytes.
// Callees: CAIStateGroupBase::CAIStateGroupBase @ 0x008FD255 (out of line), CBaseIntelligentPointer::
// CBaseIntelligentPointer @ 0x00A01B10 twice (the two CIntelligentPointer members' inlined constructors:
// base ctor + vtable 0x0123E7D8 for CIntelligentPointer<CThingCreatureBase>, 0x01238C6C for
// CIntelligentPointer<CThing>). Vtable 0x0128D444 = CAIStateGroup_Guildmaster (rtti-vtables.tsv).
//
// Shape: base ctor, own vtable store, then the members in declaration order (names from
// rebuild/include/engine/CAIStateGroup_Guildmaster.h, which keeps the pointers/vector opaque, so the
// layout is modelled locally): PEnemy at +0x18, PLastTeleportMarker at +0x20, and the TeleportMarkers
// vector at +0x28 whose three pointers (first/last/end) are zeroed by its inline constructor. AttackNum
// (+0x14) and the two frame counters after the vector are not initialised by the constructor.
class CThing;
class CThingCreatureBase;

class CAIStateGroupBase { public:
    virtual void v00();
    CAIStateGroupBase();                        // 0x008FD255
};

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


template<class T> struct CRetailVector {        // retail std::vector<T>: first/last/end, 12 bytes (the PDB's 0x10 is Ego_r's STL)
    T* first;
    T* last;
    T* end_of_storage;
    CRetailVector() : first(0), last(0), end_of_storage(0) { }
};

struct CAIStateGroup_Guildmaster : CAIStateGroupBase {
    unsigned char _base[0x10];                  // +0x04 CAIStateGroupBase members (PBrain, priority, name, flags)
    long AttackNum;                             // +0x14 (uninitialised here)
    CIntelligentPointer<CThingCreatureBase> PEnemy;            // +0x18
    CIntelligentPointer<CThing> PLastTeleportMarker;           // +0x20
    CRetailVector<CIntelligentPointer<CThing> > TeleportMarkers; // +0x28 (12 bytes in retail, cf. the two
                                                //   adjacent vectors of CAIStateGroup_JackOfBladesBase @ 0x00902FE8)
    long LastWillFrame;                         // +0x34 (PDB: +0x38)
    long LastTeleportFrame;                     // +0x38 (PDB: +0x3c)
    virtual void v00();
    CAIStateGroup_Guildmaster();
};

CAIStateGroup_Guildmaster::CAIStateGroup_Guildmaster()
    : CAIStateGroupBase()
    , PEnemy()
    , PLastTeleportMarker()
    , TeleportMarkers()
{
}