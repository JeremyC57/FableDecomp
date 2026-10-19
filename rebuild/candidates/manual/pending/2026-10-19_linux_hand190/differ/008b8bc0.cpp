// CCreatureAction_ScreamerDrainAttack::Initialise() -- animation ST_CAST_DRAIN_LIFE_ADVANCE when the live target is farther (in XY) than the owner's combat-type range, else ST_CAST_DRAIN_LIFE; then the base Initialise
// Grade: DIFFER(231v231) -- register swap only: retail keeps this in ebp and the target in ebx, ours this in ebx / target in ebp
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B8BC0, length 231 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::GetPosition @ 0x004C73D0,
// CThingCreatureBase::GetCombatTypeDef @ 0x00666D10, CCharString::CCharString(const char*, long) @ 0x0099EBF0,
// CCombatActionBase::SetAnimationName @ 0x008574D0, CCharString::~CCharString @ 0x0099EAE0,
// CCombatActionBase::Initialise @ 0x00857520.
// Shape: an inline range predicate (target present, not dead -- thing byte +0x91 bit 0 -- and the squared XY distance
// beyond the squared range) chooses one of two block-scoped strings; both arms share the SetAnimationName/destructor
// tail. The delta is a 2D vector built in the frame; the def's range (+0x80) is loaded before the squares.
// Layout: owner CIntelligentPointer at +8, target at +0xa8.
struct C3DVector { float X, Y, Z; };
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
};

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

struct CCombatTypeDef {
    unsigned char _pad[0x80];
    float         Range;                               // +0x80
};

struct CThing {
    unsigned char _pad0[0x91];
    unsigned char m_flags91;                           // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;              // 0x004C73D0
};
struct CThingCreatureBase : CThing {
    CCombatTypeDef* GetCombatTypeDef();                // 0x00666D10
};

struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                          // 0x00A01B50
};

class CCombatActionBase {
public:
    virtual void Initialise();                         // 0x00857520
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;                 // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CBaseIntelligentPointer PTarget;                   // +0xa8
    unsigned char           _padb0[0x114 - 0xb0];
    void SetAnimationName(const CCharString& name);    // 0x008574D0
};

class CCreatureAction_ScreamerDrainAttack : public CCombatActionBase {
public:
    virtual void Initialise();
    __forceinline bool IsTargetOutOfRange() const
    {
        if (PTarget.GetPItem() && !(PTarget.GetPItem()->m_flags91 & 1)) {
            CThing* target = PTarget.GetPItem();
            const C3DVector& ownerPos = PCreature.GetPItem()->GetPosition();
            const C3DVector& targetPos = target->GetPosition();
            C3DVector delta;
            delta.X = targetPos.X - ownerPos.X;
            delta.Y = targetPos.Y - ownerPos.Y;
            float range = ((CThingCreatureBase*)PCreature.GetPItem())->GetCombatTypeDef()->Range;
            if (delta.X * delta.X + delta.Y * delta.Y > range * range)
                return true;
        }
        return false;
    }
};

void CCreatureAction_ScreamerDrainAttack::Initialise()
{
    if (IsTargetOutOfRange()) {
        CCharString name("ST_CAST_DRAIN_LIFE_ADVANCE", -1);
        SetAnimationName(name);
    }
    else {
        CCharString name("ST_CAST_DRAIN_LIFE", -1);
        SetAnimationName(name);
    }
    CCombatActionBase::Initialise();
}
