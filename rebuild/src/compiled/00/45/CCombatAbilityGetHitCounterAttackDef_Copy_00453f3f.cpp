class CDefClassBase { public:
    unsigned char _b[0x28];
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};
struct CMemberDef {
    unsigned char _m[0x20];
    void Copy(CMemberDef* other);
};
class CCombatAbilityGetHitCounterAttackDef : public CDefClassBase { public:
    CMemberDef m_member; // at +0x28
    unsigned char _tail[0x48 - 0x28 - 0x20];
    void Copy(CCombatAbilityGetHitCounterAttackDef* other);
};

void CCombatAbilityGetHitCounterAttackDef::Copy(CCombatAbilityGetHitCounterAttackDef* other)
{
    CDefClassBase::operator=(*(const CDefClassBase*)other);
    this->m_member.Copy(&other->m_member);
}