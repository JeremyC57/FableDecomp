// CTCLook::SetEyesLooking(const C3DVector& pos) -- point the eyes at pos: the eye angles are the differences from the head facing (body facing + head angle, wrapped) to the direction head->pos, or to the recovery angles when pos is the head position
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00752930, length 462 bytes (+ int3 padding), `ret 4`, esp frame.
// Callees: the physics TC's GetFacingAngleXY / GetFacingAngleYZ (slots 63 / 64, +0xFC / +0x100), _CIfmod @ 0x00BFEB28 (the
// intrinsic fmod of WrapAngle), CThingCreatureBase::GetHeadPos(bool) @ 0x00661B40, AngleBetweenPositions @ 0x00661540 and
// the YZ twin @ 0x00754E70 (`__fastcall`), AngleDifference @ 0x00A441E0 (`__stdcall`), CTCLook::ChangeEyeAngles @ 0x00752200.
// Globals: g_bQueryCreatureHeadPos 0x013BAE1B, 1e-4f 0x0129BA3C; 0.0f / 1.0f / double 1.0 of WrapAngle.
// Shape: the landed GetAnglesFromBodyToPos 0x00751C70 family: IS_THING_ALIVE `T* const&` template, the inline GetHeadPos
// (creature query or the packed cached copy through the user copy ctor), head facing = WrapAngle(body + HeadAngleRelToBody);
// the 1e-4 position test is an inline C3DVector::operator!= (written in place, VC lays the recovery arm first); both arms
// assign the two eye angles (the YZ AngleDifference call is shared) and one ChangeEyeAngles call follows.
// Layout (retail, local model as the landed CTCLook sources): ParentThing +0x04, PParentCreature +0x10, RecoveryAngleToLookAt
// XY/YZ +0x1c/+0x20, HeadAngleRelToBody XY/YZ +0x2c/+0x30, CachedHeadPos +0xa1 (packed, PDB +0xa5). No engine header.
#include <math.h>
#pragma intrinsic(fmod, fabs)

#define FILL10(n) virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3(); \
    virtual void v##n##4(); virtual void v##n##5(); virtual void v##n##6(); virtual void v##n##7(); \
    virtual void v##n##8(); virtual void v##n##9();
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    __forceinline bool operator!=(const C3DVector& o) const;
};
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
__forceinline float WrapAngle(float angle)
{
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)fmod((double)angle, 1.0);
        if (angle < 0.0f)
            angle += 1.0f;
    }
    return angle;
}

float __fastcall AngleBetweenPositions(const C3DVector& from, const C3DVector& to);     // 0x00661540
float __fastcall AngleYZBetweenPositions(const C3DVector& from, const C3DVector& to);   // 0x00754E70
float __stdcall AngleDifference(float a, float b);                                      // 0x00A441E0
extern bool g_bQueryCreatureHeadPos;                                                    // 0x013BAE1B
extern float g_epsilon_0129ba3c;                                                        // 0x0129BA3C (1e-4f)

__forceinline bool C3DVector::operator!=(const C3DVector& o) const
{
    return (float)fabs(X - o.X) > g_epsilon_0129ba3c || (float)fabs(Y - o.Y) > g_epsilon_0129ba3c ||
           (float)fabs(Z - o.Z) > g_epsilon_0129ba3c;
}
#pragma pack(push, 1)
struct CTCLook {
    void*               __vftable;              // +0x00
    CThing*             ParentThing;            // +0x04
    unsigned char       _pad08[0x10 - 0x08];
    CThingCreatureBase* PParentCreature;        // +0x10
    unsigned char       _pad14[0x1c - 0x14];
    float               RecoveryAngleToLookAtXY;   // +0x1c
    float               RecoveryAngleToLookAtYZ;   // +0x20
    float               BodyAngleXY;            // +0x24
    float               BodyAngleYZ;            // +0x28
    float               HeadAngleRelToBodyXY;   // +0x2c
    float               HeadAngleRelToBodyYZ;   // +0x30
    unsigned char       _pad34[0xa0 - 0x34];
    bool                TargetAcquired;         // +0xa0 (PDB +0xa4)
    C3DVector           CachedHeadPos;          // +0xa1 (PDB +0xa5)
    __forceinline C3DVector GetHeadPos() const
    {
        if (g_bQueryCreatureHeadPos)
            return PParentCreature->GetHeadPos(false);
        return CachedHeadPos;
    }
    void ChangeEyeAngles(float xy, float yz);   // 0x00752200
    void SetEyesLooking(const C3DVector& pos);
};
#pragma pack(pop)

void CTCLook::SetEyesLooking(const C3DVector& pos)
{
    if (IS_THING_ALIVE(PParentCreature)) {
        float headXY = WrapAngle(ParentThing->PhysicsTC->GetFacingAngleXY() + HeadAngleRelToBodyXY);
        float headYZ = WrapAngle(ParentThing->PhysicsTC->GetFacingAngleYZ() + HeadAngleRelToBodyYZ);
        C3DVector headPos = GetHeadPos();
        float eyeXY, eyeYZ;
        if (pos != headPos) {
            eyeXY = AngleDifference(headXY, AngleBetweenPositions(headPos, pos));
            eyeYZ = AngleDifference(headYZ, AngleYZBetweenPositions(headPos, pos));
        } else {
            eyeXY = AngleDifference(headXY, RecoveryAngleToLookAtXY);
            eyeYZ = AngleDifference(headYZ, RecoveryAngleToLookAtYZ);
        }
        ChangeEyeAngles(eyeXY, eyeYZ);
    }
}
