// NScript::CV_AmbushScamScript::CAS_Trickster::GetRandomSpeech() -- pick one of 5 speech lines at random, never either of the last two, yielding a script frame per rejected pick; the empty string when the script is done
// Leaf: ?GetRandomSpeech@CAS_Trickster@CV_AmbushScamScript@NScript@@AAE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E2E440, length 210 bytes (`ret 4`, CCharString by hidden pointer).
// Callees: CCharString default ctor @ 0x0099E4B0, copy ctor @ 0x0099EC30, operator= @ 0x0099EFB0, dtor @ 0x0099EAE0,
// rand @ 0x00BFEB16, CEntityScriptBase::IsActiveThreadTerminating @ 0x00F35B30, the game script interface (+4) slot 7 NewScriptFrame (+0x1c).
// Shape: `for (;;) { if (IsActiveThreadTerminating()) return none; pick; if (ok) break; yield; }` then IsActiveThreadTerminating re-checked
// around the bookkeeping; an empty string, a copy of it returned on every exit, and the chosen line copied out last (no NRVO: each
// return copy-constructs the hidden result); the line table is CV_AmbushScamScript::RandomSpeech[5] (+0x80 of the parent, +0x14).
// NF196 (2026-11-20, q6532): re-authored on the shared engine/CV_AmbushScamScript.h (as the landed caller CAS_Trickster::Main 0x00E2C100): the self leaf is now the Ego_r.pdb
//   ?GetRandomSpeech@CAS_Trickster@CV_AmbushScamScript@NScript@@AAE?AVCCharString@@XZ (private, nested; the caller already binds it);
//   was the global stand-in CAC_Owner (catalog label propagated from a byte-identical body) (no Leaf line) with row-local CCharString / CEntityScriptBase / CSpeechRecord models.
// NF200 (2026-11-20, q6532): NF196's draft graded unchanged (RELOCATION_MATCH); recorded and handed back here.
#include "fable_compiler.h"
#define FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CV_AmbushScamScript.h"
#include "engine/CGameScriptInterfaceBase.h"
extern "C" int __cdecl rand();                      // 0x00BFEB16 (MSVCR71)
namespace NScript {
CCharString CV_AmbushScamScript::CAS_Trickster::GetRandomSpeech()
{
    CCharString empty;
    CCharString none(empty);
    CCharString speech;
    long index;
    for (;;)
    {
        if (IsActiveThreadTerminating())
            return none;
        index = rand() % 5;
        if (index != lastRandomSpeechIdx1 && index != lastRandomSpeechIdx2)
            break;
        Game->NewScriptFrame();
    }
    if (IsActiveThreadTerminating())
        return none;
    lastRandomSpeechIdx2 = lastRandomSpeechIdx1;
    lastRandomSpeechIdx1 = index;
    if (IsActiveThreadTerminating())
        return none;
    speech = ParentClass.RandomSpeech[index];
    return speech;
}
}
