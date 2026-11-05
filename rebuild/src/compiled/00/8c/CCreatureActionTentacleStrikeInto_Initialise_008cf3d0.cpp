// CCreatureAction_TentacleStrikeInto::Initialise() -- cache the creature's kraken-tentacle component (interface type 0xe3), set the name "TENTACLE_SMASH_INTO", start the tentacle striking, then the base Initialise
// Leaf: ?Initialise@CCreatureAction_TentacleStrikeInto@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CF3D0, length 129 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for
// type 0xe3 = bit 3 of the bitset byte at +0x3c), CCharString::CCharString(const char*, long) @ 0x0099EBF0 (literal 0x01284D78 "TENTACLE_SMASH_INTO"),
// CCombatActionBase::SetCombatAnimation(const CCharString&) @ 0x008574D0, CCharString::~CCharString @ 0x0099EAE0, CTCKrakenTentacle::SetIsStriking(bool) @ 0x007F0770,
// CCombatActionBase::Initialise @ 0x00857520 (direct base call, last).
// Shape: the GetInterface out pointer is the MEMBER at +0x114 (the hit path stores `mov [edi+114h],ecx`, the miss path leaves it untouched, GOTCHAS 24m);
// the scoped name string shares the `push ecx` frame word with the lookup key; the member is re-read for SetIsStriking.
// The PDB header (CCreatureAction_TentacleStrikeInto.h) flattens the base; modelled locally (CCombatActionBase is 0x114 bytes in retail).
#include "fable_string.h"
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CTCKrakenTentacle { public:                  // interface type 0xe3 component
    void SetIsStriking(bool striking);      // 0x007F0770
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    virtual void Initialise();              // 0x00857520
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
public:
    void SetCombatAnimation(const CCharString&);   // 0x008574D0
};
class CCreatureAction_TentacleStrikeInto : public CCombatActionBase { public:
    CTCKrakenTentacle* PTentacle;           // +0x114
    virtual void Initialise();
};

void CCreatureAction_TentacleStrikeInto::Initialise()
{
    PCreature.Get()->GetTC(TCI_KRAKEN_TENTACLE, &PTentacle);
    {
        CCharString name("TENTACLE_SMASH_INTO", -1);
        SetCombatAnimation(name);
    }
    PTentacle->SetIsStriking(true);
    CCombatActionBase::Initialise();
}