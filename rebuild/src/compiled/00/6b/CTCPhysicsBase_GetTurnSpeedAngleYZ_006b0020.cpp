// CTCPhysicsBase::GetTurnSpeedAngleYZ() const -- the change in the forward axis's YZ (elevation) angle since the last frame: current RH set forward angle minus the last one, each a fraction of a turn wrapped into [0, 1)
// Grade: RELOCATION_MATCH (retry wave 2 sec2b): the difference goes through a named result local (second angle stays on the x87 stack, one fsubr [esp] join), as 0x006AFF40
// Leaf: ?GetTurnSpeedAngleYZ@CTCPhysicsBase@@QBEMXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006B0020, 258 bytes (+ int3 padding to 272), thiscall, plain `ret`, float in
//   st(0).
// Identity: the catalog's "ComputeAngleHeading" is a descriptive auto label. The body sits in the CTCPhysicsBase TU
//   (0x006AFxxx..0x006B0xxx); FableWin.pdb lists ?GetTurnSpeedAngleXY/XZ/YZ@CTCPhysicsBase@@QBEMXZ and
//   ?GetAngleYZ@C3DVector@@QBEMXZ, and the elevation form atan2(z, sqrt(x*x + y*y)) is the landed GFGetAngleYZTo
//   (0x00754E70). No direct callers in the image (an unreferenced out-of-line copy).
// Callees: virtual slots 72 (+0x120, the current right-handed set) and 73 (+0x124, the last one; the landed 0x006B0460
//   names both), _CIfmod 0x00BFEB28 (the intrinsic fmod, twice). Constants: 1/(2*pi) 0x01237F08, 0.0f 0x0122DEDC, 1.0f
//   0x0122DED8, 1.0 (double) 0x01237F00. CRightHandedSet = {Up +0, Forward +0xC} (Ego_r.pdb).
// Shape: both virtual calls first, then the inline C3DVector::GetAngleYZ (fsqrt / fpatan / * 1/2pi) and the landed
//   __forceinline WrapAngle on each forward axis; the first angle is kept in a frame float for the final `fsubr`.
#include <math.h>
#pragma intrinsic(atan2, sqrt, fmod)

__forceinline float WrapAngle(float angle)
{
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)fmod((double)angle, 1.0);
        if (angle < 0.0f)
            angle += 1.0f;
    }
    return angle;
}

class C3DVector {
public:
    float X, Y, Z;
    __forceinline float GetAngleYZ() const
    {
        float angle = (float)atan2(Z, sqrt(X * X + Y * Y)) * 0.15915494f;
        return WrapAngle(angle);
    }
};

class CRightHandedSet {
public:
    C3DVector Up;                                           // +0x00
    C3DVector Forward;                                      // +0x0C
};

#define VF(n) virtual void v##n();
class CTCPhysicsBase {
public:
    VF(00) VF(01) VF(02) VF(03) VF(04) VF(05) VF(06) VF(07) VF(08) VF(09)
    VF(10) VF(11) VF(12) VF(13) VF(14) VF(15) VF(16) VF(17) VF(18) VF(19)
    VF(20) VF(21) VF(22) VF(23) VF(24) VF(25) VF(26) VF(27) VF(28) VF(29)
    VF(30) VF(31) VF(32) VF(33) VF(34) VF(35) VF(36) VF(37) VF(38) VF(39)
    VF(40) VF(41) VF(42) VF(43) VF(44) VF(45) VF(46) VF(47) VF(48) VF(49)
    VF(50) VF(51) VF(52) VF(53) VF(54) VF(55) VF(56) VF(57) VF(58) VF(59)
    VF(60) VF(61) VF(62) VF(63) VF(64) VF(65) VF(66) VF(67) VF(68) VF(69)
    VF(70) VF(71)
    virtual const CRightHandedSet& GetLastRHSet() const;    // slot 72 (+0x120), Ego_r ?GetLastRHSet@CTCPhysicsBase@@UBEABVCRightHandedSet@@XZ (landed OnDie 0x00663F50)
    virtual const CRightHandedSet& GetOldRHSet() const;     // slot 73 (+0x124), descriptive (previous-frame set, as 0x006AFF40)
    float GetTurnSpeedAngleYZ() const;
};

float CTCPhysicsBase::GetTurnSpeedAngleYZ() const
{
    const CRightHandedSet& current = GetLastRHSet();
    const CRightHandedSet& last = GetOldRHSet();
    float result = current.Forward.GetAngleYZ() - last.Forward.GetAngleYZ();
    return result;
}