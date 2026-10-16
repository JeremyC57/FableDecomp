// CTCContainerRewardHero::AdjustTrajectoryVector() -- catalog label; the body is a C3DVector fast normalise (returns *this): scale the vector to unit length with the table-driven 1/sqrt and one Newton step
//
// Retail 0x00486F60, 144 bytes, no calls. Globals: the 128-entry reciprocal-square-root mantissa table
// @ 0x013CE558, 3.0f @ 0x0122DED4, 0.5f @ 0x0122F59C.
// Label: `this` is the vector itself (x/y/z at +0/+4/+8) and the body is the 3D twin of the landed
// C2DVector::Normalise 0x00A14540 (the main C3DVector::Normalise is 0x00A14440); this out-of-line copy
// sits in another module's code, so the CTCContainerRewardHero label is propagated. Kept under the
// catalog leaf on a C3DVector model.
// Shape: the landed 0x00A14540 form with a third component: lenSq = x*x + y*y + z*z through the frame
// as bits, exponent (380 - e) / 2 (`shl 22 ; and 0FF800000h`), mantissa from the table, one Newton step
// `(3 - r*r*lenSq) * r * 0.5`; the leading `mov eax,ecx` is the `return *this` register.
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558

struct C3DVector {
    float X;    // +0x00
    float Y;    // +0x04
    float Z;    // +0x08
    C3DVector& AdjustTrajectoryVector();
};

C3DVector& C3DVector::AdjustTrajectoryVector()
{
    float x = X;
    float y = Y;
    float z = Z;
    float lengthSquared = x * x + y * y + z * z;
    unsigned long bits = *(unsigned long*)&lengthSquared;
    unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                             (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r = *(float*)&estimate;
    r = (3.0f - r * r * lengthSquared) * r * 0.5f;
    X *= r;
    Y *= r;
    Z *= r;
    return *this;
}
