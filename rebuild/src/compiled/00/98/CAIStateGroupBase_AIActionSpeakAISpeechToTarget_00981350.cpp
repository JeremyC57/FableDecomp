// CAIStateGroupBase::AIAction_SpeakAISpeechToTarget(repeat, speaker, target, const CCharString& speech, bool interrupt, long priority, bool wait)
//   -- look the AI speech name up in the game text bank (CRC of the name) and forward to AIAction_SpeakToTarget with the same flags
// Leaf: ?AIAction_SpeakAISpeechToTarget@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@V?$CIntelligentPointer@VCThingCreatureBase@@@@V?$CIntelligentPointer@$$CBVCThing@@@@ABVCCharString@@_NJ4@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00981350, 181 bytes, `ret 24h`. Callees: CCRC::Calc 0x004014A0 (inline CCharString::GetCRC),
//   CBankFile::FindIndexBySymbolCRC 0x009CC410 on CMainGameComponent::GetTextBank() (PMainGameComponent 0x013B86A0), the two by-value
//   intelligent pointers copied through their inline copy ctors (CBaseIntelligentPointer 0x00A01B10 / GetPItem / SetPItem, vtables 0x01237F1C /
//   0x0123E7D8) and destroyed after the call (0x00A01C10), CAIStateGroupBase::AIAction_SpeakToTarget 0x00980BD0 (Ego_r ..K_NJ3@Z).
// Names (NF86, q11108): the seven-argument overload is in no PDB (TLC added the trailing flag; NF84's descriptive spelling, the
//   FABLE_RETAIL_CAISTATEGROUPBASE_SPEAKAISPEECH_WAIT declaration in engine/CAIStateGroupBase.h): protected like the PDB six-argument overload
//   0x00981EA0 (which forwards here with `true`) and its sibling AIAction_SpeakToTarget(.., bool interrupt, long priority, bool wait), with the
//   speech as the six-argument overload's const CCharString&. 0x00981EA0 (NF84) and the q11108 caller bind this spelling. Descriptive (NOTES).
// Port (NF86): was a public long-parameter leaf over a local CAIStateGroupBase / CBaseIntelligentPointer / CIntelligentPointer model; now the
//   shared engine/CAIStateGroupBase.h (+ CIntelligentPointer COPY, CThingCreatureBase, CMainGameComponent TEXTBANK, NGameText_CDataBank), the
//   0x00981EA0 include set; the AIAction_SpeakToTarget call binds the PDB spelling.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_CCHARSTRING_GETASCHARARRAY_SELECT
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
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CBANKFILE_FINDINDEXBYSYMBOLCRC
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGameText_CDataBank.h"

EAIActionReturn CAIStateGroupBase::AIAction_SpeakAISpeechToTarget(EAIActionRepeat repeat, CIntelligentPointer<CThingCreatureBase> speaker,
    CIntelligentPointer<const CThing> target, const CCharString& speech, bool interrupt, long priority, bool wait)
{
    const NGameText::CDataBank& bank = CMainGameComponent::Get()->GetTextBank();
    unsigned long speech_id = bank.FindIndexBySymbolCRC(speech.GetCRC());
    return AIAction_SpeakToTarget(repeat, speaker, target, speech_id, interrupt, priority, wait);
}
