// Leaf: ?IsMeleeFlourish@CCombatActionBase@@MBE_NXZ
class CCombatActionBase
{
public:
    char pad[0xb4];
    unsigned long m_meleeFlourish;

    protected:
    virtual bool IsMeleeFlourish() const;
    public:
};

bool CCombatActionBase::IsMeleeFlourish() const
{
    if (m_meleeFlourish)
        return true;
    return false;
}