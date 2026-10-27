// CWaveFile::ConvertSamplingRate(unsigned long newRate) -- linear-interpolation resample of the 16-bit sample vector to a new rate
// Retail-asm: 0x00AAD381..0x00AAD3A1 `fld [ebp - 0x20] ; fsub [ebp - 0x24] ; fistp [ebp - 0x18] ; fild [ebp - 0x18] ; fadd [ebp - 0x28] ; fstp [ebp - 0x1c] ; mov eax, [ebp - 0x20] ; cmp eax, [ebp - 0x1c] ; jne 0xaad39e ; inc [ebp - 0x18] ; fild [ebp - 0x18] ; mov edi, [ebp - 0x18]` and 0x00AAD3C8..0x00AAD3E8 `fld [ebp - 0x20] ; fsub [ebp - 0x24] ; fistp [ebp - 0x1c] ; fild [ebp - 0x1c] ; fadd [ebp - 0x28] ; fstp [ebp - 0x14] ; mov eax, [ebp - 0x20] ; cmp eax, [ebp - 0x14] ; jne 0xaad3e5 ; inc [ebp - 0x1c] ; mov ecx, [esi + 4] ; fild [esi + 4]` and 0x00AAD42A..0x00AAD449 `fld [ebp - 0x20] ; fsub [ebp - 0x24] ; fistp [ebp - 0x10] ; fild [ebp - 0x10] ; fadd [ebp - 0x28] ; fstp [ebp - 0x14] ; mov eax, [ebp - 0x20] ; cmp eax, [ebp - 0x14] ; jne 0xaad447 ; inc [ebp - 0x10] ; mov eax, [esi] ; imul eax, [ebp - 0x10]` and 0x00AAD4DE..0x00AAD4FD `fstp [ebp - 0x2c] ; fld [ebp - 0x2c] ; fistp [ebp - 0x30] ; mov eax, [ebp - 0x14] ; mov dx, word ptr [ebp - 0x30] ; add eax, 2 ; mov word ptr [eax - 2], dx ; mov [ebp - 0x14], eax ; mov eax, [esi] ; inc ecx ; cmp ecx, eax ; jb 0xaad4b0` -- the engine's inline-assembly floor helper expanded in place (0.5f / 1.0f stored as frame constants, a bare dword fistp into a named long, the bit compare and `jne ; inc dword ptr` in the same 10-instruction block: rasm1 family (b)). VC7.1 emits no bare dword fistp from C++ (`(long)x` calls __ftol2, /QIfist gives `fistp qword`, /arch:SSE2 cvttss2si: rasm1 probe family (a)); the result is read back by C++ after the block.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AAD340; reconstructed from the parked reference body (rebuild/candidates/manual/pending), the C++ rounding stand-in replaced by the inline-assembly helper. Three floor blocks (step, fraction step, new count / samples).
#pragma optimize("y", off)
#include <stdlib.h>
struct CShortVector {
    short* _M_start; short* _M_finish; short* _M_end_of_storage;
    CShortVector(unsigned long n);          // 0x00AADD80
    void Assign(CShortVector& other);       // 0x00AADE80
    ~CShortVector() { if (_M_start) free(_M_start); }
};
// the engine's inline-assembly FPU floor helper (0x0048D940 is its standalone copy): VC7.1 expands the __forceinline body in
// place; the by-value parameter and the four locals become frame words, which is why the function carries an ebp frame.
__forceinline long FloorToLong(float value)
{
    long  result;
    float one;
    float half;
    float next;
    half = 0.5f;
    one = 1.0f;
    __asm {
        fld   value
        fsub  half
        fistp result
        fild  result
        fadd  one
        fstp  next
        mov   eax, value
        cmp   eax, next
        jne   done
        inc   result
    done:
    }
    return result;
}

struct CWaveFile {
    unsigned long Channels;
    unsigned long NumSamples;
    unsigned long SampleRate;
    CShortVector Samples;
    void ConvertSamplingRate(unsigned long newRate);
};
void CWaveFile::ConvertSamplingRate(unsigned long newRate)
{
    float ratio = (float)SampleRate / (float)newRate;
    unsigned long step = FloorToLong(ratio);
    unsigned long fracStep = FloorToLong((ratio - (float)step) * 65536.0f);
    unsigned long newCount = FloorToLong((float)((double)NumSamples * (double)newRate / (double)SampleRate));
    CShortVector out(Channels * newCount);
    unsigned long pos = 0, frac = 0, inv = 0x10000, next = 1;
    short* dst = out._M_finish - out._M_start > 0 ? out._M_start : (short*)&out;
    for (unsigned long i = 0; i < newCount; ++i) {
        unsigned long hi = next;
        if (hi > NumSamples - 1) hi = NumSamples - 1;
        for (unsigned long c = 0; c < Channels; ++c) {
            long v = ((long)Samples._M_start[Channels * hi + c] * (long)frac + (long)Samples._M_start[Channels * pos + c] * (long)inv) >> 16;
            float f = (float)v;
            *dst++ = (short)FloorToLong(f);
        }
        pos += step; frac += fracStep; next += step; inv -= fracStep;
        if (frac > 0x10000) { frac -= 0x10000; ++pos; ++next; inv += 0x10000; }
    }
    NumSamples = newCount;
    SampleRate = newRate;
    Samples.Assign(out);
}
