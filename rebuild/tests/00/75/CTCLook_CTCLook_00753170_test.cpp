// Behaviour test for CTCLook::CTCLook(CThing*) (0x00753170).
#include <cstdio>
struct CThing;
struct CLookDef;
struct CAppearanceDef;
struct CPrimitive;
struct CAnimationEntry;
struct CCPPointerInfo;
struct CAnimEntryLite;
struct CThingCreatureBase;
namespace NGraphicAppearance {
struct CAGraphicAppearanceComponent;
struct CTCGraphicAppearance {
    CAGraphicAppearanceComponent* Registered; int Count;
    void RegisterComponent(CAGraphicAppearanceComponent* component) { Registered = component; ++Count; }
};
struct CAGraphicAppearanceComponent {               // vtable 0x0125A170
    virtual void OnAppearanceDraw() { }
    CAGraphicAppearanceComponent() { }
};
}
struct CThing {
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;   // +0x64
    unsigned long ThingTypeFlags;                   // +0x6c (PDB +0x7c)
};
static bool g_bLookConstructedOnce = false;

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
#pragma pack(push, 1)
struct CPackedVector { float X, Y, Z; CPackedVector(float x, float y, float z) : X(x), Y(y), Z(z) { } };
#pragma pack(pop)
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};
template<class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
};
template <class T> struct CAllocator {              // STLport allocator<T>: empty
    CAllocator() { }
};
template <class T> struct CAllocProxy : CAllocator<T> {    // STLport _STLP_alloc_proxy<T*, T, allocator<T> >
    T* m_data;
    CAllocProxy(const CAllocator<T>& alloc, T* p) : CAllocator<T>(alloc), m_data(p) { }
};
template <class T> struct CVectorBase {             // STLport _Vector_base<T, allocator<T> >
    T*             _M_start;
    T*             _M_finish;
    CAllocProxy<T> _M_end_of_storage;
    CVectorBase(const CAllocator<T>& alloc) : _M_start(0), _M_finish(0), _M_end_of_storage(alloc, 0) { }
};
struct STurningAnim { float Angle; const CAnimationEntry* Entry; };   // std::pair<float, const CAnimationEntry*>
template<class T> struct CVector : CVectorBase<T> {   // STLport vector<T> (12 bytes)
    CVector() : CVectorBase<T>(CAllocator<T>()) { }
};
struct CTCBase {
    virtual ~CTCBase() { }
    CThing* ParentThing;                            // +0x04
    bool ToBeRemoved;                               // +0x08
    bool IsInGlobalUpdateTCs;                       // +0x09
    CTCBase(CThing* thing) : ParentThing(thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { }
};
struct CTCLook : CTCBase, NGraphicAppearance::CAGraphicAppearanceComponent {   // vtables 0x01269744 / 0x012697B8
    CThingCreatureBase*          PParentCreature;               // +0x10
    CDefPointer<const CLookDef>  PDef;                          // +0x14
    CDefPointer<const CAppearanceDef> PAppearanceDef;           // +0x18
    float RecoveryAngleToLookAtXY;                              // +0x1c
    float RecoveryAngleToLookAtYZ;                              // +0x20
    float BodyAngleXY;                                          // +0x24
    float BodyAngleYZ;                                          // +0x28
    float HeadAngleRelToBodyXY;                                 // +0x2c
    float HeadAngleRelToBodyYZ;                                 // +0x30
    float EyeAnglesRelToHeadXY;                                 // +0x34
    float EyeAnglesRelToHeadYZ;                                 // +0x38
    long  LookTimer;                                            // +0x3c
    bool  UsingOldStyleTurning;                                 // +0x40
    bool  TurningViaAnimationDisable;                           // +0x41
    float FractionOfTurningToAcceptEachFrame;                   // +0x44
    long  HeadTurnState;                                        // +0x48
    long  BodyTurnState;                                        // +0x4c
    bool  CanTurnBody;                                          // +0x50
    float BodyVelocityXY;                                       // +0x54
    float BodyVelocityYZ;                                       // +0x58
    float HeadVelocityXY;                                       // +0x5c
    float HeadVelocityYZ;                                       // +0x60
    long  LookFlags;                                            // +0x64
    C3DVector PosToLookAt;                                      // +0x68
    C3DVector DirectionToLookIn;                                // +0x74
    bool  EyeAnglesChanged;                                     // +0x80
    CPrimitive* LeftEyePrimitive;                               // +0x84
    CPrimitive* RightEyePrimitive;                              // +0x88
    CVector<STurningAnim> TurningAnims;                         // +0x8c
    float TurningViaAnimMinAngle;                               // +0x98
    CAnimationEntry* PCurrentTurningAnim;                       // +0x9c
    bool  TargetAcquired;                                       // +0xa0
    CPackedVector CachedHeadPos;                                // +0xa1
    long  HeadCacheTimer;                                       // +0xb0
    long  TimeToStopLooking;                                    // +0xb4
    bool  OneFrameDelay;                                        // +0xb8
    CCountedPointer<CAnimEntryLite> PHeadTurnAnimH;             // +0xbc
    CCountedPointer<CAnimEntryLite> PHeadTurnAnimV;             // +0xc4
    bool  FirstTimeThrough;                                     // +0xcc
    CTCLook(CThing* thing);
    virtual ~CTCLook() { }
    virtual void OnAppearanceDraw() { }
};

CTCLook::CTCLook(CThing* thing)
    : CTCBase(thing)
    , PParentCreature(0)
    , RecoveryAngleToLookAtXY(0.0f)
    , RecoveryAngleToLookAtYZ(0.0f)
    , BodyAngleXY(0.0f)
    , BodyAngleYZ(0.0f)
    , HeadAngleRelToBodyXY(0.0f)
    , HeadAngleRelToBodyYZ(0.0f)
    , EyeAnglesRelToHeadXY(0.0f)
    , EyeAnglesRelToHeadYZ(0.0f)
    , LookTimer(0)
    , UsingOldStyleTurning(false)
    , TurningViaAnimationDisable(false)
    , FractionOfTurningToAcceptEachFrame(1.0f)
    , HeadTurnState(1)
    , BodyTurnState(1)
    , CanTurnBody(true)
    , BodyVelocityXY(0.0f)
    , BodyVelocityYZ(0.0f)
    , HeadVelocityXY(0.0f)
    , HeadVelocityYZ(0.0f)
    , LookFlags(0x17)
    , PosToLookAt(0.0f, 0.0f, 0.0f)
    , EyeAnglesChanged(true)
    , LeftEyePrimitive(0)
    , RightEyePrimitive(0)
    , PCurrentTurningAnim(0)
    , TargetAcquired(false)
    , CachedHeadPos(0.0f, 0.0f, 0.0f)
    , HeadCacheTimer(0)
    , OneFrameDelay(false)
    , FirstTimeThrough(true)
{
    ParentThing->PTCGraphicAppearance->RegisterComponent(this);
    if (ParentThing->ThingTypeFlags & 8)
        PParentCreature = (CThingCreatureBase*)thing;
    if (!g_bLookConstructedOnce)
        g_bLookConstructedOnce = true;
}

int main()
{
    NGraphicAppearance::CTCGraphicAppearance app; app.Registered = 0; app.Count = 0;
    CThing creature; creature.PTCGraphicAppearance = &app; creature.ThingTypeFlags = 8;
    CTCLook* look = new CTCLook(&creature);
    bool ok = look->ParentThing == &creature && look->PParentCreature == (CThingCreatureBase*)&creature
        && app.Count == 1 && app.Registered == static_cast<NGraphicAppearance::CAGraphicAppearanceComponent*>(look)
        && look->HeadTurnState == 1 && look->BodyTurnState == 1 && look->CanTurnBody && look->LookFlags == 0x17
        && look->FractionOfTurningToAcceptEachFrame == 1.0f && look->EyeAnglesChanged && look->FirstTimeThrough
        && !look->TargetAcquired && look->HeadVelocityXY == 0.0f && look->PDef.Object == 0
        && look->TurningAnims._M_start == 0 && look->PHeadTurnAnimV.Info == 0 && look->CachedHeadPos.Y == 0.0f
        && g_bLookConstructedOnce;
    delete look;
    CThing prop; prop.PTCGraphicAppearance = &app; prop.ThingTypeFlags = 0;
    CTCLook* look2 = new CTCLook(&prop);
    ok = ok && look2->PParentCreature == 0 && app.Count == 2;
    delete look2;
    if (!ok) { std::printf("00753170_TEST FAIL\n"); return 1; }
    std::printf("00753170_TEST PASS\n");
    return 0;
}