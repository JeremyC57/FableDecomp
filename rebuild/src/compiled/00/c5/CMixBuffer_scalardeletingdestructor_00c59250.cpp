// CMixBuffer::`scalar deleting destructor'(unsigned int flags) -- compiler-generated: the destructor (0x00C57EF0), then the library's CPKObject::operator delete (0x00C3C370) when bit 0 of the flags is set; returns `this`
// Leaf: ??_GCMixBuffer@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C59250, 30 bytes (+ int3 padding), thiscall `ret 4`.
// Identity (sweep1, 2026-10-25; follows FINDINGS chunk 441 "CMixBuffer ctor + mix kernels"): slot 0 of the CMixBuffer
//   vtable 0x012B8F38 (slots 2..9 the mix kernels 0x00C57F00..0x00C58140 agent441 names; the landed CMixBuffer::CMixBuffer
//   0x00C591F0 stores it); it calls CMixBuffer::~CMixBuffer 0x00C57EF0 and frees through the class operator delete of the
//   libpk root (0x00C3C370, the stdcall free hook), not the CRT's 0x00BFE9BC. The landed CLandscapeBackgroundPatch vector
//   deleting destructor is a propagated label over a `T::VecDel` placeholder.
// Shape: the compiler-generated `??_G`; the vtable (and so `??_G`) is emitted by the out-of-line factory below, the
//   destructor stays an out-of-line call (VC7.1 does not inline it into `??_G` here).
class CPKObject {                                       // vtable 0x012B5868: the library's root class
public:
    virtual ~CPKObject() { }
    static void* __cdecl operator new(unsigned int size);   // 0x00C3C360
    static void __cdecl operator delete(void* p);           // 0x00C3C370
};

class CMixBuffer : public CPKObject {                   // vtable 0x012B8F38, 0xC34 bytes
public:
    CMixBuffer() { }                                    // out of line at 0x00C591F0
    virtual ~CMixBuffer();                              // 0x00C57EF0
    long*         m_pChannel[6];                        // +0x04 planar 128-sample accumulators
    long          m_active;                             // +0x1c
    unsigned long m_numChannels;                        // +0x20
    unsigned char m_storage[0xc10];                     // +0x24 accumulator store, 16-byte aligned by hand
};

CMixBuffer* Create_CMixBuffer()
{
    return new CMixBuffer;
}
