// CDelayLine::CDelayLine() -- default constructor of the reverb's 0x10-byte delay line: vptr, empty buffer
// Leaf: ??0CDelayLine
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C74F60, length 17 bytes. No callees.
// Layout (descriptive; libpk reverb module, no PDB/RTTI): vtable 0x012BF074 (slot 0 = the scalar deleting
//   destructor 0x00C75470), the ring buffer {float* data +4, unsigned size +8, unsigned pos +0xC}; the write
//   position is left unset (Allocate 0x00C75050 / Clear 0x00C75010 zero it). Two of these lines make the
//   stereo delay at reverb +0x25C (ctor 0x00C75410 passes this body to the `eh vector constructor iterator`).
// Shape: `mov eax,ecx` first, one zero register for both member stores (init list in declaration order).
struct CDelayLine {
    virtual ~CDelayLine();                 // 0x00C75400
    float* m_data;                         // +0x04
    unsigned int m_size;                   // +0x08
    unsigned int m_pos;                    // +0x0C
    CDelayLine();
};

CDelayLine::CDelayLine()
    : m_data(0), m_size(0)
{
}