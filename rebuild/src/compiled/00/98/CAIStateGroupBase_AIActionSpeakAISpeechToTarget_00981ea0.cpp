// CAIStateGroupBase::AIAction_SpeakAISpeechToTarget(EAIActionRepeat repeat, CIntelligentPointer<CThingCreatureBase> speaker, CIntelligentPointer<const CThing> target, const CCharString& speech, bool interrupt, long priority) -- forwards to the seven-argument AIAction_SpeakAISpeechToTarget with a trailing `true`
// Leaf: ?AIAction_SpeakAISpeechToTarget@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@V?$CIntelligentPointer@VCThingCreatureBase@@@@V?$CIntelligentPointer@$$CBVCThing@@@@ABVCCharString@@_NJ@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00981EA0, length 130 bytes, `ret 20h`.
// Callees: CAIStateGroupBase::AIAction_SpeakAISpeechToTarget(...) @ 0x00981350 (thiscall, seven arguments),
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, ::GetPItem @ 0x00A01B50, ::SetPItem @ 0x00A01B90,
// ::~CBaseIntelligentPointer @ 0x00A01C10.
// Shape: arguments evaluated right to left -- the literal `true`, the three scalar arguments (two of them loaded
// before the prologue pushes), then the two by-value intelligent pointers copy-constructed in their argument slots
// (`sub esp,8 ; mov esi,esp`, inline family-44 copy constructor: base ctor, own vtable 0x01237F1C / 0x0123E7D8,
// SetPItem(other.GetPItem())), the repeat mode last. The callee-destroyed by-value parameters are released after
// the call through the base destructor (the CIntelligentPointer<T> classes declare none); the result rides in esi.
// The two rows 0x009812C0 / 0x00981EA0 are the same forwarder shape over two different seven-argument targets
// (0x009812C0 -> 0x00980BD0 AIAction_SpeakToTarget with an unsigned long speech id, 0x00981EA0 -> 0x00981350
// AIAction_SpeakAISpeechToTarget with the speech name as `const CCharString&`).
// NF84 (round 7, q10656 self, NF64-blocked): re-authored on engine/CAIStateGroupBase.h: Ego_r.pdb 0x0084D220 spells this member
//   ?AIAction_SpeakAISpeechToTarget@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@V?$CIntelligentPointer@VCThingCreatureBase@@@@V?$CIntelligentPointer@$$CBVCThing@@@@ABVCCharString@@_NJ@Z
//   (protected, speech by `const CCharString&`; the K overload is 0x009812C0), the header's AIACTIONS + SPEECH block declares it at
//   this address, the landed 0x0092FAB0 already binds that spelling here, and the callers pass CCharString objects (SUCCESS_STRING /
//   FAILURE_STRING in 0x0091D190). The by-value intelligent pointers are engine/CIntelligentPointer.h's; the seven-argument callee
//   0x00981350 comes from the NF84 overlay opt-in FABLE_RETAIL_CAISTATEGROUPBASE_SPEAKAISPEECH_WAIT (descriptive: no PDB has it).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEECH
#define FABLE_RETAIL_CAISTATEGROUPBASE_SPEAKAISPEECH_WAIT
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CAIStateGroupBase.h"

EAIActionReturn CAIStateGroupBase::AIAction_SpeakAISpeechToTarget(EAIActionRepeat repeat, CIntelligentPointer<CThingCreatureBase> speaker,
    CIntelligentPointer<const CThing> target, const CCharString& speech, bool interrupt, long priority)
{
    return AIAction_SpeakAISpeechToTarget(repeat, speaker, target, speech, interrupt, priority, true);
}
