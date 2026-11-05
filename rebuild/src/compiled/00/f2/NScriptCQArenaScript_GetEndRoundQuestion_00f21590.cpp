// NScript::CQ_ArenaScript::GetEndRoundQuestion() -- text tag of the end-of-round "carry on?" question for the current gold multiplier
// Leaf: ?GetEndRoundQuestion@CQ_ArenaScript@NScript@@QAE?AVCCharString@@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F21590, length 204 bytes (jump table of 8 at 0x00F2163C inside the row), `ret 4`.
// Naming: Ego_r.pdb / FableWin.pdb ?GetEndRoundQuestion@CQ_ArenaScript@NScript@@QAE?AVCCharString@@XZ (declared in
//   engine/CQ_ArenaScript.h); the text tags TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_<n> are the arena quest's.
// Shape: a generated script function returning T opens with `TRet<T> _Fail_Val(<default>)` (Ego_r/FableWin name sets:
//   TRet<CCharString> ctor(const CCharString&) / dtor / Return): the "An error" literal 0x012EB2AC becomes a temporary,
//   is copied into _Fail_Val.Val (copy ctor 0x0099EC30) and destroyed (0x0099EAE0); _Fail_Val dies at the shared exit.
//   Cases 0..7 of GoldMultiplier (+0xB0, OnPersist "GoldMultiplier") return QUESTION_1..8 (0x012FABD4 .. 0x012FAAA0, paired
//   with the retail table by hand), anything else the plain QUESTION tag 0x012FAA74; one result-construction tail
//   (CCharString(const char*, long) 0x0099EBF0).
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CQ_ArenaScript.h"
#include "engine/TRet.h"

CCharString NScript::CQ_ArenaScript::GetEndRoundQuestion()
{
    TRet<CCharString> _Fail_Val("An error");
    switch (GoldMultiplier)
    {
    case 0: return "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_1";
    case 1: return "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_2";
    case 2: return "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_3";
    case 3: return "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_4";
    case 4: return "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_5";
    case 5: return "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_6";
    case 6: return "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_7";
    case 7: return "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_8";
    }
    return "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION";
}