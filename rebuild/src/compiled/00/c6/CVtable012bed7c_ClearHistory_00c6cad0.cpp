// CVtable_012bed7c::ClearHistory() -- vtable slot 1 of the libpk multi-tap delay effect: zero the 64 bytes of sample history just below the aligned buffer (m_pAlignedBuffer - 0x40), the plain C path
// Leaf: ?ClearHistory@CVtable_012bed7c@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C6CAD0, 23 bytes (+ int3 padding), thiscall, plain `ret`; slot 1 of the
//   vtable 0x012BED7C (word at 0x012BED80). The derived MMX table 0x012B8AA0 overrides it with 0x00C6F300 (`pxor/movq`) and
//   the SSE2 table 0x012B8A64 with 0x00C6D560 (`xorps/movdqa`), the same 64 bytes (this chunk, retail_asm).
// Name: descriptive (libpk, no PDB/RTTI; the manifest label `_gapscan::sub_00c6cad0` is a placeholder); the model is the
//   landed constructor's (0x00C6CA20).
// Shape: the intrinsic memset of 0x40 bytes is `mov ecx,10h ; xor eax,eax ; rep stosd` (`this` moved to edx for edi).
extern "C" void* __cdecl memset(void* dest, int value, unsigned int bytes);
#pragma intrinsic(memset)
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

void CVtable_012bed7c::ClearHistory()
{
    memset(m_pAlignedBuffer - 0x40, 0, 0x40);
}