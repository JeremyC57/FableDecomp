// CVtable_012bedcc::ClearScratch() -- vtable slot 1 of the libpk voice-mixer effect: zero the 128 bytes just below its 16-byte aligned scratch buffer (m_pScratch - 0x80) with sixteen MMX quadword stores, then emms
// Leaf: ?ClearScratch@CVtable_012bedcc@@UAEXXZ
// Retail-asm: 0x00C701F5..0x00C70237 `pxor mm0,mm0 ; movq qword ptr [edi],mm0 .. movq [edi+78h],mm0 ; emms` -- a hand MMX store block on a fixed edi loaded from a frame local (`mov [esp+4],eax ; mov edi,[esp+4]`): the VC7.1 RTM include set has no mmintrin.h (GOTCHAS 0xA44250); the same block as the free helper 0x00C700D0 and the landed MMX copy 0x00C70040. The pointer arithmetic, the local and the frame are C++.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C701E0, 92 bytes (+ int3 padding), thiscall, plain `ret`; slot 1 of the
//   vtable 0x012BEDCC (word at 0x012BEDD0; ctor 0x00C70120 landed as CVtable_012bedcc, whose second base sets the aligned
//   scratch pointer +0x1B8). The SSE2 twin class 0x012BEDE0 has 0x00C70BC0.
// Name: descriptive (libpk, no PDB/RTTI; the manifest label `_gapscan::sub_00c701e0` is a placeholder).
class CVtable_012beda4 {                               // vtable 0x012BEDA4 (ctor 0x00C6E6A0)
public:
    virtual void v00();
    unsigned char _body[0x24];                         // +0x04 lock owner, volume, flag
};
class CVtable_012bedcc : public CVtable_012beda4 {     // vtable 0x012BEDCC
public:
    virtual void ClearScratch();                       // slot 1 (0x00C701E0)
    unsigned char m_state[0x80];                       // +0x28
    unsigned char m_raw[0x110];                        // +0xa8 64 floats + 16, used 16-byte aligned
    float*        m_pScratch;                          // +0x1b8
};

void CVtable_012bedcc::ClearScratch()
{
    unsigned char* tail = (unsigned char*)m_pScratch - 0x80;
    __asm {
        mov     edi, tail
        pxor    mm0, mm0
        movq    [edi], mm0
        movq    [edi + 8h], mm0
        movq    [edi + 10h], mm0
        movq    [edi + 18h], mm0
        movq    [edi + 20h], mm0
        movq    [edi + 28h], mm0
        movq    [edi + 30h], mm0
        movq    [edi + 38h], mm0
        movq    [edi + 40h], mm0
        movq    [edi + 48h], mm0
        movq    [edi + 50h], mm0
        movq    [edi + 58h], mm0
        movq    [edi + 60h], mm0
        movq    [edi + 68h], mm0
        movq    [edi + 70h], mm0
        movq    [edi + 78h], mm0
        emms
    }
}