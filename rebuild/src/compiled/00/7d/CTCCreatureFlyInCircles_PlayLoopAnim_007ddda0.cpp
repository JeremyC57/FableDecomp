// CTCCreatureFlyInCircles::PlayLoopAnim() -- point the wind along the creature's flat heading (towards the living hero, else its facing) and start the dragon-flight loop animation for the current fly mode (strafe at half wind, fly loop, or fire-breathing at 1.5 wind early in a swoop), clearing the transition flag once the action is taken
// Leaf: ?PlayLoopAnim@CTCCreatureFlyInCircles@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DDDA0, 496 bytes, plain `ret`.
// Callees: CCharString() 0x0099E4B0 / operator=(const char*) 0x0099EFE0 ("STRAFE_RIGHT" 0x0126FAC8, "FLY_LOOP" 0x0126FAD8,
//   "BREATHE_FIRE_STRAIGHT_LOOP" 0x0126FAE4) / ~CCharString 0x0099EAE0, CTCBase::PeekWorld 0x00686A80 (catalog
//   "CViewBase::GetDisplayEngine"), CWorld::PeekMyPlayer 0x0049D9D0, CPlayer::GetPControlledCreature 0x00487DC0,
//   CThing::GetPos 0x004C73D0, the physics body's facing (vtable +0x118), CTCBase::GetWorld 0x00686A70,
//   CEnvironment::OverrideWindDirection 0x006B2B10, CCreatureAction_DragonFlight ctor 0x008F4CB0 / temporary dtor
//   0x0062E900 (~CCombatActionBase), CThingCreatureBase::SetCurrentAction 0x006644F0.
// Layout: CTCCreatureFlyInCircles Creature +0x0c, Mode +0x34, Transitioning +0x38, SwoopProgress +0x6c; CThing physics
//   body +0x60, dead bit +0x91 bit 0; CWorld environment +0x18; the fast inverse-sqrt mantissa table 0x013CE558.
// Shape: inline Normalise over local copies (the Z = 0 term folds away, Z = r * 0); the SetCurrentAction result is a named
//   bool so it is kept in bl across the temporary's destructor; the world is a named local so GetWorld is called before
//   the OverrideWindDirection argument pushes.
extern unsigned long g_InvSqrtMantissaTable[128];       // 0x013CE558

class C3DVector { public:
    float X;    // +0x00
    float Y;    // +0x04
    float Z;    // +0x08
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    __forceinline void Normalise()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = x * x + y * y + z * z;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r * 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
};
inline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }

class CCharString { public:
    void* PStringData;
    CCharString();                                         // 0x0099E4B0
    ~CCharString();                                        // 0x0099EAE0
    const CCharString& operator=(const char* text);              // 0x0099EFE0
};

struct CPhysicsBody {                                      // the thing's physics body at +0x60
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF10(6)
    virtual C3DVector GetFacing();                         // slot 70 (+0x118)
};
class CCreatureActionBase;
class CThing { public:
    unsigned char _pad00[0x60];
    CPhysicsBody* PhysicsBody;                             // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char StateBits;                               // +0x91 bit 0: dead
    const C3DVector& GetPos() const;                  // 0x004C73D0
    bool IsDead() const { return (StateBits & 1) != 0; }
};
class CThingPlayerCreature : public CThing { };
class CThingCreatureBase : public CThing { public:
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};
class CCreatureActionBase { public: };
class CCombatActionBase : public CCreatureActionBase { public:
    unsigned char _body[0x11c];
    ~CCombatActionBase();                                  // 0x0062E900
};
class CCreatureAction_DragonFlight : public CCombatActionBase { public:
    CCreatureAction_DragonFlight(CThingCreatureBase* creature, const CCharString& animation);   // 0x008F4CB0
};

class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                   // 0x00487DC0
};
class CEnvironment { public:
    void OverrideWindDirection(const C3DVector& direction, float speed);   // 0x006B2B10
};
class CWorld { public:
    unsigned char _pad00[0x18];
    CEnvironment* Environment;                             // +0x18
    CPlayer* PeekMyPlayer() const;                         // 0x0049D9D0
};

enum EFlyMode { FLY_MODE_STRAFE, FLY_MODE_LOOP, FLY_MODE_BREATHE_FIRE };

class CTCBase
{
protected:
    CWorld& GetWorld() const;   // 0x00686A70
    const CWorld& PeekWorld() const;   // 0x00686A80
};

struct CTCCreatureFlyInCircles : public CTCBase {
    void*               __vftable;                         // +0x00
    CThing*             ParentThing;                       // +0x04 (CTCBase)
    unsigned char       _pad08[0x0c - 0x08];
    CThingCreatureBase* Creature;                          // +0x0c
    unsigned char       _pad10[0x34 - 0x10];
    EFlyMode            Mode;                              // +0x34
    bool                Transitioning;                     // +0x38
    unsigned char       _pad39[0x6c - 0x39];
    float               SwoopProgress;                     // +0x6c

protected:
    void PlayLoopAnim();
};

void CTCCreatureFlyInCircles::PlayLoopAnim()
{
    CCharString animation;
    float windSpeed = 0.0f;
    C3DVector direction;
    CThing* hero = PeekWorld().PeekMyPlayer()->GetPControlledCreature();
    if (hero && !hero->IsDead())
        direction = hero->GetPos() - Creature->GetPos();
    else
        direction = Creature->PhysicsBody->GetFacing();
    direction.Z = 0.0f;
    direction.Normalise();
    switch (Mode) {
    case FLY_MODE_STRAFE:
        animation = "STRAFE_RIGHT";
        windSpeed = 0.5f;
        break;
    case FLY_MODE_LOOP:
        animation = "FLY_LOOP";
        break;
    case FLY_MODE_BREATHE_FIRE:
        animation = "BREATHE_FIRE_STRAIGHT_LOOP";
        if (SwoopProgress < 0.6f)
            windSpeed = 1.5f;
        break;
    }
    CWorld* world = (&GetWorld());
    world->Environment->OverrideWindDirection(direction, windSpeed);
    bool started = Creature->SetCurrentAction(CCreatureAction_DragonFlight(Creature, animation));
    if (started && Transitioning)
        Transitioning = false;
}