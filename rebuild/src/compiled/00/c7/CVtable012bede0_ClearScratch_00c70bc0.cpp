// CVtable_012bede0::ClearScratch() -- vtable slot 1 of the libpk SSE2 voice-mixer effect: zero the 128 bytes just below its 16-byte aligned scratch buffer (m_pScratch - 0x80) with eight aligned 16-byte SSE stores
// Leaf: ?ClearScratch@CVtable_012bede0@@UAEXXZ
// Retail-asm: 0x00C70BD5..0x00C70BFA `xorps xmm0,xmm0 ; movdqa xmmword ptr [edi],xmm0 .. movdqa [edi+70h],xmm0` -- a hand SSE2 store block on a fixed edi loaded from a frame local (`mov [esp+4],eax ; mov edi,[esp+4]`): VC7.1 RTM has no intrinsic that names edi or keeps the pointer in a frame slot; the MMX twin of the same slot is 0x00C701E0 and the libpk SIMD kernels are hand asm (landed 0x00C5D4B0 / 0x00C6D370). The pointer arithmetic, the local and the frame are C++.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C70BC0, 66 bytes (+ int3 padding), thiscall, plain `ret`; slot 1 of the
//   vtable 0x012BEDE0 (word at 0x012BEDE4; ctor 0x00C70B00 landed as CVtable_012bede0, the aligned scratch pointer +0x1B8).
// Name: descriptive (libpk, no PDB/RTTI; the manifest label `_gapscan::sub_00c70bc0` is a placeholder).
class CVtable_012beda4 {                               // vtable 0x012BEDA4 (ctor 0x00C6E6A0)
public:
    virtual void v00();
    unsigned char _body[0x24];                         // +0x04 lock owner, volume, flag
};
class CVtable_012bede0 : public CVtable_012beda4 {     // vtable 0x012BEDE0
public:
    virtual void ClearScratch();                       // slot 1 (0x00C70BC0)
    unsigned char m_state[0x80];                       // +0x28
    unsigned char m_raw[0x110];                        // +0xa8 64 floats + 16, used 16-byte aligned
    float*        m_pScratch;                          // +0x1b8
};

void CVtable_012bede0::ClearScratch()
{
    unsigned char* tail = (unsigned char*)m_pScratch - 0x80;
    __asm {
        mov     edi, tail
        xorps   xmm0, xmm0
        movdqa  [edi], xmm0
        movdqa  [edi + 10h], xmm0
        movdqa  [edi + 20h], xmm0
        movdqa  [edi + 30h], xmm0
        movdqa  [edi + 40h], xmm0
        movdqa  [edi + 50h], xmm0
        movdqa  [edi + 60h], xmm0
        movdqa  [edi + 70h], xmm0
    }
}