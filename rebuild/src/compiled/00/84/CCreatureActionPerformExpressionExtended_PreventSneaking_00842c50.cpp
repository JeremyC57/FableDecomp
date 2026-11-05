// Leaf: ?PreventSneaking@CCreatureAction_PerformExpressionExtended_Methods@@QAEXXZ
#include "engine/CCreatureAction_PerformExpressionExtended.h"
#include "rebuild_abi.h"
// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `p` -> `PExpressionDef_Object` (+0xa8).

extern const char kExprName_00842c50[];
struct CExprField { void Assign(const char *name); };
struct CCreatureAction_PerformExpressionExtended_Methods : CCreatureAction_PerformExpressionExtended {
    void PreventSneaking();
};

void CCreatureAction_PerformExpressionExtended_Methods::PreventSneaking()
{
    ((CExprField*)((char*)PExpressionDef_Object + 0x78))->Assign(kExprName_00842c50);
}
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?PreventSneaking@CCreatureAction_PerformExpressionExtended@@UBE_NXZ); evidence: RTTI vtable slot + PDB name set.
