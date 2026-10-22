struct CCombatActionBase {
    char pad[0xc0];
    int strikeCount;
};

bool __fastcall CCombatActionBase_IsStrikeAction(const CCombatActionBase* action)
{
    return action->strikeCount > 0;
}