// Leaf: ?CCreatureAction_PerformExpression_IsToHaveEmptyHands@@YIEPAUS@@@Z
struct Inner { char pad[0x85]; unsigned char f; };
struct S { char pad[0xA8]; Inner* p; };
unsigned char __fastcall CCreatureAction_PerformExpression_IsToHaveEmptyHands(S* action)
{
    return action->p->f;
}