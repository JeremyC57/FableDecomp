// CTCBoastingPodium::CTCBoastingPodium(CThing* thing) -- CTCBase(thing), own vtable, the empty BoastingPositions vector map, the boast counters cleared, BoastingActive false, PDef null
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E7513, length 50 bytes; a size-optimised TU
// (`push dword ptr [esp+8]` pass-through): `#pragma optimize("s", on)`.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800. Vtable 0x0124346C = CTCBoastingPodium (rtti-vtables.tsv).
// Layout = Ego_r.pdb CTCBoastingPodium with the retail 12-byte vector: BoastingPositions
// (CVectorMap<long, CIntelligentPointer<CThing>, CKeyPairCompareLess>) +0xc = vector +0xc..+0x17,
// the empty comparator +0x18, the sorted flag +0x19; LastPositionsWithBoastCount +0x1c,
// FrameBoastListFrom +0x20, BoastingActive +0x24, PDef (CDefPointer) +0x28.
// Shape: every store follows the vtable store and shares the eax zero: the vector map and the def
// pointer are member objects with inline constructors (Wave 24i), the scalars are initialisers.
#pragma optimize("s", on)
class CThing;

struct CTCBase_Calls {                                      // engine/CTCBase.h layout (0xc bytes)
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CTCBase_Calls(CThing* thing);                           // 0x00686800
};

struct SBoastingPosition;                                   // pair<long, CIntelligentPointer<CThing>>
struct CKeyPairCompareLess { };

struct CBoastingPositionMap {                               // CVectorMap: a sorted vector of pairs
    SBoastingPosition*  m_pFirst;                           // +0x00
    SBoastingPosition*  m_pLast;                            // +0x04
    SBoastingPosition*  m_pEnd;                             // +0x08
    CKeyPairCompareLess m_compare;                          // +0x0c
    bool                m_sorted;                           // +0x0d
    CBoastingPositionMap() : m_pFirst(0), m_pLast(0), m_pEnd(0), m_sorted(false) { }
};

class CBoastingPodiumDef;

struct CBoastingPodiumDefPointer {                          // CDefPointer<const CBoastingPodiumDef>
    const CBoastingPodiumDef* m_pDef;
    CBoastingPodiumDefPointer() : m_pDef(0) { }
};

class CTCBoastingPodium : public CTCBase_Calls { public:                  // vtable 0x0124346C
    CBoastingPositionMap      BoastingPositions;            // +0x0c
    long                      LastPositionsWithBoastCount;  // +0x1c
    unsigned long             FrameBoastListFrom;           // +0x20
    bool                      BoastingActive;               // +0x24
    CBoastingPodiumDefPointer PDef;                         // +0x28
    virtual void v00();
    CTCBoastingPodium(CThing* thing);
};

CTCBoastingPodium::CTCBoastingPodium(CThing* thing)
    : CTCBase_Calls(thing)
    , LastPositionsWithBoastCount(0)
    , FrameBoastListFrom(0)
    , BoastingActive(false)
{
}