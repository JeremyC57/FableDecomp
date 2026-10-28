// CCreatureAction_ScreamerDrainAttack::Initialise() -- pick the drain-life cast animation: ST_CAST_DRAIN_LIFE_ADVANCE when the live target is farther (in the ground plane) than the creature's combat-type front-near limit, else ST_CAST_DRAIN_LIFE; then the combat-action Initialise
// Retry 10b (chain-split sweep, FIXES B-16): the creature is a named local (`CThing* creature = PCreature.GetPItem();`) before its position is taken: retail keeps `this` in ebp and the target in ebx (the chained call swaps them).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B8BC0, length 240 bytes (plain `ret`, virtual).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PTarget +0xA8 three times, PCreature +8 twice), CThing::GetPos
// 0x004C73D0 (creature first, then target), CThingCreatureBase::GetCombatTypeDef 0x00666D10, CCharString(const char*, long)
// 0x0099EBF0 / ~CCharString 0x0099EAE0, CCombatActionBase::SetCombatAnimation(const CCharString&) 0x008574D0,
// CCombatActionBase::Initialise 0x00857520 (direct base call, last).
// Layout: the target pointer is the CCreatureActionBase member at +0xA8; the combat-type def's FrontNearLimit sits at retail
// +0x80 (PDB +0x84: def layouts are 4 bytes smaller in retail). The PDB header for this class flattens its base.
// Shape: the XY delta (target - creature) is a two-float local; the limit is read before the squared length, which is
// evaluated right operand first (Y*Y, then X*X); `limit * limit < lengthSquared` is the `test ah,5 ; jp` ordered
// less-than; the two animation names are if/else blocks whose SetAnimationName call and string destructor tail-merge.
class C3DVector { public:
    float X, Y, Z;
    float GetMagnitudeSquared2D() const { return X * X + Y * Y; }
};

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

class CCombatTypeDef { public:
    unsigned char _pad0[0x80];
    float         FrontNearLimit;               // +0x80
};

class CThing { public:
    unsigned char _pad0[0x91];
    unsigned char m_deleteFlags;                // +0x91 bit 0: being deleted
    const C3DVector& GetPos() const;       // 0x004C73D0
    CCombatTypeDef* GetCombatTypeDef();         // 0x00666D10 (CThingCreatureBase)
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CCombatActionBase { public:
    virtual void v00();
    virtual void Initialise();                  // 0x00857520
    unsigned char           _pad04[4];
    CIntelligentPointer<CThing> PCreature;          // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CIntelligentPointer<CThing> PTarget;            // +0xa8
    unsigned char           _padb0[0x114 - 0xb0];
public:
    void SetCombatAnimation(const CCharString&);   // 0x008574D0
};

class CCreatureAction_ScreamerDrainAttack : public CCombatActionBase { public:
    virtual void Initialise();
};

void CCreatureAction_ScreamerDrainAttack::Initialise()
{
    bool advance = false;
    C3DVector delta;
    if (PTarget.GetPItem() && !(PTarget.GetPItem()->m_deleteFlags & 1)) {
        CThing* target = PTarget.GetPItem();
        CThing* creature = PCreature.GetPItem();
        const C3DVector& from = creature->GetPos();
        const C3DVector& to = target->GetPos();
        delta.X = to.X - from.X;
        delta.Y = to.Y - from.Y;
        float limit = PCreature.GetPItem()->GetCombatTypeDef()->FrontNearLimit;
        if (delta.GetMagnitudeSquared2D() > limit * limit)
            advance = true;
    }
    if (advance) {
        CCharString name("ST_CAST_DRAIN_LIFE_ADVANCE", -1);
        SetCombatAnimation(name);
    }
    else {
        CCharString name("ST_CAST_DRAIN_LIFE", -1);
        SetCombatAnimation(name);
    }
    CCombatActionBase::Initialise();
}