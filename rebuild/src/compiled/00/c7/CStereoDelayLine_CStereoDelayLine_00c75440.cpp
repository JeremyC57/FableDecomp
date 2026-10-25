// CStereoDelayLine::~CStereoDelayLine() -- destructor of the libpk reverb's two-channel delay line: its vtable (0x012BF078), both lines detached from their pooled sample memory (data and size zeroed inline), then the two CDelayLines destroyed through `__ehvec_dtor` (CDelayLine::~CDelayLine 0x00C75400)
// Leaf: ??1CStereoDelayLine@@UAE@XZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C75440, 39 bytes (+ int3 padding), thiscall, plain `ret`.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 444 "0xC75440 (CKrakenHolder = ~CStereoDelayLine)"):
//   - agent444's landed CStereoDelayLine::CStereoDelayLine 0x00C75410 stores the same vtable 0x012BF078 and builds the
//     same two 0x10-byte elements at +4 through `__ehvec_ctor` with CDelayLine::CDelayLine 0x00C74F60 and this array's
//     element destructor 0x00C75400 (vtable 0x012BF074: data +4, size +8, position +0xc); its Clear 0x00C75010 and
//     Allocate 0x00C75050 work on the same two lines;
//   - the reverb (vtable 0x012B9594, agent443's CReverbProcessor) holds one at +0x25c: its destructor 0x00C6A780 calls
//     this as the member destructor; 0x00C754A0 is the scalar deleting destructor around this body.
//   The CKrakenHolder / CTCKraken names are propagated (the Kraken boss has no audio DSP); descriptive names (libpk has no
//   PDB/RTTI), agent444's classes.
// Shape: the two inline detaches are interleaved with the `__ehvec_dtor` pushes; no EH frame (nothing after the call).
class CDelayLine {                                      // vtable 0x012BF074, 0x10 bytes
public:
    CDelayLine();                                       // 0x00C74F60
    virtual ~CDelayLine();                              // 0x00C75400
    void Detach() { m_data = 0; m_size = 0; }           // the sample memory belongs to the reverb's pool
    float*       m_data;                                // +0x04
    unsigned int m_size;                                // +0x08 (floats)
    unsigned int m_pos;                                 // +0x0c
};

class CStereoDelayLine {                                // vtable 0x012BF078, 0x24 bytes
public:
    CStereoDelayLine();                                 // 0x00C75410
    virtual ~CStereoDelayLine();
    CDelayLine m_lines[2];                              // +0x04
};

CStereoDelayLine::~CStereoDelayLine()
{
    m_lines[0].Detach();
    m_lines[1].Detach();
}
