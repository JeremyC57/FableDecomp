class CDefClassBase { public:
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};

struct CMemberDef {
    void Copy_00461bb7(const CMemberDef* other);
};

class CCombatAbilityFlourishCounterAttackDef { public:
    unsigned char _base[0x28];
    CMemberDef m_member; // at +0x28
    unsigned char _tail[0x18 - sizeof(CMemberDef)];

    void Copy(const CCombatAbilityFlourishCounterAttackDef* other);
};

void CCombatAbilityFlourishCounterAttackDef::Copy(
        const CCombatAbilityFlourishCounterAttackDef* other)
{
    ((CDefClassBase*)this)->operator=(*(const CDefClassBase*)other);
    m_member.Copy_00461bb7(&other->m_member);
}