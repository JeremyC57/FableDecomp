// NTCScriptedControl::CActionFollowCreature::Update() -- keep the follow target valid, ignore it for navigation, mirror its sneaking, then pick a speed from the gap to the target (stop, slow, match or run) and steer along the nav route, looking where it goes
// Leaf: ?Update@CActionFollowCreature@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00713EE0, length 1153 bytes.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (interface 7 CTCCreatureNavigation) and its const twin @ 0x004365B0
// (interface 0x36 CTCStealth, through the const lookup), CTCCreatureNavigation::SetThingToIgnore @ 0x006CBE30 /
// GetNextNavPosOnRoute @ 0x006CBAC0 / SetNavigationTarget @ 0x006CB960, SetSneaking @ 0x00713D50, the 2D distance @ 0x009E1AD0
// (__fastcall), the thing's current speed @ 0x00661CD0, sneak speed @ 0x00664CB0 / virtual max speed (slot 81, +0x144),
// CThing::GetPosition @ 0x004C73D0, IsPerformingAnyAction @ 0x006622A0, CTCLook::LookAtPos @ 0x00753E90 / IsPosInViewRange
// @ 0x007534B0; the script thing's slots 11/71/76/77 (+0x2c/+0x11c/+0x130/+0x134) and the physics slots 33/70 (+0x84/+0x118).
// Data: 0.7f @ 0x01255318, 8.0f @ 0x0125916C, 0.5f @ 0x0122F59C, 0.8f @ 0x012654EC, 1.5f @ 0x0125AADC, epsilon 0.0001f @ 0x0129BA3C.
// Layout: per the landed CActionFollowCreature::OnActivate 0x00715710 (+0x09 is the finished flag, +0x1C Sneaking, +0x20
// FollowRange, +0x25 AvoidDynamicObstacles, +0x27 Running); CNavigationInput / CCreatureNavOutput per the landed 0x0097EC00.
// Shape: the max-speed arms go through an inline GetCreature() accessor (one hoisted load, per-arm pushes); the 2D positions
// sit in their own block; the look-ahead point is `delta + pos` through a helper taking (pos, velocity) so the velocity
// virtual is called first; the clamp keeps `const float&` from Min; the epsilon is an extern float squared at run time; the
// squared length is written Y*Y + Z*Z + X*X (emitted X, Z, Y); the facing point is a named local over a named physics pointer.
template<class T> inline const T& Max(const T& a, const T& b) { return a < b ? b : a; }
template<class T> inline const T& Min(const T& a, const T& b) { return b < a ? b : a; }
struct CTCBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    __forceinline C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
struct C2DVector { float X, Y; };
__forceinline C3DVector Offset(const C3DVector& pos, const C3DVector& delta) { return C3DVector(delta.X + pos.X, delta.Y + pos.Y, delta.Z + pos.Z); }
float __fastcall Get2DDistance(const C2DVector& a, const C2DVector& b);   // 0x009E1AD0
struct CTCPhysicsBase {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032();
    virtual void SetFacingPos(const C3DVector& pos);   // slot 33 (+0x84)
    virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual C3DVector GetVelocity() const;      // slot 70 (+0x118)
    unsigned char _pad04[0xc - 0x4];
    C3DVector Position;                         // +0x0C
    unsigned char _pad18[0x34 - 0x18];
    float Radius;                               // +0x34
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
struct CThing {
    virtual void t000(); virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005(); virtual void t006(); virtual void t007();
    virtual void t008(); virtual void t009(); virtual void t010(); virtual void t011(); virtual void t012(); virtual void t013(); virtual void t014(); virtual void t015();
    virtual void t016(); virtual void t017(); virtual void t018(); virtual void t019(); virtual void t020(); virtual void t021(); virtual void t022(); virtual void t023();
    virtual void t024(); virtual void t025(); virtual void t026(); virtual void t027(); virtual void t028(); virtual void t029(); virtual void t030(); virtual void t031();
    virtual void t032(); virtual void t033(); virtual void t034(); virtual void t035(); virtual void t036(); virtual void t037(); virtual void t038(); virtual void t039();
    virtual void t040(); virtual void t041(); virtual void t042(); virtual void t043(); virtual void t044(); virtual void t045(); virtual void t046(); virtual void t047();
    virtual void t048(); virtual void t049(); virtual void t050(); virtual void t051(); virtual void t052(); virtual void t053(); virtual void t054(); virtual void t055();
    virtual void t056(); virtual void t057(); virtual void t058(); virtual void t059(); virtual void t060(); virtual void t061(); virtual void t062(); virtual void t063();
    virtual void t064(); virtual void t065(); virtual void t066(); virtual void t067(); virtual void t068(); virtual void t069(); virtual void t070(); virtual void t071();
    virtual void t072(); virtual void t073(); virtual void t074(); virtual void t075(); virtual void t076(); virtual void t077(); virtual void t078(); virtual void t079();
    virtual void t080();
    virtual float GetMaxSpeedTowards(const C3DVector& pos);   // slot 81 (+0x144)
    unsigned char _pad04[0x20 - 0x04];
    unsigned long UsingComponents[9];           // +0x20
    CInterfaceMap Components;                   // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PPhysics;                   // +0x60
    const C3DVector& GetPosition() const;       // 0x004C73D0
    float GetCurrentSpeed() const;              // 0x00661CD0
    float GetSneakSpeedTowards(const C3DVector& pos);   // 0x00664CB0
    bool IsPerformingAnyAction();               // 0x006622A0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterfaceC(T** ppOut, long interfaceType) const
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &Components;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CTCStealth { char _pad[0xf]; bool Sneaking; };   // interface 0x36
struct CNavigationInput {
    C3DVector Destination;                      // +0x00
    float Proximity;                            // +0x0c
    long  MovementType;                         // +0x10
    float Speed;                                // +0x14
};
struct CCreatureNavOutput {
    long Status;                                // 0 arrived, 1 failed, 2 moving
    C3DVector Offset;
};
struct CTCCreatureNavigation {                  // interface 7
    unsigned char _pad[0x56];
    bool Active;                                // +0x56
    void SetThingToIgnore(const CThing* thing); // 0x006CBE30
    CCreatureNavOutput GetNextNavPosOnRoute(const CNavigationInput& input, bool b1, bool b2, bool b3, bool b4, bool b5);   // 0x006CBAC0
    void SetNavigationTarget(const CNavigationInput& input);  // 0x006CB960 (PDB ?SetNavigationTarget@CTCCreatureNavigation@@QAEXABVCNavigationInput@@@Z)
};
struct CTCLook {                                // interface 0x43
    void LookAtPos(const C3DVector* position, long priority);   // 0x00753E90
    bool IsPosInViewRange(const C3DVector& pos, float angle, bool a, bool b);   // 0x007534B0
};
extern float g_navEpsilon;                      // 0x0129BA3C (0.0001f)
struct CScriptThing {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing* GetThing() const;   // slot 11 (+0x2c)
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070();
    virtual void Refresh();             // slot 71 (+0x11c)
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075();
    virtual bool IsDead() const;        // slot 76 (+0x130)
    virtual bool IsValid() const;       // slot 77 (+0x134)
    long _a, _b;
};
struct CCreaturePointer { void* __vftable; CThing* PThing; };
class CActionFollowCreature {
public:
    void* __vftable;                                 // +0x00
    CCreaturePointer* PControl;                      // +0x04
    bool Activated;                                  // +0x08
    bool Finished;                                   // +0x09
    CScriptThing PTargetThing;                       // +0x0C
    void* PTCOldPhysics;                             // +0x18
    bool Sneaking;                                   // +0x1C
    float FollowRange;                               // +0x20
    bool AddedStealthTC;                             // +0x24
    bool AvoidDynamicObstacles;                      // +0x25
    bool PreviousPushableByHero;                     // +0x26
    bool Running;                                    // +0x27
    void SetSneaking(bool sneaking);                 // 0x00713D50
    CThing* GetCreature() const { return PControl->PThing; }
    void Update();
};

void CActionFollowCreature::Update()
{
    if (!PTargetThing.IsValid())
        PTargetThing.Refresh();
    CThing* target = PTargetThing.GetThing();
    CTCCreatureNavigation* navigation;
    if (PControl->PThing->GetInterface(&navigation, 7))
        navigation->SetThingToIgnore(target);
    if (PTargetThing.IsDead()) {
        Finished = true;
        return;
    }
    CTCStealth* stealth;
    SetSneaking(PTargetThing.GetThing()->GetInterfaceC(&stealth, 0x36) && stealth->Sneaking);
    CTCCreatureNavigation* nav;
    PControl->PThing->GetInterface(&nav, 7);
    PTargetThing.GetThing();
    float followRange = FollowRange;
    CTCPhysicsBase* myPhysics = PControl->PThing->PPhysics;
    float radii = PTargetThing.GetThing()->PPhysics->Radius + myPhysics->Radius + 0.7f;
    float distance;
    {
        C2DVector targetPos;
        const C3DVector& tp = PTargetThing.GetThing()->PPhysics->Position;
        targetPos.X = tp.X;
        targetPos.Y = tp.Y;
        C2DVector myPos;
        const C3DVector& mp = PControl->PThing->PPhysics->Position;
        myPos.X = mp.X;
        myPos.Y = mp.Y;
        distance = Get2DDistance(myPos, targetPos);
    }
    float targetSpeed = PTargetThing.GetThing()->GetCurrentSpeed();
    if (targetSpeed == 0.0f) {
        CThing* t = PTargetThing.GetThing();
        targetSpeed = t->GetMaxSpeedTowards(PControl->PThing->GetPosition());
    }
    CThing* me = PControl->PThing;
    C3DVector ahead = Offset(me->GetPosition(), me->PPhysics->GetVelocity());
    float maxSpeed;
    if (Sneaking)
        maxSpeed = GetCreature()->GetSneakSpeedTowards(ahead);
    else
        maxSpeed = GetCreature()->GetMaxSpeedTowards(ahead);
    float gap = distance - radii;
    float half = (followRange + 8.0f) * 0.5f;
    float speed;
    if (gap > 8.0f || (Running && gap > half)) {
        float run = targetSpeed * 1.5f;
        speed = Max(run, maxSpeed);
        Running = true;
    } else {
        Running = false;
        if (gap <= followRange)
            speed = 0.0f;
        else if (gap < half)
            speed = targetSpeed * 0.8f;
        else
            speed = targetSpeed;
    }
    const float& finalSpeed = Min(speed, maxSpeed);
    CNavigationInput input;
    input.Destination = PTargetThing.GetThing()->GetPosition();
    input.Proximity = radii;
    input.MovementType = 0;
    input.Speed = finalSpeed;
    CCreatureNavOutput output = nav->GetNextNavPosOnRoute(input, false, AvoidDynamicObstacles, true, false, false);
    nav->SetNavigationTarget(input);
    nav->Active = true;
    if (!PControl->PThing->IsPerformingAnyAction() && output.Status == 2
        && output.Offset.Y * output.Offset.Y + output.Offset.Z * output.Offset.Z + output.Offset.X * output.Offset.X > g_navEpsilon * g_navEpsilon) {
        C3DVector lookPos = output.Offset + PControl->PThing->PPhysics->Position;
        CTCLook* look;
        if (PControl->PThing->GetInterface(&look, 0x43)) {
            look->LookAtPos(&lookPos, 0x21);
            if (look->IsPosInViewRange(lookPos, 0.41666666f, false, false))
            {
                CTCPhysicsBase* phys = PControl->PThing->PPhysics;
                C3DVector facing = output.Offset + phys->Position;
                phys->SetFacingPos(facing);
            }
        }
    }
}