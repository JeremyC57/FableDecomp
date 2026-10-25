// CStereoDelayLine::`scalar deleting destructor'(unsigned int flags) -- compiler-generated: the destructor (0x00C75440), then the CRT's operator delete (0x00BFE9BC) when bit 0 of the flags is set; returns `this`
// Leaf: ??_GCStereoDelayLine@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C754A0, 28 bytes (+ int3 padding), thiscall `ret 4`.
// Identity (sweep1, 2026-10-25; follows FINDINGS chunk 444, the libpk reverb family): the only slot of the CStereoDelayLine vtable 0x012BF078 (stored by agent444's CStereoDelayLine::CStereoDelayLine
//   0x00C75410); it calls CStereoDelayLine::~CStereoDelayLine 0x00C75440 (FINDINGS chunk 444). The reverb (vtable
//   0x012B9594) holds one at +0x25c.
//   The reverb parts have no class allocator (the CRT's 0x00BFE9BC, as CDspGain's 0x00C696A0). The landed
//   CLandscapeBackgroundPatch vector deleting destructor is a propagated label over a placeholder; descriptive names
//   (libpk: no PDB/RTTI).
// Shape: the compiler-generated `??_G`; the vtable (and so `??_G`) is emitted by the out-of-line factory below, the
//   destructor stays an out-of-line call.
class CDelayLine {                                      // vtable 0x012BF074, 0x10 bytes
public:
    CDelayLine();                                       // 0x00C74F60
    virtual ~CDelayLine();                              // 0x00C75400
    float*       m_data;                                // +0x04
    unsigned int m_size;                                // +0x08
    unsigned int m_pos;                                 // +0x0c
};

class CStereoDelayLine {                                // vtable 0x012BF078, 0x24 bytes
public:
    CStereoDelayLine() { }                              // out of line at 0x00C75410
    virtual ~CStereoDelayLine();                        // 0x00C75440
    CDelayLine m_lines[2];                              // +0x04
};

CStereoDelayLine* Create_CStereoDelayLine()
{
    return new CStereoDelayLine;
}
