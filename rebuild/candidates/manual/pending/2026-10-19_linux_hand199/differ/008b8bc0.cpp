// CCreatureAction_ScreamerDrainAttack::Initialise() -- pick the drain-life cast animation: ST_CAST_DRAIN_LIFE_ADVANCE when the live target is farther (in the ground plane) than the creature's combat-type front-near limit, else ST_CAST_DRAIN_LIFE; then the combat-action Initialise
// Grade: DIFFER(231v231) -- register swap only: retail keeps `this` in ebp and the target thing in ebx, ours the reverse (same with an inline helper)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B8BC0, length 240 bytes (plain `ret`, virtual).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PTarget +0xA8 three times, PCreature +8 twice), CThing::GetPosition
// 0x004C73D0 (creature first, then target), CThingCreatureBase::GetCombatTypeDef 0x00666D10, CCharString(const char*, long)
// 0x0099EBF0 / ~CCharString 0x0099EAE0, CCombatActionBase::SetAnimationName(const CCharString&) 0x008574D0,
// CCombatActionBase::Initialise 0x00857520 (direct base call, last).
// Layout: the target pointer is the CCreatureActionBase member at +0xA8; the combat-type def's FrontNearLimit sits at retail
// +0x80 (PDB +0x84: def layouts are 4 bytes smaller in retail). The PDB header for this class flattens its base.
// Shape: the XY delta (target - creature) is a two-float local; the limit is read before the squared length, which is
// evaluated right operand first (Y*Y, then X*X); `limit * limit < lengthSquared` is the `test ah,5 ; jp` ordered
// less-than; the two animation names are if/else blocks whose SetAnimationName call and string destructor tail-merge.
struct C3DVector {
    float X, Y, Z;
    float GetMagnitudeSquared2D() const { return X * X + Y * Y; }
};

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

struct CCombatTypeDef {
    unsigned char _pad0[0x80];
    float         FrontNearLimit;               // +0x80
};

struct CThing {
    unsigned char _pad0[0x91];
    unsigned char m_deleteFlags;                // +0x91 bit 0: being deleted
    const C3DVector& GetPosition() const;       // 0x004C73D0
    CCombatTypeDef* GetCombatTypeDef();         // 0x00666D10 (CThingCreatureBase)
};

struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

struct CCombatActionBase {
    virtual void v00();
    virtual void Initialise();                  // 0x00857520
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CBaseIntelligentPointer PTarget;            // +0xa8
    unsigned char           _padb0[0x114 - 0xb0];
    void SetAnimationName(const CCharString& name);   // 0x008574D0
};

struct CCreatureAction_ScreamerDrainAttack : CCombatActionBase {
    virtual void Initialise();
};

void CCreatureAction_ScreamerDrainAttack::Initialise()
{
    bool advance = false;
    C3DVector delta;
    if (PTarget.GetPItem() && !(PTarget.GetPItem()->m_deleteFlags & 1)) {
        CThing* target = PTarget.GetPItem();
        const C3DVector& from = PCreature.GetPItem()->GetPosition();
        const C3DVector& to = target->GetPosition();
        delta.X = to.X - from.X;
        delta.Y = to.Y - from.Y;
        float limit = PCreature.GetPItem()->GetCombatTypeDef()->FrontNearLimit;
        if (delta.GetMagnitudeSquared2D() > limit * limit)
            advance = true;
    }
    if (advance) {
        CCharString name("ST_CAST_DRAIN_LIFE_ADVANCE", -1);
        SetAnimationName(name);
    }
    else {
        CCharString name("ST_CAST_DRAIN_LIFE", -1);
        SetAnimationName(name);
    }
    CCombatActionBase::Initialise();
}
