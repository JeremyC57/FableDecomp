// CMixBuffer::~CMixBuffer() -- destructor of the GameCODA mix accumulator: nothing to release; ends with the inlined CPKObject root destructor's vtable store (0x012B5868)
// Leaf: ??1CMixBuffer@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C57EF0, 7 bytes (+ int3 padding), thiscall, plain `ret`.
// Retail bytes:  mov dword ptr [ecx],012B5868h ; ret
// Identity (sweep1, 2026-10-25; follows FINDINGS chunk 441 "CMixBuffer ctor + mix kernels"):
//   - the scalar deleting destructor 0x00C59250 (slot 0 of the CMixBuffer vtable 0x012B8F38, whose constructor is the
//     landed CMixBuffer::CMixBuffer 0x00C591F0) calls this, then CPKObject::operator delete 0x00C3C370;
//   - the compiler-generated destructor of the derived CLimitedMixBuffer (0x00C3F030) calls it as its base destructor
//     after destroying the six limiters.
//   The landed CBase / SetMemberConst_00c57ef0 names are placeholders; descriptive names (GameCODA: no PDB/RTTI).
// Shape: a user-defined empty destructor: VC7.1 drops its own vptr store (0x012B8F38) as dead before the inlined root
//   destructor's store of 0x012B5868, which is the only store left.
class CPKObject {                                       // vtable 0x012B5868: the library's root class
public:
    virtual ~CPKObject() { }
    static void* __cdecl operator new(unsigned int size);   // 0x00C3C360
    static void __cdecl operator delete(void* p);           // 0x00C3C370
};

class CMixBuffer : public CPKObject {                   // vtable 0x012B8F38, 0xC34 bytes
public:
    CMixBuffer();                                       // 0x00C591F0
    virtual ~CMixBuffer();
    long*         m_pChannel[6];                        // +0x04 planar 128-sample accumulators
    long          m_active;                             // +0x1c
    unsigned long m_numChannels;                        // +0x20
    unsigned char m_storage[0xc10];                     // +0x24 accumulator store, 16-byte aligned by hand
};

CMixBuffer::~CMixBuffer()
{
}
