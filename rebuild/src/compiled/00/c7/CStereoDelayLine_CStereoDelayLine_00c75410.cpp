// CStereoDelayLine::CStereoDelayLine() -- constructor of the reverb's two-channel delay (reverb +0x25C): vptr, then both CDelayLine elements through the `eh vector constructor iterator`
// Leaf: ??0CStereoDelayLine
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C75410, length 36 bytes. Callees: `eh vector constructor
//   iterator' 0x00F3FB96 (this+4, 0x10, 2, CDelayLine ctor 0x00C74F60, CDelayLine dtor 0x00C75400).
// Layout (descriptive; libpk reverb module): vtable 0x012BF078 (slot 0 = scalar deleting destructor 0x00C754A0),
//   two 0x10-byte CDelayLine at +4 (vtable 0x012BF074). The destructor is landed at 0x00C75440.
// Shape: an element type with an out-of-line constructor AND destructor under /EHsc gives the five-argument
//   iterator; the holder's own vptr store is scheduled after the pushes, before the call.
struct CDelayLine {
    virtual ~CDelayLine();                 // 0x00C75400
    float* m_data;                         // +0x04
    unsigned int m_size;                   // +0x08
    unsigned int m_pos;                    // +0x0C
    CDelayLine();                          // 0x00C74F60
};

struct CStereoDelayLine {
    virtual ~CStereoDelayLine();           // 0x00C75440
    CDelayLine m_lines[2];                 // +0x04
    CStereoDelayLine();
};

CStereoDelayLine::CStereoDelayLine()
{
}