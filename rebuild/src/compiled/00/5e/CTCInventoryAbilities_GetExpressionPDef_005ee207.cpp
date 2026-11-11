// CTCInventoryAbilities::GetExpressionPDef(long) const -- looks up the definition with the given global index and returns it as a counted CDefPointer<const CExpressionDef>
// Leaf: ?GetExpressionPDef@CTCInventoryAbilities@@QBE?AV?$CDefPointer@$$CBVCExpressionDef@@@@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EE207, length 68 bytes.
// Identity: Ego_r.pdb + FableWin.pdb ?GetExpressionPDef@CTCInventoryAbilities@@QBE?AV?$CDefPointer@$$CBVCExpressionDef@@@@J@Z.
// Callees: CTCBase::GetDefinitionManager 0x006869D0, CDefinitionManager::GetPDefFromGlobalIndex(long) const 0x009AD9E0
//          (CDefPointer<const CDefClassBase> returned through the hidden pointer, which VC homes in the dead `index` slot [ebp+0Ch]);
//          the def's virtual Delete at a zero count.
// Pragmas: frame-pointer size TU (`optimize("y", off)` + `("s", on)`).
// Shape: the base-typed temporary is converted into the typed result (the shared CDefPointer converting constructor, add-ref when
//        non-null), then the temporary's destructor releases it; the arguments of GetPDefFromGlobalIndex are pushed before the
//        GetDefinitionManager() call that yields its `this`.
// Name fix (agentNF8 2026-11-11): moved off the local CTCInventoryAbilities_Retail / CDefinitionManager / CDefPointer stand-ins onto
// the shared headers: the callees bind ?GetDefinitionManager@CTCBase@@IBEABVCGameDefinitionManager@@XZ (was
// ?GetDefinitions@CTCInventoryAbilities_Retail@@QAEPAVCDefinitionManager@@XZ) and the const
// ?GetPDefFromGlobalIndex@CDefinitionManager@@QBE?AV?$CDefPointer@$$CBVCDefClassBase@@@@J@Z (was the QAE CDefPointer<CDefClassBase>
// spelling); the row's own symbol is the PDB const member (was CTCInventoryAbilities_Retail, non-const, CDefPointer<CExpressionDef>).
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
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCInventoryAbilities.h"
#include "engine/CGameDefinitionManager.h"

CDefPointer<const CExpressionDef> CTCInventoryAbilities::GetExpressionPDef(long index) const
{
    return CDefPointer<const CExpressionDef>(GetDefinitionManager().GetPDefFromGlobalIndex(index));
}
