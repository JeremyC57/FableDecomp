// CAIStateGroup_MinionReactToEnemy::Update() -- while the enemy is alive and still a target to attack: back off (to 2.4 units beyond the creature, away from the enemy) when the enemy is within 5 units in XY, then keep looking at the enemy; terminate when the enemy dies, stops being a target, or a move or look fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00937DA0, length 450 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PEnemy), IsToAttackTarget(CThingAICreature*, CThingCreatureBase*)
// @ 0x00864D00 (__fastcall), CThing::PeekPos @ 0x004C73D0 (the XY distance helper), CThingCreatureBase::PeekPos
// @ 0x006A4D00, C3DVector::AccurateNormalise @ 0x00A14440, AIAction_MoveToPosProximity @ 0x0097E890, CBaseIntelligentPointer ctor
// @ 0x00A01B10 / SetPItem @ 0x00A01B90 (CIntelligentPointer<const CThing> vtable 0x01237F1C, by value),
// AIAction_LookAtThing @ 0x0097D6F0, Terminate @ 0x008FCF40.
// Data: 25.0f @ 0x0123003C, 2.4f @ 0x0129234C.
// Shape: the XY distance is an inline helper on CThing* (position references taken second-argument first, explicit dx/dy)
// stored to a float local before the `< 25` test (the x87 temporaries are popped first); C3DVector has a user copy
// constructor, so `creature position + away` is built straight into the argument slot; the back-off is `away *= 2.4f`
// in place after the out-of-line Normalise; the look target converts through a template copy constructor.
// Layout: Ego_r.pdb CAIStateGroup_MinionReactToEnemy (PEnemy +0x14, CIntelligentPointer<CThingCreatureBase>).
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector& operator*=(float s) { X *= s; Y *= s; Z *= s; return *this; }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    float GetLengthSquaredXY() const { return X * X + Y * Y; }
    C3DVector& AccurateNormalise();                           // 0x00A14440 (Ego_r ?AccurateNormalise@C3DVector@@QAEAAV1@XZ)
};
class CThing { public:
    unsigned char _pad0[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPos() const;    // 0x004C73D0
};
class CThingPhysical : public CThing { public:
    const C3DVector& PeekPos() const;       // 0x006A4D00
};
class CThingCreatureBase : public CThingPhysical { public:
};
class CThingAICreature : public CThingCreatureBase { public: };
bool __fastcall IsToAttackTarget(CThingAICreature* creature, CThingCreatureBase* target);   // 0x00864D00
inline float GetDistanceSquaredXY(const CThing* a, const CThing* b)
{
    const C3DVector& posB = b->GetPos();
    const C3DVector& posA = a->GetPos();
    float dx = posA.X - posB.X;
    float dy = posA.Y - posB.Y;
    return dx * dx + dy * dy;
}

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(const CThing* item);          // 0x00A01B90
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual void v00();
    template<class U> CIntelligentPointer(const CIntelligentPointer<U>& other) { SetPItem(other.GetPItem()); }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
inline bool IsThingAlive(const CBaseIntelligentPointer& thing) { return thing.GetPItem() != 0 && !(thing.GetPItem()->m_flags91 & 1); }

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingAICreature* PCreature;                // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];                 // +0x08..+0x13
    CThingAICreature* GetCreature() const { return PBrain->PCreature; }
    void Terminate();                           // 0x008FCF40
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float proximity, bool& arrived, long movement,
                                     bool a, bool b, bool avoidDynamicObstacles, const CThing* ignore, bool c, float d);   // 0x0097E890
    long AIAction_LookAtThing(long repeat, CIntelligentPointer<const CThing> thing, long frames, float speed);   // 0x0097D6F0
};
struct CAIStateGroup_MinionReactToEnemy : CAIStateGroupBase {
    CIntelligentPointer<CThingCreatureBase> PEnemy;   // +0x14
    virtual void Update();
};

void CAIStateGroup_MinionReactToEnemy::Update()
{
    for (;;) {
        if (!IsThingAlive(PEnemy))
            break;
        if (!IsToAttackTarget(GetCreature(), PEnemy.GetPItem()))
            break;
        float distanceSquared = GetDistanceSquaredXY(GetCreature(), PEnemy.GetPItem());
        if (distanceSquared < 25.0f) {
            C3DVector away = GetCreature()->PeekPos() - PEnemy.GetPItem()->PeekPos();
            away.AccurateNormalise();
            away *= 2.4f;
            bool arrived;
            if (AIAction_MoveToPosProximity(1, GetCreature()->PeekPos() + away, 0.5f, arrived, 3, true, false, true, 0,
                                            true, 0.0f) != 2)
                break;
        }
        if (!IsThingAlive(PEnemy))
            break;
        long result = AIAction_LookAtThing(2, PEnemy, 0xb, 0.0027777f);
        if (result != 4 && result != 2)
            break;
    }
    Terminate();
}