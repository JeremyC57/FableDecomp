// CTCTalk::PlayAISpeech(const CCharString& name, bool force, EDialogueLayer layer, bool b1, bool b2) -- plays a named line on the AI layer, only for a pure-AI speaker or when forced
// Leaf: ?PlayAISpeech@CTCTalk@@QAE?AW4ESpeechPlaybackResult@1@ABVCCharString@@_NW4EDialogueLayer@NSpeechGainManager@@11@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E3B30, length 48 bytes (ret 14h).
// Callee: CTCTalk::PlaySpeech(const CCharString&, EDialogueLayer, bool, bool) @ 0x006E3AB0 (ret 10h; it resolves the
// name to a sample id through the world's sound manager and calls the full PlaySpeech @ 0x006E3890).
// Ego_r.pdb CTCTalk: IsPureAISpeaker is the bool at +0x34. When the talker is not a pure-AI speaker and the
// caller did not force it, nothing is played and result 1 is returned (`mov eax,1 ; ret 14h`); otherwise the
// line is played with the layer pinned to 1 -- the caller's layer argument is ignored (its slot is never read).
// Shape: the two byte tests are `if (!IsPureAISpeaker && !force) return 1;` with the forward call the fall-through.
// NF59 (round 7, queues q10520 / q10539): the talk component is engine/CTCTalk.h (FABLE_RETAIL_CTCTALK_AISPEECHTEXT declares this overload,
//   Ego_r.pdb 0x0070E070 ?PlayAISpeech@CTCTalk@@QAE?AW4ESpeechPlaybackResult@1@ABVCCharString@@_NW4EDialogueLayer@NSpeechGainManager@@11@Z,
//   with the nested enum result; _PLAYSPEECH_OVERLOADS the text PlaySpeech 0x006E3AB0, Ego_r.pdb 0x0070E010 + FableWin.pdb 0x023E6D50).
//   The local CTCTalk / CCharString models returned a `typedef long` result and a long layer (mangled J).
// NF95 2026-11-19 (q11285): the NSpeechGainManager::EDialogueLayer enumerators are the PDB names (engine/NSpeechGainManager.h;
//   Ego_d.pdb LF_FIELDLIST 0x14C96, installed-game header_enums.csv): DIALOGUE_LAYER_FOREGROUND 0, _MIDGROUND 1, _BACKGROUND 2, _LAST 3;
//   were the stand-ins DIALOGUE_LAYER_0 / _1 / _2 / _NONE / _COUNT or a bare cast.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCTALK
#define FABLE_RETAIL_CTCTALK_PLAYSPEECH_OVERLOADS
#define FABLE_RETAIL_CTCTALK_AISPEECHTEXT
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCTalk.h"
namespace NSpeechGainManager { enum EDialogueLayer { DIALOGUE_LAYER_MIDGROUND = 1 }; }   // the enumerator this row uses (CTCTalk.h forward enum)

CTCTalk::ESpeechPlaybackResult CTCTalk::PlayAISpeech(const CCharString& name, bool force, NSpeechGainManager::EDialogueLayer layer, bool b1, bool b2)
{
    if (!IsPureAISpeaker && !force)
        return SPEECH_PLAYBACK_FAILED_FINAL;
    return PlaySpeech(name, NSpeechGainManager::DIALOGUE_LAYER_MIDGROUND, b1, b2);
}
