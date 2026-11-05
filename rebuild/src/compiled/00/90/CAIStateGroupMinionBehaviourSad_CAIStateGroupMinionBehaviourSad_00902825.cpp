// CAIStateGroup_MinionBehaviourSad::CAIStateGroup_MinionBehaviourSad() -- base CAIStateGroupBehaviourBase(), own vtable, then the PSadBecauseOfThing member (+0x34)
// Leaf: ??0CAIStateGroup_MinionBehaviourSad@@QAE@XZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00902825, 36 bytes, thiscall, `ret`.
// Identity: stores own vtable 0x0128D11C (RTTI .?AVCAIStateGroup_MinionBehaviourSad@@, base CAIStateGroupBehaviourBase : CAIStateGroupBase : CBaseClass); member vtable 0x01237F1C = RTTI .?AV?$CIntelligentPointer@$$CBVCThing@@@@; Ego_r.pdb / FableWin.pdb
//   ??0CAIStateGroup_MinionBehaviourSad@@QAE@XZ.
// Callees: CAIStateGroupBehaviourBase::CAIStateGroupBehaviourBase 0x009009A7 (??0CAIStateGroupBehaviourBase@@QAE@XZ), CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10 (??0CBaseIntelligentPointer@@QAE@XZ, Ego_r.pdb / FableWin.pdb).
// Layout: engine/CAIStateGroupBase.h's retail branch (CAIStateGroupBase : CBaseClass, 0x14); Ego_r.pdb CAIStateGroup_MinionBehaviourSad::PSadBecauseOfThing +0x34; the members before it
//   are left unmodelled. CBaseIntelligentPointer / CIntelligentPointer<T>: the chunk 703 model (0x00900B41).
// Shape: user default ctor, empty; CIntelligentPointer's ctor inlines to the base call + vtable store after this class's own vtable store.
// History: generated in a chunk batch for a repeated body shape over the CBase_009009a7 / CMemberBase_00a01b10 stand-ins; re-authored on the
//   PDB classes in naming wave 32 (family aisg).
#define FABLE_RETAIL_CAISTATEGROUPBASE
#include "engine/CAIStateGroupBase.h"                  // CAIStateGroupBase : CBaseClass (0x14), ctor 0x008FD255

class CBaseObjectPointer;
#pragma pack(push, 1)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
#pragma pack(pop)
class CThing;

class CAIStateGroupBehaviourBase : public CAIStateGroupBase {                    // RTTI CAIStateGroupBehaviourBase : CAIStateGroupBase
public:
    CAIStateGroupBehaviourBase();                         // ??0CAIStateGroupBehaviourBase@@QAE@XZ (Ego_r.pdb / FableWin.pdb)
    unsigned char _unmodeled14[0x34 - 0x14];       // Ego_r.pdb: PTargetThing +0x14, PTCPerception +0x1c, PTCOpinion +0x20, PTCCreatureMode +0x24, TriggerTime +0x28, RandomWaitTime +0x2c, ThisSpeechDuration +0x30
};

class CAIStateGroup_MinionBehaviourSad : public CAIStateGroupBehaviourBase {      // vtable 0x0128D11C
public:
    CIntelligentPointer<const CThing> PSadBecauseOfThing;                     // +0x34
    CAIStateGroup_MinionBehaviourSad();
};

CAIStateGroup_MinionBehaviourSad::CAIStateGroup_MinionBehaviourSad()
{
}
