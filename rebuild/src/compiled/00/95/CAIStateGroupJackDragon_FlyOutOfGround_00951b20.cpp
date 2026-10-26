// CAIStateGroup_JackDragon::FlyOutOfGround() -- kill the invisible bodge creature, stop head turning and wait out the anim loop, then (enemy alive and not stopping) build the fly-out route on a random side (left/right ground-exit positions), remember the side's third point as the previous flight position, take off along it and choose the hover/strafe state; terminate otherwise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00951B20, length 553 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::Kill @ 0x004C9B80, SetHeadTurning @ 0x00950BB0,
// WaitForAnimLoop @ 0x00950950, CAIStateGroupBase::IsStopping @ 0x008FD010, Terminate @ 0x008FCF40, C3DVector::RotateAboutZ
// @ 0x00A14040, CThing::GetPosition @ 0x004C73D0, GetPWorld @ 0x008FCFC0, CWorld::GetPRandomSeed @ 0x0049D810, GFROR13
// @ 0x00497890 (`__fastcall`), vector<C3DVector>::push_back @ 0x0066E630, CCreatureAction_DragonTakeOffIntoFlying ctor
// @ 0x009533E0 / dtor @ 0x00953470, AIAction_PerformActionAndWait @ 0x0097DB70, ChooseHoverOrStrafeState @ 0x0094FFF0,
// free @ 0x00BFEA14.
// Layout: Ego_r.pdb CAIStateGroup_JackDragon (packed): PEnemy +0x14, PInvisibleBodgeCreature +0x44, CurrentState +0x5c,
// GoLeft +0x60, PreviousFlightPos +0x65, DragonPosOutOfGround1/2/3Left/3Right/4Left/4Right +0x72/+0x7e/+0x8a/+0x96/+0xa2/+0xae.
#include <stdlib.h>
#pragma pack(push, 1)
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    void RotateAboutZ(float angle);             // 0x00A14040
};
#pragma pack(pop)
struct CPhysicsBody {
    virtual void v00();
    // slot 63 (+0xfc)
};
struct CPhysicsBodyV {
    virtual void s00(); virtual void s01(); virtual void s02(); virtual void s03(); virtual void s04(); virtual void s05();
    virtual void s06(); virtual void s07(); virtual void s08(); virtual void s09(); virtual void s10(); virtual void s11();
    virtual void s12(); virtual void s13(); virtual void s14(); virtual void s15(); virtual void s16(); virtual void s17();
    virtual void s18(); virtual void s19(); virtual void s20(); virtual void s21(); virtual void s22(); virtual void s23();
    virtual void s24(); virtual void s25(); virtual void s26(); virtual void s27(); virtual void s28(); virtual void s29();
    virtual void s30(); virtual void s31(); virtual void s32(); virtual void s33(); virtual void s34(); virtual void s35();
    virtual void s36(); virtual void s37(); virtual void s38(); virtual void s39(); virtual void s40(); virtual void s41();
    virtual void s42(); virtual void s43(); virtual void s44(); virtual void s45(); virtual void s46(); virtual void s47();
    virtual void s48(); virtual void s49(); virtual void s50(); virtual void s51(); virtual void s52(); virtual void s53();
    virtual void s54(); virtual void s55(); virtual void s56(); virtual void s57(); virtual void s58(); virtual void s59();
    virtual void s60(); virtual void s61(); virtual void s62();
    virtual float GetHeading();                 // slot 63 (+0xfc)
};
class CThing { public:
    unsigned char _pad0[0x60];
    CPhysicsBodyV* PhysicsBody;                 // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
    void Kill(bool b);                          // 0x004C9B80
};
class CThingCreatureBase : public CThing { public: };
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
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
long __fastcall GFROR13(unsigned long);      // 0x00497890
class CWorld { public:
    unsigned long* GetPRandomSeed();            // 0x0049D810
};
inline long WorldRandomRange(unsigned long* seed, unsigned long range)
{
    *seed = *seed * 0x24A1 + 0x24DF;
    *seed = GFROR13(*seed);
    return *seed % range;
}
struct CVector3DList {                          // STLport vector<C3DVector>, retail 12 bytes
    C3DVector* _M_start;
    C3DVector* _M_finish;
    C3DVector* _M_end_of_storage;
    CVector3DList() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CVector3DList() { if (_M_start) free(_M_start); }
    void push_back(const C3DVector& v);         // 0x0066E630
};
class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _body[0x13c];
};
class CCreatureAction_DragonTakeOffIntoFlying : public CCreatureActionBase { public:
    unsigned char _takeoff[0x10];
    CCreatureAction_DragonTakeOffIntoFlying(CThingCreatureBase* creature, const CVector3DList& route);   // 0x009533E0
    ~CCreatureAction_DragonTakeOffIntoFlying();  // 0x00953470
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->PCreature; }
    CWorld* GetPWorld();                        // 0x008FCFC0
    long AIAction_PerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DB70
    void Terminate();                           // 0x008FCF40
    bool IsStopping();                          // 0x008FD010
};
#pragma pack(push, 1)
struct CAIStateGroup_JackDragon : CAIStateGroupBase {
    CIntelligentPointer<CThing> PEnemy;             // +0x14
    unsigned char _pad1c[0x44 - 0x1c];
    CIntelligentPointer<CThing> PInvisibleBodgeCreature;   // +0x44
    unsigned char _pad4c[0x5c - 0x4c];
    long CurrentState;                          // +0x5c
    long GoLeft;                                // +0x60
    bool BattleTriggered;                       // +0x64
    C3DVector PreviousFlightPos;                // +0x65
    bool IsRangeAttack;                         // +0x71
    C3DVector DragonPosOutOfGround1;            // +0x72
    C3DVector DragonPosOutOfGround2;            // +0x7e
    C3DVector DragonPosOutOfGround3Left;        // +0x8a
    C3DVector DragonPosOutOfGround3Right;       // +0x96
    C3DVector DragonPosOutOfGround4Left;        // +0xa2
    C3DVector DragonPosOutOfGround4Right;       // +0xae
    void SetHeadTurning(bool on);               // 0x00950BB0
    void WaitForAnimLoop(bool b);               // 0x00950950
    long ChooseHoverOrStrafeState();            // 0x0094FFF0
    void FlyOutOfGround();
};
#pragma pack(pop)

void CAIStateGroup_JackDragon::FlyOutOfGround()
{
    if (IS_THING_ALIVE(PInvisibleBodgeCreature))
        PInvisibleBodgeCreature.GetPItem()->Kill(true);
    SetHeadTurning(false);
    WaitForAnimLoop(false);
    if (!IS_THING_ALIVE(PEnemy)) {
        Terminate();
        return;
    }
    if (IsStopping()) {
        Terminate();
        return;
    }
    C3DVector up(0.0f, 50.0f, 0.0f);
    up.RotateAboutZ(GetCreature()->PhysicsBody->GetHeading());
    GetCreature()->GetPosition();
    CVector3DList route;
    if (WorldRandomRange(GetPWorld()->GetPRandomSeed(), 100) > 50)
        GoLeft = 1;
    else
        GoLeft = 0;
    if (GoLeft) {
        route.push_back(DragonPosOutOfGround1);
        route.push_back(DragonPosOutOfGround2);
        route.push_back(DragonPosOutOfGround3Left);
        route.push_back(DragonPosOutOfGround4Left);
        PreviousFlightPos = DragonPosOutOfGround3Left;
    } else {
        route.push_back(DragonPosOutOfGround1);
        route.push_back(DragonPosOutOfGround2);
        route.push_back(DragonPosOutOfGround3Right);
        route.push_back(DragonPosOutOfGround4Right);
        PreviousFlightPos = DragonPosOutOfGround3Right;
    }
    AIAction_PerformActionAndWait(&CCreatureAction_DragonTakeOffIntoFlying(GetCreature(), route), false);
    bool stop = !IS_THING_ALIVE(PEnemy) || IsStopping();
    if (stop)
        Terminate();
    else
        CurrentState = ChooseHoverOrStrafeState();
}