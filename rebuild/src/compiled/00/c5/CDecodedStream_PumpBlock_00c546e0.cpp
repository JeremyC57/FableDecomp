// CDecodedStream::PumpBlock() -- decode one 128-sample block into four zeroed, 16-byte-aligned channel buffers (resetting the decoder first when a restart is pending) and hand it to the sink when the decode step succeeded
// Leaf: ?PumpBlock@CDecodedStream@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C546E0, length 152 bytes (+ int3 padding); slot 7 of the vtable 0x012B89C4
// (slot 0 the landed scalar deleting destructor 0x00C53AE0).
// Callees: the decoder reset 0x00C6B5D0 (__cdecl, the decoder at +0x21c), the progress monitor's UpdateProgress 0x00C6AF50
// (landed: (source, quiet, context, amount), `ret 10h`), the sink's Write 0x00C58520 (thiscall, the first buffer).
// No PDB/RTTI for this libpk class: the names are descriptive; the catalog label "Audio_ApplyEffectChain" is descriptive too.
// Shape: `__declspec(align(16))` on the 4 x 128 float block gives the `and esp,-16` frame (0x818 bytes: the pointer array
// at +8, the block at +0x18, both 16-byte aligned); the pointer array is built before the restart test; the explicit
// memset is the `rep stosd` (0x200 dwords) after the reset call; the bool member passes as `xor eax,eax ; mov al,[esi+8]`.
extern "C" void* __cdecl memset(void* dest, int value, unsigned int count);
#pragma intrinsic(memset)

void __cdecl ResetDecoder(void* decoder);                       // 0x00C6B5D0

struct CStreamProgressMonitor {
    bool UpdateProgress(void* source, bool quiet, void* context, long amount);   // 0x00C6AF50
};

struct CSampleSink {
    void Write(float* samples);                                 // 0x00C58520
};

struct CDecodedStream {
    void*                  __vftable;                           // +0x00 (vtable 0x012B89C4)
    long                   m_4;                                 // +0x04
    bool                   m_restartPending;                    // +0x08
    unsigned char          _pad9[0x213];
    void*                  m_pDecoder;                          // +0x21c
    CSampleSink*           m_pSink;                             // +0x220
    unsigned char          _pad224[0xd4];
    CStreamProgressMonitor m_monitor;                           // +0x2f8
    void PumpBlock();                                           // vtable slot 7
};

void CDecodedStream::PumpBlock()
{
    __declspec(align(16)) float samples[4][128];
    float* channels[4] = { samples[0], samples[1], samples[2], samples[3] };
    if (m_restartPending)
        ResetDecoder(m_pDecoder);
    memset(samples, 0, sizeof(samples));
    if (m_monitor.UpdateProgress(m_pDecoder, m_restartPending, channels, 128))
        m_pSink->Write(samples[0]);
}