// Leaf: ?CCreatureAction_PerformExpression_IsToReplaceItemIfRemoved@@YIEPAUS@@@Z
struct Inner { char pad[0x86]; unsigned char f; };
struct S { char pad[0xA8]; Inner* p; };
unsigned char __fastcall CCreatureAction_PerformExpression_IsToReplaceItemIfRemoved(S* action)
{
    return action->p->f;
}