// CVtable_012b8a64::ClearHistory() -- vtable slot 1 of the SSE2 multi-tap delay effect: zero the 64 bytes of sample history below the aligned buffer (m_pAlignedBuffer - 0x40) with four aligned 16-byte SSE stores
// Leaf: ?ClearHistory@CVtable_012b8a64@@UAEXXZ
// Retail-asm: 0x00C6D573..0x00C6D584 `xorps xmm0,xmm0 ; movdqa xmmword ptr [edi],xmm0 ; movdqa [edi+10h],xmm0 ; movdqa [edi+20h],xmm0 ; movdqa [edi+30h],xmm0` -- a hand SSE2 store block on a fixed edi loaded from a frame local (`mov [esp+4],eax ; mov edi,[esp+4]`, the asm's `mov edi,history`): VC7.1 RTM has no intrinsic that names edi or keeps the pointer in a frame slot, and the C path of the same slot (0x00C6CAD0) compiles to `rep stosd`; the libpk SIMD kernels are hand asm (GOTCHAS rasm1; landed twins 0x00C5D4B0 / 0x00C6D370). The pointer arithmetic, the local and the frame are C++.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C6D560, 44 bytes (+ int3 padding), thiscall, plain `ret`; slot 1 of the
//   vtable 0x012B8A64 (word at 0x012B8A68), the SSE2 derivative of the multi-tap delay effect: ctor 0x00C55070 = the base
//   ctor 0x00C6CA20 (CVtable_012bed7c) then this vtable (landed under the propagated label "C3DAnimationUtil").
// Name: descriptive (libpk, no PDB/RTTI); the base slot 1 is CVtable_012bed7c::ClearHistory 0x00C6CAD0 (this chunk).
class CVtable_012beda4 {                               // vtable 0x012BEDA4, 0x28 bytes (ctor 0x00C6E6A0)
public:
    virtual void v00();
    unsigned char _body[0x24];                         // +0x04 lock owner, volume, flag
};
struct SRecord_00c6ca20 {                              // twelve words (the landed ctor's tap set)
    long m_words[12];
};
// The multi-tap delay mixer effect (vtable 0x012BED7C, ctor 0x00C6CA20 landed as CVtable_012bed7c): 128 input samples
// are copied into the 16-byte aligned history buffer at m_pAlignedBuffer (+0x178), and the taps read up to 64 bytes
// (32 samples) back from it, into the tail of the previous block just below the aligned pointer.
class CVtable_012bed7c : public CVtable_012beda4 {     // vtable 0x012BED7C
public:
    virtual void ClearHistory();                       // slot 1 (0x00C6CAD0; MMX 0x00C6F300, SSE2 0x00C6D560)
    unsigned char     m_state[0x40];                   // +0x28
    unsigned char     m_buffer[0x110];                 // +0x68 scratch storage, used 16-byte aligned
    unsigned char*    m_pAlignedBuffer;                // +0x178
    SRecord_00c6ca20  m_first;                         // +0x17c
    SRecord_00c6ca20  m_second;                        // +0x1ac
    SRecord_00c6ca20* m_pCurrent;                      // +0x1dc
    SRecord_00c6ca20* m_pPrevious;                     // +0x1e0
    long              m_valuesA[6];                    // +0x1e4
    long              m_valuesB[6];                    // +0x1fc
};
class CVtable_012b8a64 : public CVtable_012bed7c {    // vtable 0x012B8A64
public:
    virtual void ClearHistory();                       // slot 1 (0x00C6D560)
};

void CVtable_012b8a64::ClearHistory()
{
    unsigned char* history = m_pAlignedBuffer - 0x40;
    __asm {
        mov     edi, history
        xorps   xmm0, xmm0
        movdqa  [edi], xmm0
        movdqa  [edi + 10h], xmm0
        movdqa  [edi + 20h], xmm0
        movdqa  [edi + 30h], xmm0
    }
}