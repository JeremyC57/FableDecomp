// Leaf: ?GetAnimExtraFlags@CCombatActionBase@@UBEKXZ
typedef unsigned long ulong;
class CCombatActionBase { public:
    public: virtual unsigned long GetAnimExtraFlags() const;
};
unsigned long CCombatActionBase::GetAnimExtraFlags() const
{
    const CCombatActionBase* action = this;
    (void)action;
    return 8u;
}