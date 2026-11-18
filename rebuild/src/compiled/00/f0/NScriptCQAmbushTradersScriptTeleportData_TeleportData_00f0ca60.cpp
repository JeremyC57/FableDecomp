// NScript::CQ_AmbushTradersScript::TeleportData::TeleportData(const TeleportData& other) -- memberwise copy constructor of the 0x1C-byte convoy teleport record: two CCharStrings, two words and the carried-items vector
// Leaf: ??0TeleportData@CQ_AmbushTradersScript@NScript@@QAE@ABU012@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F0CA60, 57 bytes (+ int3 padding).
// Identity: Ego_r.pdb ??0TeleportData@CQ_AmbushTradersScript@NScript@@QAE@ABU012@@Z; engine/CQ_AmbushTradersScript.h declares it at this
//   address (FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_PHASETWO), and the landed vector<TeleportData>::push_back 0x00F0D490 binds this spelling.
//   Its retail callers are PhaseTwo 0x00F0C140 and the TeleportInfo vector's push_back / _M_insert_overflow. The old row's global
//   ??0TeleportData@@ and its local CCharString / one-argument Dinkumware vector models were stand-ins.
// Callees: CCharString copy ctor @ 0x0099EC30 (x2), vector<CCharString> copy ctor @ 0x007ACEA0 (q11134, STLport spelling).
// Shape: the implicit memberwise copy, in member order; `return this`.
// NF87: re-authored on the shared header (the copy ctor written out memberwise, as the implicit one VC7.1 emits out of line).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
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

NScript::CQ_AmbushTradersScript::TeleportData::TeleportData(const TeleportData& other)
    : defName(other.defName)
    , appearanceSeed(other.appearanceSeed)
    , health(other.health)
    , scriptName(other.scriptName)
    , itemsCarried(other.itemsCarried)
{
}
