// CTCStealth::GetPositionToMoveToForStabOn(CThing&, float&) const -- where the stealthy hero should stand to stab the victim: one unit back from the victim's stab position along the (normalised) line from the hero, and the facing angle (in turns, wrapped into [0, 1)) along that line
// Grade: DIFFER(326v326) -- frame slots only: retail puts the result position at +0x10..+0x18 (Z sharing the dead direction.Z word) and direction.X at +0x1C, ours direction.X at +0x10 and the position at +0x1C..+0x24.
// Flags: none
// Leaf: GetPositionToMoveToForStabOn
// Retail: Fable.exe (TLC, VC7.1) @ 0x00746EC0, 326 bytes. `ret 0Ch` (hidden C3DVector result, victim, angle out).
// Label correction: catalogued as "CCombatAction_StabThing::CalculateStabVector"; the Ego_r PDB names the shape
//   ?GetPositionToMoveToForStabOn@CTCStealth@@QBE?AVC3DVector@@AAVCThing@@AAM@Z, and `this+4` is the TC's
//   ParentThing.
// Callees: `__fastcall` NStealth::GetStabPosOnThing(CThing&) 0x00746DB0 (C3DVector by hidden pointer in ecx),
//          CThing::GetPosition 0x004C73D0, _CIfmod 0x00BFEB28 (fmod intrinsic); fpatan (atan2 intrinsic).
//          Globals: the reciprocal-square-root mantissa table 0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C,
//          1/(2pi) 0x01237F08, 0.0f 0x0122DEDC, 1.0f 0x0122DED8, the double 1.0 0x01237F00.
//
// Shape: the inline fast C3DVector::Normalise of the landed CTCAssassinRush row (0x007A6E20) with its squared
// length summed z, y, x (a `s = z*z; s += y*y; s += x*x;` chain), r homed in the dead victim argument slot; the
// angle is the landed CTCLook WrapAngle(atan2(X, Y) * 1/(2pi)) (0x00751E80) stored before the result vector
// (a named local) is copied out.
#include <math.h>
#pragma intrinsic(atan2, fmod)
extern unsigned long GFastOneOverSqrtTable[128];   // 0x013CE558

__forceinline float __fastcall FastOneOverSqrt(float value)
{
    unsigned long bits = *(unsigned long*)&value;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) | (GFastOneOverSqrtTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    return (3.0f - r * r * value) * r * 0.5f;
}
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector& operator*=(const float& s) { X *= s; Y *= s; Z *= s; return *this; }
    float GetLengthSquared() const { return X * X + (Y * Y + Z * Z); }
};
__forceinline float WrapAngle(float angle)
{
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)fmod((double)angle, 1.0);
        if (angle < 0.0f)
            angle += 1.0f;
    }
    return angle;
}

struct CThing {
    const C3DVector& GetPosition() const;   // 0x004C73D0
};
namespace NStealth {
C3DVector __fastcall GetStabPosOnThing(CThing& thing);   // 0x00746DB0
}

class CTCStealth {
public:
    virtual void v00();
    CThing* ParentThing;                    // +0x04
    C3DVector GetPositionToMoveToForStabOn(CThing& victim, float& angle) const;
};

C3DVector CTCStealth::GetPositionToMoveToForStabOn(CThing& victim, float& angle) const
{
    C3DVector stabPos = NStealth::GetStabPosOnThing(victim);
    C3DVector direction = stabPos - ParentThing->GetPosition();
    direction *= FastOneOverSqrt(direction.GetLengthSquared());
    C3DVector position = stabPos - direction;
    angle = WrapAngle((float)atan2(direction.X, direction.Y) * 0.15915494f);
    return position;
}
