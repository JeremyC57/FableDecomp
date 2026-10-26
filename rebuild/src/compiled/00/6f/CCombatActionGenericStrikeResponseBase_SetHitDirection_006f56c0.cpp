class C3DVector { public:
    float x, y, z;
};

class CCombatAction_GenericStrikeResponseBase { public:
    char pad[0x128];
    C3DVector m_hitDir;
    void SetHitDirection(C3DVector v);
};

void CCombatAction_GenericStrikeResponseBase::SetHitDirection(C3DVector v)
{
    m_hitDir = v;
}