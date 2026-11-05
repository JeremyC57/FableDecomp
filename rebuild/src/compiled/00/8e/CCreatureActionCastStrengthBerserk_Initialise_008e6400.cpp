// CCreatureAction_CastStrengthBerserk::Initialise() -- base Initialise, add a "CTCBerserk" component to the creature and start its effect, then if the creature has an interface-167 component set its byte at +0xc and store 8 in its word at +0x10
// Leaf: ?Initialise@CCreatureAction_CastStrengthBerserk@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E6400, length 142 bytes (+ int3 padding).
// Callees: CCombatActionBase::Initialise @ 0x00857520 (direct base call, first), CCharString::CCharString(const char*, long) @ 0x0099EBF0,
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, twice), CThing::AddTC(const CCharString&, bool, CTCParamsBase*)
// @ 0x004C9D60 (landed naming of 0x008C4250), CCharString::~CCharString @ 0x0099EAE0, CTCBerserk::CreateStartingEffect @ 0x0078C4C0,
// CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 167 = bit 7 of the bitset dword at +0x34, the sign test
// `test cl,cl ; jns`).
// Shape: `this` is dead after the base call, so esi is advanced to the PCreature address in place (`add esi,8`); the name is a scoped
// named local re-addressed with `lea`; the berserk component rides in edi across the string destructor and is not null-checked; the
// two stores into the looked-up component are immediates. Retail CCombatActionBase is 0x114 bytes (the engine header is the PDB's
// 0x12c-byte layout), so `this` is modelled locally.
#include "fable_string.h"
class CTCParamsBase;
class CTCBerserk { public:
    void CreateStartingEffect();            // 0x0078C4C0
};
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

struct CTCStrengthBoost {                   // the interface-167 component: a flag byte at +0xc and a strength word at +0x10
    unsigned char _pad[0xc];
    unsigned char m_active;                 // +0x0c
    unsigned char _pad0d[3];
    long m_boostLevel;                      // +0x10
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    virtual void Initialise();              // 0x00857520
    unsigned char _pad04[4];
    CIntelligentPointer<CThing> PCreature;      // +8
    unsigned char _pad10[0x114 - 0x10];
};

class CCreatureAction_CastStrengthBerserk : public CCombatActionBase { public:
    virtual void Initialise();
};

void CCreatureAction_CastStrengthBerserk::Initialise()
{
    CCombatActionBase::Initialise();
    CTCBerserk* pBerserk;
    {
        CCharString name("CTCBerserk", -1);
        pBerserk = ((CTCBerserk*)PCreature.Get()->AddTC(name, true, 0));
    }
    pBerserk->CreateStartingEffect();
    CTCStrengthBoost* pBoost;
    if (PCreature.Get()->GetTC(TCI_SPECIAL_ABILITIES, &pBoost)) {
        pBoost->m_active = 1;
        pBoost->m_boostLevel = 8;
    }
}