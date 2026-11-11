// CTCInventoryAbilities::InitialiseExpressions() -- sizes the received-expression order to the number of EXPRESSION definitions (new entries zeroed)
// Leaf: ?InitialiseExpressions@CTCInventoryAbilities@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F3E3E, length 63 bytes.
// Identity: Ego_r.pdb + FableWin.pdb ?InitialiseExpressions@CTCInventoryAbilities@@QAEXXZ.
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0, CTCBase::GetDefinitionManager 0x006869D0,
//          CDefinitionManager::GetNoInstantiatedDefsOfType(const CCharString&) 0x009AD3B0, std::vector<long>::resize(size_type)
//          0x004433F0 (out of line), CCharString::~CCharString 0x0099EAE0. The literal "EXPRESSION" sits at 0x01235D94.
// Pragmas: frame-pointer size TU (`optimize("y", off)` + `("s", on)`; `push ecx` frame word, `leave`).
// Layout: engine/CTCInventoryAbilities.h retail branch: ReceivedExpressionOrder (vector<long>) +0x1BC.
// Shape: the type name is an IMPLICIT conversion temporary (`"EXPRESSION"` through the defaulted length, re-addressed with
//        `lea eax,[ebp-4]` after its constructor), built before GetDefinitionManager() is called.
// Name fix (agentNF8 2026-11-11): moved off the local CTCInventoryAbilities_Retail / CDefinitionManager / CLongVector stand-ins onto
// the shared headers: the callees bind ?GetDefinitionManager@CTCBase@@IBEABVCGameDefinitionManager@@XZ (was
// ?GetDefinitions@CTCInventoryAbilities_Retail@@QAEPAVCDefinitionManager@@XZ) and ?resize@?$vector@JV?$allocator@J@std@@@std@@QAEXI@Z
// (was ?resize@CLongVector@@QAEXK@Z); the row's own symbol is the PDB member (was CTCInventoryAbilities_Retail).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTCINVENTORYABILITIES_INITIALISE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER_NOINSTANTIATED
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCInventoryAbilities.h"
#include "engine/CGameDefinitionManager.h"

void CTCInventoryAbilities::InitialiseExpressions()
{
    ReceivedExpressionOrder.resize(GetDefinitionManager().GetNoInstantiatedDefsOfType("EXPRESSION"));
}
