// NScript::CQ_AmbushTradersScript::TeleportData::~TeleportData() -- destroy a convoy member's teleport record: the carried-item names (itemsCarried, every string then the block), then scriptName, then defName
// Leaf: ??1TeleportData@CQ_AmbushTradersScript@NScript@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CDBC50, 64 bytes, thiscall, tail `jmp ~CCharString` (the first member is destroyed last).
// Identity (proven): Ego_r.pdb ??1TeleportData@CQ_AmbushTradersScript@NScript@@QAE@XZ (the TeleportData of CQ_AmbushTradersScript is the
//   only PDB record with CCharStrings at +0 and +0xC round a 12-byte STLport vector<CCharString> at +0x10: defName +0,
//   appearanceSeed +4, health +8, scriptName +0xC, itemsCarried +0x10, 28 bytes); the landed vector<TeleportData>::~vector
//   0x00CDD0D0 / __destroy_aux 0x00CDC000 run this body per element, and the landed CAT_ConvoyMember::Main 0x00F0CAA0 builds the
//   record it destroys. The shared engine/CQ_AmbushTradersScript.h declares it out of line at this address.
// Callees: CCharString::~CCharString 0x0099EAE0 (??1CCharString@@QAE@XZ) for each carried-item string, scriptName and defName
//   (tail jump), free 0x00BFEA14 (the STLport allocator::deallocate behind the vector's null test).
// Body: the compiler-generated member-by-member destructor, emitted out of line under its PDB name: STLport 4.6 ~vector
//   (`_Destroy(_M_start, _M_finish)`: the inline __destroy_aux loop over the strings) and ~_Vector_base (free the block when
//   non-null), then the two strings in reverse declaration order.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_CONVOYMEMBER
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_AmbushTradersScript.h"

namespace NScript {
CQ_AmbushTradersScript::TeleportData::~TeleportData()
{
}
}