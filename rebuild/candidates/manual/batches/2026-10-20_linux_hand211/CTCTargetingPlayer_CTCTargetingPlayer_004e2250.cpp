// CTCTargetingPlayer::CTCTargetingPlayer(CThing* thing) -- the targeting base, then a cleared player-targeting state: no target-next request, an empty targeted point and point lists, no Z target, zeroed targeted positions and four empty thing pointers
// Leaf: ??0CTCTargetingPlayer
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E2250, 184 bytes (`ret 4`; size TU: `or [m],-1`, `fldz ; fstp` zeros);
// vtable 0x0124280C CTCTargetingPlayer.
// Callees: CTCTargetingBase::CTCTargetingBase(CThing*) @ 0x007F58E0, NPlayerTargeting::CTargetPoint::CTargetPoint() @ 0x004D7A76,
// CIntelligentPointer<CThing>::CIntelligentPointer(CThing*) @ 0x004A96F0 (out of line, with null), CBaseIntelligentPointer
// ctor @ 0x00A01B10 (the three default-constructed pointers; vtable 0x01238C6C stored inline).
// Layout: engine/CTCTargetingPlayer.h carries the PDB offsets (16-byte vectors); retail has 12-byte vectors, so from
// CurrentZTargetIndex on every member sits 8 below the header (local model): FrameSetAsTargetNextThing +0x44, TargetedPoint +0x4c,
// Targets +0x68, RejectedTargetPoints +0x74, CurrentZTargetIndex +0x80, TargetedPos +0x84, OldTargetedPos +0x90,
// PLastThreatenedThing +0xd4, PProjectileTargetedThing +0xdc, PTargetLockedThing +0xe4, PTargetOnlyThing +0xec,
// LockedOntoEnemy +0xf4.
// Shape: everything through the initialiser list in declaration order (the -1 store is scheduled above the vtable store);
// both vectors are the STLport _Vector_base + _STLP_alloc_proxy; the two positions are C3DVector(0, 0, 0) built through
// their address (x87 zero stores); one zero register serves the remaining zeros.
class CThing;
class CBaseObject;
struct CTCTargetingBase {
    virtual void v00();
    unsigned char _base[0x34];                          // +0x04 CTCBase rest, ScriptOverride.., PTargetedThing, PTargetingDef, LookAtCamera
    CTCTargetingBase(CThing* thing);                    // 0x007F58E0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                    // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    CIntelligentPointer(T* item);                       // 0x004A96F0 (CThing)
};
namespace NPlayerTargeting {
struct CTargetPoint {
    unsigned char _data[0x1c];
    CTargetPoint();                                     // 0x004D7A76
};
}
template <class T> struct CAllocator {                  // STLport allocator<T> (empty)
    CAllocator() { }
};
template <class T> struct CAllocProxy : CAllocator<T> { // STLport _STLP_alloc_proxy<T*, T, allocator<T> >
    T* m_data;
    CAllocProxy(const CAllocator<T>& alloc, T* p) : CAllocator<T>(alloc), m_data(p) { }
};
template <class T> struct CVectorBase {                 // STLport _Vector_base<T, allocator<T> >
    T* _M_start;
    T* _M_finish;
    CAllocProxy<T> _M_end_of_storage;
    CVectorBase(const CAllocator<T>& alloc) : _M_start(0), _M_finish(0), _M_end_of_storage(alloc, 0) { }
};
template <class T> struct CRetailVector : CVectorBase<T> {   // STLport vector<T> (12 bytes)
    CRetailVector() : CVectorBase<T>(CAllocator<T>()) { }
};
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct C2DVector {
    float X, Y;
    C2DVector() { }
};
struct CTCTargetingPlayer : CTCTargetingBase {
    long ZLockTargetSelectionTimer;                     // +0x38
    bool TargetNextThing;                               // +0x3c
    bool ForcedToHaveNoTarget;                          // +0x3d
    long TargetNextThingMethod;                         // +0x40 ETargetNextThingMethod
    long FrameSetAsTargetNextThing;                     // +0x44
    float MaxTargetingDistance;                         // +0x48
    NPlayerTargeting::CTargetPoint TargetedPoint;       // +0x4c
    CRetailVector<NPlayerTargeting::CTargetPoint> Targets;              // +0x68
    CRetailVector<NPlayerTargeting::CTargetPoint> RejectedTargetPoints; // +0x74
    long CurrentZTargetIndex;                           // +0x80
    C3DVector TargetedPos;                              // +0x84
    C3DVector OldTargetedPos;                           // +0x90
    C3DVector TargetedPosOffset3D;                      // +0x9c
    C3DVector OldTargetedPosOffset3D;                   // +0xa8
    C2DVector TargetedPosOffset;                        // +0xb4
    C2DVector DeltaTargetedPosOffset;                   // +0xbc
    C2DVector DesiredTargetedPosOffset;                 // +0xc4
    C2DVector TargetedPosOffsetSecondOrder;             // +0xcc
    CIntelligentPointer<CThing> PLastThreatenedThing;   // +0xd4
    CIntelligentPointer<CThing> PProjectileTargetedThing;   // +0xdc
    CIntelligentPointer<CThing> PTargetLockedThing;     // +0xe4
    CIntelligentPointer<CThing> PTargetOnlyThing;       // +0xec
    bool LockedOntoEnemy;                               // +0xf4
    virtual void v00();
    CTCTargetingPlayer(CThing* thing);
};

CTCTargetingPlayer::CTCTargetingPlayer(CThing* thing)
    : CTCTargetingBase(thing), TargetNextThing(false), ForcedToHaveNoTarget(false), FrameSetAsTargetNextThing(-1),
      CurrentZTargetIndex(-1), TargetedPos(0.0f, 0.0f, 0.0f), OldTargetedPos(0.0f, 0.0f, 0.0f), PLastThreatenedThing(0),
      LockedOntoEnemy(false)
{
}
