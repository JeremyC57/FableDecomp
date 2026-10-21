// Grade: DIFFER(328v326) -- after the inline fast normalise retail pops r to its frame slot and multiplies X, Y, Z by the memory operand (fstp r ; fxch ; fmul [r]); ours keeps r on the x87 stack (fst r ; fmul st(2)), which also shifts the result-vector frame slots
// CTCStealth::GetPositionToMoveToForStabOn(CThing&, float&) const -- where the stealthy hero should stand to stab the victim: one unit back from the victim's stab position along the (normalised) line from the hero, and the facing angle (in turns, wrapped into [0, 1)) along that line
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
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    __forceinline void Normalise()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = z * z;
        lengthSquared += y * y;
        lengthSquared += x * x;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        float r;
        *(unsigned long*)&r = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                              (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        r = (3.0f - r * r * lengthSquared) * r * 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
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
    direction.Normalise();
    C3DVector position = stabPos - direction;
    angle = WrapAngle((float)atan2(direction.X, direction.Y) * 0.15915494f);
    return position;
}
