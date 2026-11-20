// std::vector<NScript::CQ_AmbushTradersScript::TeleportData>::swap(vector& x) -- STLport 4.6: std::swap of the start, finish and
//   end-of-storage words with the other vector's
// Leaf: ?swap@?$vector@UTeleportData@CQ_AmbushTradersScript@NScript@@V?$allocator@UTeleportData@CQ_AmbushTradersScript@NScript@@@std@@@std@@QAEXAAV12@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F0CA30, 41 bytes (`ret 4`): three inlined std::swap, each loading this-word into edx and
//   other-word into esi, storing the other-word first.
// Name fix (NF195, 2026-11-20, q6415): was the descriptive SThreeWordRecord::Swap (catalog; the manifest label
//   CNearestBuildingWithProperty::CNearestBuildingWithProperty was propagated). Identity: CQ_AmbushTradersScript::PhaseTwo 0x00F0C140
//   calls it at 0x00F0C823 with ecx = &TeleportInfo and an empty local vector (TeleportInfo.swap(empty)) and already binds this spelling;
//   the body is STLport vector::swap (FABLE_RETAIL_STLP_VECTOR_SWAP); Ego_r.pdb 0x00F91AC0 and FableWin.pdb 0x03B0BD50 name the instance
//   (?swap@?$vector@UTeleportData@..@@QAEXAAV12@@Z). Now an explicit
//   instantiation on the shared headers (the 0x00F0C140 include set; Ego_r TeleportData, engine/CQ_AmbushTradersScript.h PHASETWO).
#define FABLE_RETAIL_STLP_VECTOR_SWAP
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE
#define FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_MAIN
#define FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_CONVOYMEMBER
#define FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_PHASETWO
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_AmbushTradersScript.h"
#include "engine/C3DVector.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_def_global.h"

template void std::vector<NScript::CQ_AmbushTradersScript::TeleportData>::swap(std::vector<NScript::CQ_AmbushTradersScript::TeleportData>&);
