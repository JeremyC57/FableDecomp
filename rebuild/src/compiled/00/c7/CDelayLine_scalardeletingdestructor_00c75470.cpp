// CDelayLine::`scalar deleting destructor'(unsigned int flags) -- compiler-generated: the inlined destructor (vtable re-stored, data and size cleared), then the CRT operator delete (0x00BFE9BC) when bit 0 of the flags is set; returns this
// Leaf: ??_GCDelayLine@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C75470, 39 bytes, thiscall, ret 4. Callee: operator delete 0x00BFE9BC
//   (the CRT's, as the CStereoDelayLine deleting destructor 0x00C754A0 uses).
// Identity: slot 0 of the CDelayLine vtable 0x012BF074 (xref 0x012BF074; the destructor 0x00C75400 stores that table).
//   The catalog label (gap-scan) is a placeholder. Descriptive class name (libpk: no PDB/RTTI; agent444's CDelayLine).
// Shape: the destructor is defined in this TU, so VC7.1 inlines it into `??_G' (the out-of-line copy is 0x00C75400).
class CDelayLine {                                      // vtable 0x012BF074, 0x10 bytes (libpk reverb)
public:
    CDelayLine();                                       // 0x00C74F60
    virtual ~CDelayLine();                              // 0x00C75400
    float*       m_data;                                // +0x04 sample memory (owned by the reverb's pool)
    unsigned int m_size;                                // +0x08 (floats)
    unsigned int m_pos;                                 // +0x0c
};

CDelayLine::~CDelayLine()
{
    m_data = 0;
    m_size = 0;
}