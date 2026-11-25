// Leaf: ?PreventSneaking@CCreatureAction_PerformExpression_Methods@@QAEXXZ
// Literal (NF418, 2026-11-25): was extern data `kExprName_00842c00` (table ?kExprName_00842c00@@3QBDB -> 0x0125EAC8); now spelled at the use site:
//   "Expression_Picklock", retail's pooled literal at 0x0125EAC8 (/GF-merged ??_C@, NF418 CLASSIFICATION.tsv).
#include "engine/CCreatureAction_PerformExpression.h"
#include "rebuild_abi.h"
// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `p` -> `PExpressionDef_Object` (+0xa8).

struct CExprField { void Assign(const char *name); };
struct CCreatureAction_PerformExpression_Methods : CCreatureAction_PerformExpression {
    void PreventSneaking();
};

void CCreatureAction_PerformExpression_Methods::PreventSneaking()
{
    ((CExprField*)((char*)PExpressionDef_Object + 0x78))->Assign("Expression_Picklock");
}
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?PreventSneaking@CCreatureAction_PerformExpression@@UBE_NXZ); evidence: RTTI vtable slot + PDB name set.
