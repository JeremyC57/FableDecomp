// Leaf: ?CCreatureAction_PerformExpressionExtended_IsToReplaceItemIfRemoved@@YIEPAUS@@@Z
struct Inner { char pad[0x86]; unsigned char f; };
struct S { char pad[0xA8]; Inner* p; };
unsigned char __fastcall CCreatureAction_PerformExpressionExtended_IsToReplaceItemIfRemoved(S* action)
{
    return action->p->f;
}