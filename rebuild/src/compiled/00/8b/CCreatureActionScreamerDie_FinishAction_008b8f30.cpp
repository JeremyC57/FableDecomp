// CCreatureAction_ScreamerDie::FinishAction() -- clear the attachments of the live effect thing's particle emitter (interface type 0x4b), run the base FinishAction, then kill the creature
// Leaf: ?FinishAction@CCreatureAction_ScreamerDie@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B8F30, length 132 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (the effect pointer at +0x114 three times, PCreature at +8 once),
// CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 0x4b = bit 11 of the bitset dword at +0x28, `test ch,8`),
// CTCDParticleEmitter::ClearAttachments @ 0x006E0DE0, CCombatActionBase::FinishAction @ 0x00859A20 (direct base call),
// CThing::Kill(bool) @ 0x004C9B80.
// Shape: the landed IS_THING_ALIVE idiom on the +0x114 pointer, then a NULL-INITIALISED emitter pointer (`xor edi,edi`, pushed inside
// the if: edi is only live there) filled by the out-parameter GetInterface and used unconditionally. `this` rides in ebx.
// No engine header for this class; modelled locally (CCombatActionBase is 0x114 bytes in retail, the effect pointer is the first own member).
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined, re-fetching GetPItem).
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.Get() != 0) {
        if (!p.Get()->IsDead())
            return true;
    }
    return false;
}

class CTCDParticleEmitter { public:                // interface type 0x4b component
    void ClearAttachments_006e0de0();                // 0x006E0DE0
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    protected: virtual void FinishAction(); public:            // slot 4: 0x00859A20
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
};

class CCreatureAction_ScreamerDie : public CCombatActionBase { public:
    CIntelligentPointer<CThing> PEffect;        // +0x114 CIntelligentPointer<CThing>: the scream effect thing
    virtual void FinishAction();
};

void CCreatureAction_ScreamerDie::FinishAction()
{
    if (IS_THING_ALIVE(PEffect)) {
        CTCDParticleEmitter* pEmitter = 0;
        PEffect.Get()->GetTC(TCI_PARTICLE_EMITTER, &pEmitter);
        pEmitter->ClearAttachments_006e0de0();
    }
    CCombatActionBase::FinishAction();
    PCreature.Get()->Kill(true);
}