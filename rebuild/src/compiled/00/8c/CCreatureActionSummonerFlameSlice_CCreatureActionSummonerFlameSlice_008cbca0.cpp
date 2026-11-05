// CCreatureAction_SummonerFlameSlice::CCreatureAction_SummonerFlameSlice(CThingCreatureBase*, CThing*) -- base CCombatActionBase(owner, target, 150, zeroed CSoundPair), own vtable (0x01264214), a default CIntelligentPointer at +0x114
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CBCA0, length 80 bytes.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, CThing*, long, const CSoundPair&) @ 0x00857F30,
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 (then the CIntelligentPointer vtable 0x01238C6C).
// Shape: the target argument is loaded into edx before the frame is set up; the sound pair is zeroed by its inline constructor
// (register-zeroed stack stores); the member pointer is constructed after the vtable store, addressed once in edi. `ret 8`.
class CThingCreatureBase;
class CThing;
class CSoundPair { public:                         // two sound ids, zeroed by the inline constructor (register-zeroed stack stores)
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound);   // 0x00857F30
};

class CCreatureAction_SummonerFlameSlice : public CCombatActionBase { public:   // vtable 0x01264214
    CIntelligentPointer<CThing> PSummonedThing;     // +0x114 (the pointer FinishAction @ 0x008CAD10 kills)
    virtual void v00();
    CCreatureAction_SummonerFlameSlice(CThingCreatureBase* owner, CThing* target);
};

CCreatureAction_SummonerFlameSlice::CCreatureAction_SummonerFlameSlice(CThingCreatureBase* owner, CThing* target)
    : CCombatActionBase(owner, target, 150, CSoundPair())
    , PSummonedThing()
{
}