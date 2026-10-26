class C3DVector { public:
    float x;
    float y;
    float z;
};

class CCombatAction_GenericStrikeResponseBase { public:
    char pad[0x11c];
    C3DVector hitPos;

    void SetHitPosition(C3DVector pos);
};

void CCombatAction_GenericStrikeResponseBase::SetHitPosition(C3DVector pos)
{
    this->hitPos = pos;
}