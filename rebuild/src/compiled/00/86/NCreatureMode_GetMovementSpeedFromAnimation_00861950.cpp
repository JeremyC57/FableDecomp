// NCreatureMode::GetMovementSpeedFromAnimation(long bankIndex, const CThingCreatureBase& creature, const CMeshDataBank& bank) -- the ground speed an animation moves the creature at: movement-vector length * 0.01 * appearance scale * physical scale / (frames per second * duration); 0 for an (almost) zero duration
// Leaf: ?GetMovementSpeedFromAnimation@NCreatureMode@@YIMJ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00861950, length 110 bytes (+2 int3). `__fastcall` free function (bank index
//   in ecx, creature in edx, the bank on the stack, `ret 4`); float in st(0).
// Mangled (Ego_r.pdb): ?GetMovementSpeedFromAnimation@NCreatureMode@@YIMJABVCThingCreatureBase@@ABVCMeshDataBank@@@Z.
// Name: the catalog row says "GetMovementSpeedModifier" (descriptive). Its callers are the CCharString/CAnimationEntry
//   overloads (0x00861C90 passes `entry->BankIndex` in ecx), so it is the `long` overload of the PDB's four.
// Callees: CMeshDataBank::PeekAnimation(unsigned long) const 0x00A242C0 (manifest "CTCInventoryAbilities::GetAbilityLevel" is
//   propagated). Data: 1.0e-4f @ 0x0129BA3C, 0.01f @ 0x01230A5C, 0.0f @ 0x0122DEDC, g_FramesPerSecond (long) @ 0x01375550.
// Layout: C3DAnimationInfo Duration +0x10, MovementVector (a C3DVector member in Ego_r.pdb) +0x18 -- local model, because
//   engine/C3DAnimationInfo.h flattens the vector into three floats; the creature's graphic appearance pointer +0x64 with
//   MainGraphic.RenderSizeX at +0x4C (landed model 0x004BC750 reads it as a percentage, `RenderSizeX * Scale * 0.01f`) and
//   CThingPhysical::Scale +0x9C (landed models).
// Shape: the duration is a float local kept on the x87 stack across the `> 1e-4` test (`fcom ; test ah,41h ; jne`); the
//   length is the inline member `MovementVector.GetLength()` = sqrt(X*X + Y*Y + Z*Z) read straight from the info (VC evaluates
//   the right operand of a float `+` first, so the components load Z, Y, X; a C3DVector built from the three floats loads
//   them X, Y, Z); the named `distance = length * 0.01f` local keeps the constant multiply right after fsqrt (one long
//   product chain moves it to the end); `fild fps ; fmul duration ; fdivp`.
extern "C" double __cdecl sqrt(double x);
#pragma intrinsic(sqrt)
class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
};
class C3DAnimationInfo {                                // Ego_r.pdb, 0x28 bytes (engine/C3DAnimationInfo.h flattens MovementVector
public:                                                 //   into three floats; the member C3DVector is kept here)
    void*         _vfptr;                               // +0x00
    unsigned char _pad04[0x10 - 0x04];
    float         Duration;                             // +0x10
    float         NonLoopingDuration;                   // +0x14
    C3DVector     MovementVector;                       // +0x18
    float         Rotation;                             // +0x24
};
class CMeshDataBank {
public:
    const C3DAnimationInfo* PeekAnimation(unsigned long index) const;   // 0x00A242C0
};
class CTCGraphicAppearance {
public:
    unsigned char _pad00[0x4c];
    float         RenderSizeX;                          // +0x4C MainGraphic.RenderSizeX (percent; landed model 0x004BC750)
};
class CThingCreatureBase {
public:
    unsigned char         _pad00[0x64];
    CTCGraphicAppearance* PGraphicAppearance;           // +0x64
    unsigned char         _pad68[0x9c - 0x68];
    float                 Scale;                        // +0x9C (CThingPhysical)
};
extern long g_FramesPerSecond;                          // 0x01375550
namespace NCreatureMode {
float __fastcall GetMovementSpeedFromAnimation(long bankIndex, const CThingCreatureBase& creature, const CMeshDataBank& bank);
}

float __fastcall NCreatureMode::GetMovementSpeedFromAnimation(long bankIndex, const CThingCreatureBase& creature, const CMeshDataBank& bank)
{
    const C3DAnimationInfo* info = bank.PeekAnimation(bankIndex);
    float duration = info->Duration;
    if (duration > 0.0001f) {
        float distance = info->MovementVector.GetLength() * 0.01f;
        return distance * creature.PGraphicAppearance->RenderSizeX * creature.Scale / (g_FramesPerSecond * duration);
    }
    return 0.0f;
}