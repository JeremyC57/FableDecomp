// CTCLook::CTCLook(CThing* thing) -- CTCBase(thing) + the graphic-appearance component base, look state cleared (state 1, flags 0x17, eye angles marked changed), register with the owner's graphic appearance and remember the owner as the parent creature when it is one
// Leaf: ??0CTCLook
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00753170, length 284 bytes (+ int3 padding), `ret 4`. Vtables: CTCLook primary
// 0x01269744, the NGraphicAppearance::CAGraphicAppearanceComponent base 0x0125A170 (stored first by its inline ctor) and
// CTCLook's side 0x012697B8 at +0xC (rtti-vtables.tsv).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800 (catalog "CEngineSubPrimitive2DClockSprite" is propagated),
// NGraphicAppearance::CTCGraphicAppearance::RegisterComponent @ 0x004BD390 (owner +0x64, the +0xC subobject; catalog
// "DoubleLinkedListInsertNode" is propagated). Global: a once-only flag byte 0x013BAD59 set after the first construction.
// Shape: the landed CTCLight ctor 0x00640F00 model: an init-list in declaration order (VC7.1 shares ebx for the zeroes and
// ecx = 1 for HeadTurnState / BodyTurnState / CanTurnBody / EyeAnglesChanged / FirstTimeThrough, sinking the three
// first ones below the LookFlags store), the STLport vector as _Vector_base + _STLP_alloc_proxy, the two counted
// pointers' own zeroing ctors; PosToLookAt is built from three zeroes, DirectionToLookIn, TurningViaAnimMinAngle and
// TimeToStopLooking are left unset. The parent-creature test reads ThingTypeFlags (+0x6c, bit 3) of the re-read owner.
// Layout: Ego_r.pdb CTCLook (0xd4) with the 12-byte retail STLport vector (TargetAcquired onward 4 below the PDB):
// PParentCreature +0x10, PDef +0x14, PAppearanceDef +0x18, ..., LookFlags +0x64, PosToLookAt +0x68, EyeAnglesChanged +0x80,
// eye primitives +0x84/+0x88, TurningAnims +0x8c, PCurrentTurningAnim +0x9c, TargetAcquired +0xa0, CachedHeadPos +0xa1
// (unaligned in the PDB as well: the 3 floats are modelled with pack(1)), HeadCacheTimer +0xb0, OneFrameDelay +0xb8,
// PHeadTurnAnimH/V +0xbc/+0xc4, FirstTimeThrough +0xcc. No engine header for CTCLook.
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
    void RegisterComponent(CAGraphicAppearanceComponent* component);   // 0x004BD390
};
struct CAGraphicAppearanceComponent {               // vtable 0x0125A170
    virtual void OnAppearanceDraw();
    CAGraphicAppearanceComponent() { }
};
}
struct CThing {
    unsigned char _pad00[0x64];
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;   // +0x64
    unsigned char _pad68[0x6c - 0x68];
    unsigned long ThingTypeFlags;                   // +0x6c (PDB +0x7c)
};
extern bool g_bLookConstructedOnce;                 // 0x013BAD59

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
    virtual ~CTCBase();
    CThing* ParentThing;                            // +0x04
    bool ToBeRemoved;                               // +0x08
    bool IsInGlobalUpdateTCs;                       // +0x09
    CTCBase(CThing* thing);                         // 0x00686800
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
    virtual ~CTCLook();
    virtual void OnAppearanceDraw();
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