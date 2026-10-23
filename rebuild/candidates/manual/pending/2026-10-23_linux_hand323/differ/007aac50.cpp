// CTCAssassinRush::RushForwards() -- one frame of the assassin rush: aim (override, target or forward), face it, and while inside the rush range either strike a live reachable target (turn it, face it, set the stun timer and slowdown, then hit) or step forward following the ground; stop with the miss sound and the queued recovery action at walls, nav blocks or the range end, and keep the radial blur on the rusher
// Grade: DIFFER(1285v1336) -- first full draft (aim, face, range, strike with stun and slowdown, ground probe, nav block, collision sound, recovery action, step with radial blur); retail frame 0x54 v our 0x64 and the x87 range/probe compares and block order differ
// Leaf: RushForwards
// Retail: Fable.exe (TLC, VC7.1) @ 0x007AAC50, 1344 bytes, `ret`.
// Literals: "HERO_ABILITY_ASSASSIN_RUSH_COLLISION" 0x0126CFC4; 0.3f 0x01238170, 0x0126CEF8, 0x01230A08.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct C2DCoordI { long X, Y; };
struct SOrientation { C3DVector Forward; C3DVector Up; };
struct CThingController {
    virtual void s000(); virtual void s004(); virtual void s008(); virtual void s00c(); virtual void s010(); virtual void s014(); virtual void s018(); virtual void s01c();
    virtual void s020(); virtual void s024(); virtual void s028(); virtual void s02c(); virtual void s030(); virtual void s034(); virtual void s038(); virtual void s03c();
    virtual void s040(); virtual void s044(); virtual void s048(); virtual void s04c(); virtual void s050(); virtual void s054(); virtual void s058(); virtual void s05c();
    virtual void s060(); virtual void s064(); virtual void s068(); virtual void s06c(); virtual void s070(); virtual void s074(); virtual void SetTargetPosition(const C3DVector& position); virtual void s07c();
    virtual void s080(); virtual void s084();
    virtual void SetPosition(const C3DVector& position, bool update);   // +0x88
    virtual void s08c(); virtual void s090(); virtual void s094(); virtual void s098(); virtual void s09c();
    virtual void s0a0(); virtual void s0a4(); virtual void s0a8(); virtual void s0ac(); virtual void s0b0(); virtual void s0b4(); virtual void s0b8(); virtual void s0bc();
    virtual void s0c0(); virtual void s0c4(); virtual void s0c8(); virtual void s0cc(); virtual void s0d0(); virtual void s0d4(); virtual void s0d8(); virtual void s0dc();
    virtual void s0e0(); virtual void s0e4(); virtual void s0e8(); virtual void s0ec(); virtual void s0f0(); virtual void s0f4(); virtual void s0f8(); virtual void s0fc();
    virtual void s100(); virtual void s104(); virtual void s108(); virtual void s10c(); virtual void s110(); virtual void s114();
    virtual C3DVector GetForward() const;               // +0x118
    virtual void s11c(); virtual void s120(); virtual void s124();
    virtual void SetOrientation(const SOrientation& orientation);   // +0x128
    unsigned char _pad04[0x18 - 0x04];
    C3DVector Velocity;                                 // +0x18
};
struct CThing {
    unsigned char _pad00[0x10];
    unsigned char Mode;                                 // +0x10
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];                     // +0x20
    struct CInterfaceMap { struct SEntry { long type; void* component; }; SEntry* first; SEntry* last; SEntry* LowerBound(const long* type); } m_interfaces;   // +0x44 (0x0040F020)
    unsigned char _pad4c[0x60 - 0x4c];
    CThingController* Controller;                       // +0x60
    unsigned char _pad64[0x6c - 0x64];
    unsigned char Flags6c;                              // +0x6c
    unsigned char _pad6d[0x91 - 0x6d];
    unsigned char m_flags;                              // +0x91
    const C3DVector& GetPosition() const;               // 0x004C73D0
    float GetGroundHeight(const C3DVector& position, bool a, bool b);   // 0x00666D40
    bool IsDead() const { return (m_flags & 1) != 0; }
    void Reevaluate();                                  // 0x00662650
    bool AddAction(void* action);                       // 0x006644F0
    void StartReacting();                               // 0x006D5AB0
    template <class T> __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CBaseIntelligentPointer {
    void* __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                           // 0x00A01B50
};
struct CNavigationMap { bool IsBlocked(); };            // 0x004FCBD0
struct CRadialBlur { void SetPosition(void* key, const C3DVector& position); };   // 0x006B8310
struct CEngineHolder { unsigned char _pad[0x1964]; CRadialBlur* RadialBlur; };
struct CBulletTimeManager { void AddBulletTimeSlowdown(long multiplier, void* key, float amount); };   // 0x004C6170
struct CWorld {
    virtual void s00(); virtual void s04(); virtual void s08(); virtual void s0c(); virtual void s10(); virtual void s14(); virtual void s18(); virtual void s1c();
    virtual void s20(); virtual void s24(); virtual void s28(); virtual void s2c(); virtual void s30();
    virtual CNavigationMap* GetNavAt(const C2DCoordI& position);   // +0x34
    unsigned char _pad04[0x18 - 0x04];
    CEngineHolder* Engine;                              // +0x18
    unsigned char _pad1c[0x68 - 0x1c];
    CBulletTimeManager* BulletTime;                     // +0x68
};
struct CTCSoundPlayer { void PlayAttachedCriteria(const CCharString& name, bool a, long b, long c, long d); };   // 0x0066BD00
struct SOrientationFrom { unsigned char _data[0x18]; SOrientationFrom(const C3DVector& up, const C3DVector& forward); };   // 0x00487050
struct CAssassinRushDef {
    unsigned char _pad00[0x44];
    float BlurHeight;                                   // +0x44
    unsigned char _pad48[0x68 - 0x48];
    float Speed;                                        // +0x68
    unsigned char _pad6c[0x70 - 0x6c];
    float StunTime;                                     // +0x70
    unsigned char _pad74[0x78 - 0x74];
    long SlowdownMultiplier;                            // +0x78
};
extern long g_FramesPerSecond;                          // 0x01375550
extern const float g_StepCheckScale;                    // 0x01238170
extern const float g_MaxStepDown;                       // 0x0126CEF8
extern const float g_BlurLag;                           // 0x01230A08
#pragma pack(push, 1)
class CTCAssassinRush {
public:
    void* __vftable;                                    // +0x00
    CThing* ParentThing;                                // +0x04
    unsigned char _pad08[0x0c - 0x08];
    CBaseIntelligentPointer Target;                     // +0x0c
    CAssassinRushDef* PDef;                             // +0x14
    unsigned char _pad18[0x2c - 0x18];
    bool Finished;                                      // +0x2c
    C3DVector StartPosition;                            // +0x2d
    CBaseIntelligentPointer StrikeTarget;               // +0x39
    bool Moving;                                        // +0x41
    unsigned char _pad42[0x44 - 0x42];
    long StunTimer;                                     // +0x44
    bool UseDirectionOverride;                          // +0x48
    C3DVector DirectionOverride;                        // +0x49
    C3DVector StrikePosition;                           // +0x55
    unsigned char _pad61[0x70 - 0x61];
    float Range;                                        // +0x70
    unsigned char _pad74[0x90 - 0x74];
    void* RecoveryAction;                               // +0x90
    unsigned char _pad94[0x98 - 0x94];
    void* RadialBlurKey;                                // +0x98
    bool CanStrike;                                     // +0x9c
    bool Charging;                                      // +0x9d
    unsigned char _pad9e;
    bool IgnoreGround;                                  // +0x9f
    CWorld* GetWorld();                                 // 0x00686A70
    void UpdateDirection(C3DVector* direction);         // 0x007A9870
    C3DVector* GetStrikePosition(C3DVector* out, CThingController* controller);   // 0x007AA9E0
    void Strike(CThing* thing);                         // 0x007AA840
    void FinishRush();                                  // 0x007AA370
    void RushForwards();
};
#pragma pack(pop)

void CTCAssassinRush::RushForwards()
{
    CThing* thing = ParentThing;
    CThingController* controller = thing->Controller;
    C3DVector dir;
    if (UseDirectionOverride && !Charging)
        dir = DirectionOverride;
    else if (Target.GetPItem() && !Target.GetPItem()->IsDead()) {
        CThing* target = Target.GetPItem();
        const C3DVector& from = thing->GetPosition();
        const C3DVector& to = target->GetPosition();
        dir = C3DVector(to.X - from.X, to.Y - from.Y, to.Z - from.Z);
    } else
        dir = controller->GetForward();
    UpdateDirection(&dir);
    SOrientation orientation;
    orientation.Forward = dir;
    orientation.Up = C3DVector(0.0f, 0.0f, 1.0f);
    controller->SetOrientation(orientation);
    float speed = PDef->Speed;
    C3DVector step(dir.X * speed, dir.Y * speed, dir.Z * speed);
    const C3DVector& pos = thing->GetPosition();
    C3DVector fromStart(pos.X - StartPosition.X, pos.Y - StartPosition.Y, pos.Z - StartPosition.Z);
    if (fromStart.X * fromStart.X + fromStart.Y * fromStart.Y + fromStart.Z * fromStart.Z < Range * Range) {
        if (!Moving)
            goto step_forward;
        if (StrikeTarget.GetPItem() && !StrikeTarget.GetPItem()->IsDead() && CanStrike && (StrikeTarget.GetPItem()->Flags6c & 8)) {
            if (StrikeTarget.GetPItem()->Mode == 1)
                StrikeTarget.GetPItem()->StartReacting();
            C3DVector strike;
            StrikePosition = *GetStrikePosition(&strike, controller);
            controller->SetTargetPosition(StrikePosition);
            const C3DVector& tp = StrikeTarget.GetPItem()->GetPosition();
            C3DVector face(tp.X - StrikePosition.X, tp.Y - StrikePosition.Y, tp.Z - StrikePosition.Z);
            C3DVector up(0.0f, 0.0f, 1.0f);
            controller->SetOrientation(*(SOrientation*)&SOrientationFrom(up, face));
            if (thing->Mode == 2) {
                CAssassinRushDef* def = PDef;
                StunTimer = (long)(g_FramesPerSecond * def->StunTime);
                GetWorld()->BulletTime->AddBulletTimeSlowdown(def->SlowdownMultiplier, this, 0.5f);
            }
            Strike(thing);
            return;
        }
        {
            C3DVector probe(controller->Velocity.X + step.X, controller->Velocity.Y + step.Y, controller->Velocity.Z + step.Z);
            probe.Z = thing->GetGroundHeight(probe, false, true);
            thing->GetPosition();
            if (!IgnoreGround) {
                const C3DVector& here = thing->GetPosition();
                float dx = probe.X - here.X;
                float dy = probe.Y - here.Y;
                float limit = PDef->Speed * g_StepCheckScale;
                if (!(dx * dx + dy * dy < limit * limit)) {
                    CWorld* world = GetWorld();
                    C2DCoordI cell;
                    cell.X = (long)probe.X;
                    cell.Y = (long)probe.Y;
                    if (world->GetNavAt(cell)->IsBlocked())
                        goto step_forward;
                }
            }
            if (thing->m_presentBits[0] & 0x10000)
                thing->GetTC<CTCSoundPlayer>(0x10)->PlayAttachedCriteria("HERO_ABILITY_ASSASSIN_RUSH_COLLISION", true, 0, 0, -1);
        }
    }
    thing->Reevaluate();
    if (RecoveryAction)
        thing->AddAction(RecoveryAction);
    Finished = true;
    FinishRush();
    return;
step_forward:
    {
        const C3DVector& here = thing->GetPosition();
        C3DVector next(here.X + step.X, here.Y + step.Y, here.Z + step.Z);
        float ground = thing->GetGroundHeight(next, true, true);
        if (ground > next.Z && ground - next.Z < g_MaxStepDown)
            next.Z = ground;
        if (!CanStrike && ground > next.Z)
            next.Z = ground;
        controller->SetPosition(next, true);
        C3DVector now = thing->GetPosition();
        CWorld* world = GetWorld();
        C3DVector blur(now.X, now.Y, now.Z + PDef->BlurHeight);
        world->Engine->RadialBlur->SetPosition(RadialBlurKey, blur);
        Moving = true;
    }
}
