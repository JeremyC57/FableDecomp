// CTCHeroStats::GetFatnessReducingFrequencyInFrames() const -- frames between fatness reductions: ceil(fps * the def's fatness reduction rate * 60), or fps * 2440 when the rate is not above the minimum
// Leaf: GetFatnessReducingFrequencyInFrames
// Retail-asm: 0x0057A64D..0x0057A65C `fld [ebp-10h] ; fadd [ebp-0Ch] ; fistp dword ptr [ebp-4] ; fild [ebp-4] ; fsub [ebp-8] ; fstp [ebp-14h]` -- the engine's inline-assembly ceiling helper expanded in place (the product homed in the by-value copy, 0.5f stored as a frame constant and 1.0f through `fld1 ; fstp` in this size-optimised TU, a bare dword fistp into a named long). VC7.1 emits no bare dword fistp from C++ (`(long)x` calls __ftol2, /QIfist gives `fistp qword`, /arch:SSE2 cvttss2si: rasm1 probe family (a)); the fix-up `mov eax,value ; cmp eax,back ; jne ; dec dword ptr` is part of the same block (10 instructions, one jne: rasm1 family (b)), the result read back by C++.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057A60A, 116 bytes (`leave ; ret`): a size-optimised TU with the frame pointer
// kept. Callees: CTCBase::GetDefinitionManager() @ 0x006869D0 (+0xDC its CDefPointer<CHeroStatsDef>; the def's rate float at
// +0x80). Globals: the frames-per-second long 0x01375550, 0.0001f @ 0x0129BA3C, 60.0f @ 0x0124F2FC.
#pragma optimize("y", off)
#pragma optimize("s", on)
extern long g_framesPerSecond_01375550;         // 0x01375550
extern const float g_minPositive_0129ba3c;      // 0x0129BA3C (0.0001f)
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
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0
};
class CTCHeroStats : public CTCBase {
public:
    long GetFatnessReducingFrequencyInFrames() const;
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
long CTCHeroStats::GetFatnessReducingFrequencyInFrames() const
{
    float rate = GetDefinitionManager().PHeroStatsDef->FatnessReductionRate;
    if (rate > g_minPositive_0129ba3c)
        return CeilToLong(g_framesPerSecond_01375550 * rate * 60.0f);
    return g_framesPerSecond_01375550 * 2440;
}