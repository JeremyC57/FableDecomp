// NThingShot::CTCShotDefinitionBase::GetInitialVelocity() -- the shot def's speed converted to per-frame units, scaled by a world-random factor in [0.8, 1.2]
// Leaf: GetInitialVelocity
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007BEFB0, length 63 bytes (+1 int3 padding).
// Callees: CTCBase::GetWorld @ 0x00686A70 (the manifest label "CViewBase::GetDisplayEngine" is propagated),
// CWorld::GetPRandomSeed @ 0x0049D810 (landed in 0x006E8730), GFFloatRandom(float range, unsigned long* seed) @ 0x004978A0
// (__fastcall: the range on the stack, the seed in ecx). Globals: g_GameFramesPerSecond (long @ 0x01375550); float
// constants 0.5f @ 0x0122F59C, 0.4f @ 0x0125A318, 1.0f @ 0x0122DED8.
// Layout: PShotDef (CDefPointer<const CThingShotDef>) at +0x38 (PDB +0x3c: the retail base's HitThingsList vector is 12
// bytes); the def's speed float at +0x3c.
// Shape: the per-frame speed is a named float local in the `push ecx` slot (`fild [g] ; fdivr [def+3Ch] ; fstp [esp]`);
// `(0.5f - random) * 0.4f + 1.0f` then `* speed` (fsubr / fmul / fadd / fmul [esp]).
struct CThingShotDef {
    unsigned char _pad[0x3c];
    float         Speed;                     // +0x3c
};

extern long g_GameFramesPerSecond;           // 0x01375550

float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0

struct CWorld {
    unsigned long* GetPRandomSeed();         // 0x0049D810
};

namespace NThingShot {

struct CTCShotDefinitionBase {
    unsigned char        _pad[0x38];
    const CThingShotDef* PShotDef;           // +0x38 (PDB +0x3c)
    CWorld* GetWorld();                      // 0x00686A70 (CTCBase)
    float GetInitialVelocity();
};

float CTCShotDefinitionBase::GetInitialVelocity()
{
    float speed = PShotDef->Speed / g_GameFramesPerSecond;
    return ((0.5f - GFFloatRandom(1.0f, GetWorld()->GetPRandomSeed())) * 0.4f + 1.0f) * speed;
}

}
