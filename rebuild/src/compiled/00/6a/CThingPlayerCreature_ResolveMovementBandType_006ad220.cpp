// CThingPlayerCreature::ResolveMovementBandType() -- set the player's movement band (walk/jog/run...) from the accurate magnitude of its relative movement acceleration (the joystick deflection)
// Leaf: ?ResolveMovementBandType@CThingPlayerCreature@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006AD220, 62 bytes, thiscall, `ret`.
// Identity (probable): Ego_r.pdb donor order puts ?GetMovementBandTypeFromJoytickMagnitude@CThingPlayerCreature@@IBE?AW4EMovementBandType@@M@Z
//   right before ?ResolveMovementBandType@CThingPlayerCreature@@IAEXXZ, and this body is exactly "band =
//   GetMovementBandTypeFromJoytickMagnitude(|vector|)": it calls 0x006AB430 with the magnitude of the Ego_r
//   RelativeMovementAcceleration (+0x1E0 there, retail +0x1B4) and stores the enum into MovementBandType (Ego_r +0x22C,
//   retail +0x200: both sit after the 0x10-byte Dinkumware map that is 0xC in retail, plus the earlier container
//   savings). No direct callers (inlined at its call sites; this COMDAT copy is unreferenced). The catalog label
//   "C3DVector::GetAccurateMagnitude" names the inlined helper (Ego_r ?GetAccurateMagnitude@C3DVector@@QBEMXZ).
// Callees: CThingPlayerCreature::GetMovementBandTypeFromJoytickMagnitude(float) const 0x006AB430.
// Shape: C3DVector::GetAccurateMagnitude inline with the three members first copied into named locals (that fixes the load order X, Y, Z; the plain member expression loads Z first); `(float)sqrt` under intrinsic sqrt is a bare fsqrt).
extern "C" double __cdecl sqrt(double x);
#pragma intrinsic(sqrt)

#pragma pack(push, 1)
class C3DVector {
public:
    float X, Y, Z;
    float GetAccurateMagnitude() const { float x = X, y = Y, z = Z; return (float)sqrt(x * x + y * y + z * z); }
};
#pragma pack(pop)

enum EMovementBandType { MOVEMENT_BAND_NONE };

class CThingPlayerCreature {
public:
    unsigned char     _pad00[0x1B4];
    C3DVector         RelativeMovementAcceleration;     // +0x1B4
    unsigned char     _pad1c0[0x200 - 0x1C0];
    EMovementBandType MovementBandType;                 // +0x200
protected:
    EMovementBandType GetMovementBandTypeFromJoytickMagnitude(float magnitude) const;   // 0x006AB430
    void ResolveMovementBandType();
};

void CThingPlayerCreature::ResolveMovementBandType()
{
    MovementBandType = GetMovementBandTypeFromJoytickMagnitude(RelativeMovementAcceleration.GetAccurateMagnitude());
}