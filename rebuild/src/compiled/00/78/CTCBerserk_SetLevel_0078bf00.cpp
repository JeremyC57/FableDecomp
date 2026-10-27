// CTCBerserk::SetLevel() -- load the berserk tuning for the current spell level from the def: the duration in frames (rounded up) and the damage, scale and rumble parameters
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0078BF00, length 187 bytes (+ int3 padding). No calls.
// Globals: the frames-per-second counter 0x01375550.
// Layout: engine/CTCBerserk.h (Timer +0x0C, MaxIncreaseInStrength +0x18, BerserkDamageToMe +0x24, BerserkDamageToEnemy +0x28,
// BerserkScale +0x2C, RumbleLevel +0x30 .. RumbleDecay +0x3C, SpellLevel +0x44, PDef_Object +0x68); the def's per-level
// vectors are read through their first pointer (CSpecialAbilitiesBerserkDef modelled locally: no engine header).
// Shape: the frame count is the engine's inline-asm ceiling helper over fps * Duration[level]; PDef and SpellLevel are re-read
// after the asm barrier for the eight per-level copies (the def pointer in edx, level*4 in eax).
#pragma optimize("y", off)
// Retail-asm: 0x0078BF2A..0x0078BF44 `fld [ebp-10h] ; fadd [ebp-0Ch] ; fistp dword ptr [ebp-4] ; fild [ebp-4] ; fsub [ebp-8] ; fstp [ebp-14h] ; mov eax,[ebp-10h] ; cmp eax,[ebp-14h] ; jne ; dec dword ptr [ebp-4]` -- the engine's inline-assembly ceiling helper expanded in place (0.5f / 1.0f stored as frame constants, a bare dword fistp into a named long, the bit compare and `jne ; dec dword ptr` in the same 10-instruction block: rasm1 family (b)). VC7.1 emits no bare dword fistp from C++ (`(long)x` calls __ftol2, /QIfist gives `fistp qword`, /arch:SSE2 cvttss2si: rasm1 probe family (a)); the result is read back by C++ after the block.
class CSpecialAbilitiesBerserkDef { public:
    unsigned char _pad00[0x7c];
    float* Duration;                            // +0x7c vector<float> first
    unsigned char _pad80[0x88 - 0x80];
    float* DamageToEnemy;                       // +0x88
    unsigned char _pad8c[0x98 - 0x8c];
    float* DamageToMe;                          // +0x98
    unsigned char _pad9c[0xa4 - 0x9c];
    float* MaxIncreaseInStrength;               // +0xa4
    unsigned char _pada8[0xb0 - 0xa8];
    float* Scale;                               // +0xb0
    unsigned char _padb4[0xf0 - 0xb4];
    float* RumbleLevel;                         // +0xf0
    unsigned char _padf4[0xfc - 0xf4];
    float* RumbleSmoothness;                    // +0xfc
    unsigned char _pad100[0x108 - 0x100];
    float* RumbleAttack;                        // +0x108
    unsigned char _pad10c[0x114 - 0x10c];
    float* RumbleDecay;                         // +0x114
};
extern long g_FramesPerSecond;                  // 0x01375550
#include "engine/CTCBerserk.h"
struct CTCBerserk_Methods : CTCBerserk {         // the PDB layout header carries no methods; PDef_Object is the def pointer
    const CSpecialAbilitiesBerserkDef* GetDef() const { return (const CSpecialAbilitiesBerserkDef*)PDef_Object; }
    void SetLevel();
};
// the engine's inline-assembly FPU ceiling helper (0x004FA9A0 is its standalone copy): VC7.1 expands the __forceinline body in
// place; the by-value parameter and the four locals become frame words, which is why the function carries an ebp frame.
// The whole 10-instruction block is asm (the bit compare and the memory decrement included: rasm1 family (b)); the result
// is read back by C++ after the block.
__forceinline long CeilToLong(float value)
{
    long  result;
    float one;
    float half;
    float back;
    half = 0.5f;
    one = 1.0f;
    __asm {
        fld   value
        fadd  half
        fistp result
        fild  result
        fsub  one
        fstp  back
        mov   eax, value
        cmp   eax, back
        jne   done
        dec   result
    done:
    }
    return result;
}
void CTCBerserk_Methods::SetLevel()
{
    Timer = CeilToLong(g_FramesPerSecond * GetDef()->Duration[SpellLevel]);
    BerserkDamageToMe = GetDef()->DamageToMe[SpellLevel];
    BerserkDamageToEnemy = GetDef()->DamageToEnemy[SpellLevel];
    MaxIncreaseInStrength = GetDef()->MaxIncreaseInStrength[SpellLevel];
    BerserkScale = GetDef()->Scale[SpellLevel];
    RumbleLevel = GetDef()->RumbleLevel[SpellLevel];
    RumbleSmoothness = GetDef()->RumbleSmoothness[SpellLevel];
    RumbleAttack = GetDef()->RumbleAttack[SpellLevel];
    RumbleDecay = GetDef()->RumbleDecay[SpellLevel];
}