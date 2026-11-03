// CTCHeroStats::GetFatnessReducingFrequencyInFrames() const -- frames between fatness reductions: ceil(fps * the def's fatness reduction rate * 60), or fps * 2440 when the rate is not above the minimum
// Leaf: GetFatnessReducingFrequencyInFrames
// Naming wave 18: the shared const float 1e-4f at 0x0129BA3C is ?DELTA_NOTIONAL_ZERO@@3MB (raw Ego_r public + typed data record; was EPSILON).
// Naming wave 18: the global stand-in g_framesPerSecond_01375550 (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail-asm: 0x0057A64D..0x0057A65C `fld [ebp-10h] ; fadd [ebp-0Ch] ; fistp dword ptr [ebp-4] ; fild [ebp-4] ; fsub [ebp-8] ; fstp [ebp-14h]` -- the engine's inline-assembly ceiling helper expanded in place (the product homed in the by-value copy, 0.5f stored as a frame constant and 1.0f through `fld1 ; fstp` in this size-optimised TU, a bare dword fistp into a named long). VC7.1 emits no bare dword fistp from C++ (`(long)x` calls __ftol2, /QIfist gives `fistp qword`, /arch:SSE2 cvttss2si: rasm1 probe family (a)); the fix-up `mov eax,value ; cmp eax,back ; jne ; dec dword ptr` is part of the same block (10 instructions, one jne: rasm1 family (b)), the result read back by C++.
// Helper (2026-11-03 asm twins): the retail inline-asm block is the shared engine/GFFloatToLongCeil.h (two-path: the byte-matching asm, and a FABLE_PORTABLE C++ twin); this row carries no asm of its own.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057A60A, 116 bytes (`leave ; ret`): a size-optimised TU with the frame pointer
// kept. Callees: CTCBase::GetDefinitionManager() @ 0x006869D0 (+0xDC its CDefPointer<CHeroStatsDef>; the def's rate float at
// +0x80). Globals: the frames-per-second long 0x01375550, 0.0001f @ 0x0129BA3C, 60.0f @ 0x0124F2FC.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};
extern const float DELTA_NOTIONAL_ZERO;               // 0x0129BA3C (0.0001f)
class CHeroStatsDef { public:
    unsigned char _pad00[0x80];
    float FatnessReductionRate;                 // +0x80
};
class CGameDefinitionManager { public:
    unsigned char  _pad00[0xdc];
    CHeroStatsDef* PHeroStatsDef;               // +0xdc
};
class CTCBase {
public:
    virtual ~CTCBase();
    class CThing* ParentThing;                  // +0x04
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0
public:
};
class CTCHeroStats : public CTCBase {
public:
    long GetFatnessReducingFrequencyInFrames() const;
};
// the engine's inline-assembly FPU ceiling helper (0x004FA9A0 is its standalone copy): VC7.1 expands the __forceinline body in
// place; the by-value parameter and the four locals become frame words, which is why the function carries an ebp frame.
// The whole 10-instruction block is asm (the bit compare and the memory decrement included: rasm1 family (b)); the result
// is read back by C++ after the block.
#include "engine/GFFloatToLongCeil.h"
long CTCHeroStats::GetFatnessReducingFrequencyInFrames() const
{
    float rate = GetDefinitionManager().PHeroStatsDef->FatnessReductionRate;
    if (rate > DELTA_NOTIONAL_ZERO)
        return GFFloatToLongCeil(CMainGameComponent::GetConstantFPS() * rate * 60.0f);
    return CMainGameComponent::GetConstantFPS() * 2440;
}