// CCombatAction_KickThingOnGround::Initialise() -- play the stealth kick on a living type-13 target (aim 1.0 forward / 0.15 aside) or the hard low kick (0.0 / 0.1); against a living target suspend its animation (type 1), make it and the kicker not solid to each other and aim at its closest hit-location point (else its position); MoveToPos keeps the kicker's height and is pushed along and beside the kicker's back-facing direction, then the base Initialise runs
// Leaf: ?Initialise@CCombatAction_KickThingOnGround
// Retail: Fable.exe (TLC, VC7.1) @ 0x0087E860, 880 bytes, plain `ret` (virtual, vtable 0x01277D3C).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PCreature +8, PTarget +0xa8), CCharString ctor/dtor 0x0099EBF0 /
//   0x0099EAE0, CCombatActionBase::SetAnimationName 0x008574D0 and Initialise 0x00857520, CThing::GetPosition 0x004C73D0,
//   the physics virtual slot 70 GetFacing (+0x118), CThingAICreature::SetSuspendAnimation 0x00833A10, CThing::AddTC
//   0x004C9D60 ("CTCSolidToThing"), CTCSolidToThing::AddThingNotSolidTo 0x00770FA0, the interface-map LowerBound
//   0x004365B0 (interface 0x7F), its component's hit-location getter 0x00814BF0 (`mov eax,[ecx+18h]`),
//   CHitLocation::GetClosestPointToPos2D 0x00815370 (hidden C3DVector result, position and direction by reference),
//   C3DVector::AccurateNormalise @ 0x00A14440 (fsqrt form). Data: epsilon 1e-4f 0x0129BA3C (read as a variable, squared at run time).
// Layout: retail CCombatActionBase is 0x114 bytes (PTarget +0xa8); Ego_r.pdb KickThingOnGround MoveToPos / Movement /
//   MaxRunningSpeed at PDB +0x12c/+0x138/+0x144 sit 0x18 lower in retail (+0x114/+0x120/+0x12c, cf. the landed ctor
//   0x0087FD20). CThing: ThingType byte +0x10, interface bits +0x20, map +0x44, physics +0x60, dead flag +0x91 bit 0.
// Shape: the animation names are block-scoped so each arm destroys its string before the two aim factors are stored; the
//   facing is a named local negated through the inline operator- (frame 0x58); C3DVector has a user copy ctor, so the first
//   `position` copy drops its dead Z word; the suspend target is a named local (GetPItem before the argument pushes); the
//   2D length test is `X*X + Y*Y > eps*eps` after `direction.Z = 0`; the aim is `direction * forward + position +
//   C3DVector(direction.Y, -direction.X, 0.0f) * side` (the perpendicular term first, then the forward sum).
extern float g_epsilon_0129ba3c;                        // 0x0129BA3C (1e-4f)

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator-() const { return C3DVector(-X, -Y, -Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector& AccurateNormalise();                                   // 0x00A14440 (Ego_r ?AccurateNormalise@C3DVector@@QAEAAV1@XZ)
};

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;               // 0x004365B0
};
struct CHitLocation {
    C3DVector GetClosestPointToPos2D(const C3DVector& position, const C3DVector& direction);   // 0x00815370
};
class CTCHitLocations { public:                                // interface 0x7F
    CHitLocation* GetHitLocation();                     // 0x00814BF0
};
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
class CTCPhysicsBase { public:
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6)
    virtual C3DVector GetFacing();                      // slot 70 (+0x118)
};
class CThing;
class CTCSolidToThing { public:
    void AddThingNotSolidTo(CThing* thing);             // 0x00770FA0
};
class CTCParamsBase;
class CThing { public:
    unsigned char   _pad0[0x10];
    unsigned char   ThingType;                          // +0x10
    unsigned char   _pad11[0x20 - 0x11];
    unsigned long   m_presentBits[9];                   // +0x20
    CInterfaceMap   m_interfaces;                       // +0x44
    unsigned char   _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* m_pPhysics;                         // +0x60
    unsigned char   _pad64[0x91 - 0x64];
    unsigned char   m_flags91;                          // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;               // 0x004C73D0
    CTCSolidToThing* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
    CTCPhysicsBase* GetPhysics() const { return m_pPhysics; }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
class CThingAICreature : public CThing { public:
    void SetSuspendAnimation(bool suspend, void* suspender);   // 0x00833A10
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
__forceinline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    return p.GetPItem() && !(p.GetPItem()->m_flags91 & 1);
}

class CCombatActionBase {
public:
    virtual void Initialise();                          // 0x00857520
    unsigned char           _pad04[4];
    CIntelligentPointer<CThing> PCreature;                  // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CIntelligentPointer<CThing> PTarget;                    // +0xa8
    unsigned char           _padb0[0x114 - 0xb0];
    void SetAnimationName(const CCharString& name);     // 0x008574D0
};
class CCombatAction_KickThingOnGround : public CCombatActionBase {
public:
    C3DVector MoveToPos;                                // +0x114
    C3DVector Movement;                                 // +0x120
    float     MaxRunningSpeed;                          // +0x12c
    virtual void Initialise();
};

void CCombatAction_KickThingOnGround::Initialise()
{
    float forward;
    float side;
    if (IS_THING_ALIVE(PTarget) && PTarget.GetPItem()->ThingType == 13) {
        {
            CCharString name("STRIKE_STEALTH_KICK_THING_ON_GROUND");
            SetAnimationName(name);
        }
        forward = 1.0f;
        side = 0.15f;
    } else {
        {
            CCharString name("STRIKE_HARD_LOW_KICK");
            SetAnimationName(name);
        }
        forward = 0.0f;
        side = 0.1f;
    }
    CTCPhysicsBase* physics = PCreature.GetPItem()->GetPhysics();
    C3DVector position = PCreature.GetPItem()->GetPosition();
    C3DVector facing = physics->GetFacing();
    C3DVector direction = -facing;
    if (IS_THING_ALIVE(PTarget)) {
        if (PTarget.GetPItem()->ThingType == 1) {
            CThingAICreature* creature = (CThingAICreature*)PTarget.GetPItem();
            creature->SetSuspendAnimation(true, 0);
        }
        CTCSolidToThing* targetSolid;
        {
            CCharString name("CTCSolidToThing");
            targetSolid = PTarget.GetPItem()->AddTC(name, true, 0);
        }
        CTCSolidToThing* creatureSolid;
        {
            CCharString name("CTCSolidToThing");
            creatureSolid = PCreature.GetPItem()->AddTC(name, true, 0);
        }
        targetSolid->AddThingNotSolidTo(PCreature.GetPItem());
        creatureSolid->AddThingNotSolidTo(PTarget.GetPItem());
        position = PTarget.GetPItem()->GetPosition();
        CTCHitLocations* hitLocations;
        if (PTarget.GetPItem()->GetInterface(&hitLocations, 0x7f)) {
            CHitLocation* location = hitLocations->GetHitLocation();
            if (location)
                position = location->GetClosestPointToPos2D(PCreature.GetPItem()->GetPosition(), direction);
        }
    }
    position.Z = PCreature.GetPItem()->GetPosition().Z;
    MoveToPos = position;
    direction.Z = 0.0f;
    if (direction.X * direction.X + direction.Y * direction.Y > g_epsilon_0129ba3c * g_epsilon_0129ba3c) {
        direction.AccurateNormalise();
        MoveToPos = direction * forward + position + C3DVector(direction.Y, -direction.X, 0.0f) * side;
    }
    CCombatActionBase::Initialise();
}