#include "engine/CCreatureAction_PerformExpression.h"
#include "rebuild_abi.h"
// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `p` -> `PExpressionDef_Object` (+0xa8).

extern const char kExprName_00842c00[];
struct CExprField { void Assign(const char *name); };
struct CCreatureAction_PerformExpression_Methods : CCreatureAction_PerformExpression {
    void PreventExpression();
};

void CCreatureAction_PerformExpression_Methods::PreventExpression()
{
    ((CExprField*)((char*)PExpressionDef_Object + 0x78))->Assign(kExprName_00842c00);
}
