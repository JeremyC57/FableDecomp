// Leaf: ?IsStrikeAction@CCombatActionBase@@UBE_NXZ
class CCombatActionBase { public:
    char pad[0xbc];
    int strikeCount;
    public: virtual bool IsStrikeAction() const;
};

bool CCombatActionBase::IsStrikeAction() const
{
    const CCombatActionBase* action = this;
    return action->strikeCount > 0;
}