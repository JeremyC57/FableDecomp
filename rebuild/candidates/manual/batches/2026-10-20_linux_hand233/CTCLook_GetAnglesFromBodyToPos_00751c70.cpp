// CTCLook::GetAnglesFromBodyToPos(const C3DVector& pos, bool unused, float& angleXY, float& angleYZ) const -- the XY / YZ angle differences from the body facing to the direction head->pos, or to the recovery angles when there is no live parent creature or pos is the head position
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00751C70, length 297 bytes (+ int3 padding), `ret 10h`; the second argument
// (a bool, FableWin.pdb ?GetAnglesFromBodyToPos@CTCLook@@ABEXABVC3DVector@@_NAAM2@Z) is never read (its slot homes the
// XY angle across the second call).
// Callees: the physics TC's GetFacingAngleXY / GetFacingAngleYZ (virtual slots 63 / 64, +0xFC / +0x100),
// CThingCreatureBase::GetHeadPos(bool) @ 0x00661B40 (C3DVector by hidden pointer), AngleBetweenPositions @ 0x00661540 and
// the YZ twin @ 0x00754E70 (`__fastcall` (from, to), float), AngleDifference @ 0x00A441E0 (`__stdcall`, float).
// Globals: g_bQueryCreatureHeadPos 0x013BAE1B, 1e-4f 0x0129BA3C.
// Shape: IS_THING_ALIVE is the template helper taking `T* const&` (retail tests the pointer in eax and re-reads the member
// into ecx for the call; a macro or by-value helper keeps it in ecx), CTCLook::GetHeadPos (0x00751B30) is inlined
// (__forceinline: creature query or the cached copy through the user copy ctor), the three-axis tolerance test is an
// || chain with a shared false exit, and both arms end in the same AngleDifference(bodyYZ, target) tail.
// Layout (retail, as the landed GetHeadPos 0x00751B30): ParentThing +0x04 (PhysicsTC +0x60), PParentCreature +0x10,
// RecoveryAngleToLookAt XY/YZ +0x1c/+0x20, CachedHeadPos +0xa1 (packed, PDB +0xa5). No engine header for CTCLook.
#include <math.h>
#pragma intrinsic(fabs)

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

#define FILL10(n) virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3(); \
    virtual void v##n##4(); virtual void v##n##5(); virtual void v##n##6(); virtual void v##n##7(); \
    virtual void v##n##8(); virtual void v##n##9();
struct CTCPhysicsBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    FILL10(1) FILL10(2) FILL10(3) FILL10(4) FILL10(5)
    virtual void v60(); virtual void v61(); virtual void v62();
    virtual float GetFacingAngleXY();           // slot 63 (+0xFC)
    virtual float GetFacingAngleYZ();           // slot 64 (+0x100)
};
struct CThing {
    unsigned char _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
};
struct CThingCreatureBase {
    unsigned char _pad00[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    C3DVector GetHeadPos(bool exact) const;     // 0x00661B40
};
template<class T> inline bool IS_THING_ALIVE(T* const& p)
{
    if (p != 0) {
        if (!(p->m_flags91 & 1))
            return true;
    }
    return false;
}

float __fastcall AngleBetweenPositions(const C3DVector& from, const C3DVector& to);     // 0x00661540
float __fastcall AngleYZBetweenPositions(const C3DVector& from, const C3DVector& to);   // 0x00754E70
float __stdcall AngleDifference(float a, float b);                                      // 0x00A441E0
extern bool g_bQueryCreatureHeadPos;                                                    // 0x013BAE1B
extern float g_epsilon_0129ba3c;                                                        // 0x0129BA3C (1e-4f)

#pragma pack(push, 1)
struct CTCLook {
    void*               __vftable;              // +0x00
    CThing*             ParentThing;            // +0x04
    unsigned char       _pad08[0x10 - 0x08];
    CThingCreatureBase* PParentCreature;        // +0x10
    unsigned char       _pad14[0x1c - 0x14];
    float               RecoveryAngleToLookAtXY;   // +0x1c
    float               RecoveryAngleToLookAtYZ;   // +0x20
    unsigned char       _pad24[0xa0 - 0x24];
    bool                TargetAcquired;         // +0xa0 (PDB +0xa4)
    C3DVector           CachedHeadPos;          // +0xa1 (PDB +0xa5)
    __forceinline C3DVector GetHeadPos() const
    {
        if (g_bQueryCreatureHeadPos)
            return PParentCreature->GetHeadPos(false);
        return CachedHeadPos;
    }
    void GetAnglesFromBodyToPos(const C3DVector& pos, bool unused, float& angleXY, float& angleYZ) const;
};
#pragma pack(pop)

void CTCLook::GetAnglesFromBodyToPos(const C3DVector& pos, bool unused, float& angleXY, float& angleYZ) const
{
    float bodyXY = ParentThing->PhysicsTC->GetFacingAngleXY();
    float bodyYZ = ParentThing->PhysicsTC->GetFacingAngleYZ();
    if (IS_THING_ALIVE(PParentCreature)) {
        C3DVector headPos = GetHeadPos();
        if ((float)fabs(pos.X - headPos.X) > g_epsilon_0129ba3c || (float)fabs(pos.Y - headPos.Y) > g_epsilon_0129ba3c ||
            (float)fabs(pos.Z - headPos.Z) > g_epsilon_0129ba3c) {
            float xy = AngleBetweenPositions(headPos, pos);
            float yz = AngleYZBetweenPositions(headPos, pos);
            angleXY = AngleDifference(bodyXY, xy);
            angleYZ = AngleDifference(bodyYZ, yz);
            return;
        }
    }
    angleXY = AngleDifference(bodyXY, RecoveryAngleToLookAtXY);
    angleYZ = AngleDifference(bodyYZ, RecoveryAngleToLookAtYZ);
}
