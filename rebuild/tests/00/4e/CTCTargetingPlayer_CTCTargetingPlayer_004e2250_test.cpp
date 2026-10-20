// Behaviour test for CTCTargetingPlayer(CThing*) (0x004E2250): the base gets the thing, the target-next flags and frame are
// cleared (-1), the targeted point is constructed, both point lists are empty, no Z target, zeroed positions, PLastThreatened
// is built from null and the other three pointers default-constructed, not locked onto an enemy.
#include <cstdio>
static int g_defaults = 0, g_fromPointer = 0, g_points = 0;
class CThing;
class CBaseObject;
struct CTCTargetingBase {
    virtual void v00() { }
    unsigned char _base[0x34];                          // +0x04 CTCBase rest, ScriptOverride.., PTargetedThing, PTargetingDef, LookAtCamera
    CThing* owner;
    CTCTargetingBase(CThing* thing) : owner(thing) { }
};
struct CBaseIntelligentPointer {
    virtual void v00() { }
    CThing* m_pItem;                                    // +0x04
    CBaseIntelligentPointer() : m_pItem((CThing*)0x1) { ++g_defaults; }
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00() { }
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { m_pItem = item; ++g_fromPointer; }
};
namespace NPlayerTargeting {
struct CTargetPoint {
    unsigned char _data[0x1c];
    CTargetPoint() { ++g_points; }
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
    virtual void v00() { }
    CTCTargetingPlayer(CThing* thing);
};

CTCTargetingPlayer::CTCTargetingPlayer(CThing* thing)
    : CTCTargetingBase(thing), TargetNextThing(false), ForcedToHaveNoTarget(false), FrameSetAsTargetNextThing(-1),
      CurrentZTargetIndex(-1), TargetedPos(0.0f, 0.0f, 0.0f), OldTargetedPos(0.0f, 0.0f, 0.0f), PLastThreatenedThing(0),
      LockedOntoEnemy(false)
{
}

int main()
{
    int bad = 0;
    CThing* thing = (CThing*)0x99;
    CTCTargetingPlayer* t = new CTCTargetingPlayer(thing);
    if (t->owner != thing || t->TargetNextThing || t->ForcedToHaveNoTarget || t->FrameSetAsTargetNextThing != -1) bad |= 1;
    if (g_points != 1 || t->Targets._M_start || t->Targets._M_finish || t->Targets._M_end_of_storage.m_data) bad |= 2;
    if (t->RejectedTargetPoints._M_start || t->RejectedTargetPoints._M_end_of_storage.m_data || t->CurrentZTargetIndex != -1) bad |= 4;
    if (t->TargetedPos.X != 0.0f || t->TargetedPos.Y != 0.0f || t->TargetedPos.Z != 0.0f || t->OldTargetedPos.Z != 0.0f) bad |= 8;
    if (g_fromPointer != 1 || g_defaults != 4 || t->PLastThreatenedThing.m_pItem != 0 || t->PTargetOnlyThing.m_pItem != (CThing*)0x1) bad |= 16;
    if (t->LockedOntoEnemy) bad |= 32;
    if (bad) { std::printf("004e2250_TEST FAIL\n"); return 1; }
    std::printf("004e2250_TEST PASS\n");
    return 0;
}