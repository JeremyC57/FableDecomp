// CCreatureAction_BalverineDropInFromSky::Initialise() -- names the animation "GENERATE_DROP", calls the creature's virtual slot 28 with true, attaches the particles of its interface-130 component, marks the creature as not willable, then runs the base Initialise
// Leaf: ?Initialise@CCreatureAction_BalverineDropInFromSky@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CCA60, length 151 bytes (+ int3 padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCombatActionBase::SetCombatAnimation(const CCharString&) @ 0x008574D0,
// CCharString::~CCharString @ 0x0099EAE0, CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, three times), the creature's
// virtual slot 28 (+0x70) with one argument, CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 130 = bit 2 of
// the bitset dword at +0x30), CTCParticleAttacher::AttachParticles @ 0x007F8890, SetThingAsNotBloodyWillableAtAll(CThing*, bool) @ 0x007E4040
// (`__fastcall`: ecx thing, dl flag), CCombatActionBase::Initialise @ 0x00857520 (direct base call, last).
// Shape: `this` rides in edi and the PCreature address in ebx; esi is pushed only inside the lookup block; the animation name is
// an implicit conversion temporary re-addressed with `lea`. Retail CCombatActionBase is 0x114 bytes (the engine header is the PDB's
// 0x12c-byte layout), so `this` is modelled locally.
class CTCBase;
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CTCPhysicsBase;
struct CTCGraphicAppearance;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


struct CThingCreatureCalls {                // the creature's own virtuals: slot 28 takes one flag
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void SetInvisible(bool invisible);   // slot 28 (+0x70)
};
class CTCParticleAttacher { public:                // the interface-130 component
    void AttachParticles();                 // 0x007F8890
};
class CTCWillResponse {
public:
    static void __fastcall SetThingAsNotBloodyWillableAtAll(CThing*, bool);   // 0x007E4040
};   // 0x007E4040

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    virtual void Initialise();              // 0x00857520
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
public:
    void SetCombatAnimation(const CCharString&);   // 0x008574D0
};

class CCreatureAction_BalverineDropInFromSky : public CCombatActionBase { public:
    virtual void Initialise();
};

void CCreatureAction_BalverineDropInFromSky::Initialise()
{
    SetCombatAnimation("GENERATE_DROP");
    ((CThingCreatureCalls*)PCreature.Get())->SetInvisible(true);
    CTCParticleAttacher* pParticles;
    if (PCreature.Get()->GetTC(TCI_PARTICLE_ATTACHER, &pParticles))
        pParticles->AttachParticles();
    CTCWillResponse::SetThingAsNotBloodyWillableAtAll(PCreature.Get(), true);
    CCombatActionBase::Initialise();
}