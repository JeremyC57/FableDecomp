// NScript::CV_ArcheryCompetitionScript::CAC_Owner::GetRandomSpeech() -- pick one of 10 owner speech lines at random, never either of the last two, yielding a script frame per rejected pick; the empty string when the script is done
// Leaf: ?GetRandomSpeech@CAC_Owner@CV_ArcheryCompetitionScript@NScript@@AAE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E362B0, length 210 bytes (`ret 4`, CCharString by hidden pointer).
// Callees: CCharString default ctor @ 0x0099E4B0, copy ctor @ 0x0099EC30, operator= @ 0x0099EFB0, dtor @ 0x0099EAE0,
// rand @ 0x00BFEB16, CEntityScriptBase::IsActiveThreadTerminating @ 0x00F35B30, the game script interface (+4) slot 7 NewScriptFrame (+0x1c).
// Shape: `for (;;) { if (IsActiveThreadTerminating()) return none; pick; if (ok) break; yield; }` then IsActiveThreadTerminating re-checked
// around the bookkeeping; an empty string, a copy of it returned on every exit, and the chosen line copied out last (no NRVO: each
// return copy-constructs the hidden result); the line table is CV_ArcheryCompetitionScript::RandomSpeech[10] (+0x74 of the parent, +0x14).
// NF196 (2026-11-20, q6524): re-authored on the shared engine/CV_ArcheryCompetitionScript.h (opt-in
//   FABLE_RETAIL_CV_ARCHERYCOMPETITIONSCRIPT_OWNERMAIN, as the landed caller CAC_Owner::Main 0x00E33EA0): the self leaf is now the Ego_r.pdb
//   ?GetRandomSpeech@CAC_Owner@CV_ArcheryCompetitionScript@NScript@@AAE?AVCCharString@@XZ (private, nested; the caller already binds it);
//   was the global stand-in CAC_Owner (no Leaf line) with row-local CCharString / CEntityScriptBase / CSpeechRecord models.
#include "fable_compiler.h"
#define FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CV_ARCHERYCOMPETITIONSCRIPT_OWNERMAIN
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CV_ArcheryCompetitionScript.h"
#include "engine/CGameScriptInterfaceBase.h"
extern "C" int __cdecl rand();                      // 0x00BFEB16 (MSVCR71)
namespace NScript {
CCharString CV_ArcheryCompetitionScript::CAC_Owner::GetRandomSpeech()
{
    CCharString empty;
    CCharString none(empty);
    CCharString speech;
    long index;
    for (;;)
    {
        if (IsActiveThreadTerminating())
            return none;
        index = rand() % 10;
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
