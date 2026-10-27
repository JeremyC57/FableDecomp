// CDelayLine::~CDelayLine() -- destructor of one reverb delay line: re-store its vtable and detach the pooled sample memory (data and size cleared; the reverb's pool owns the floats)
// Leaf: ??1CDelayLine@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C75400, 15 bytes, thiscall, plain ret. No callees.
// Identity: rebuild/oracles/reloc_bindings.tsv binds ??1CDelayLine@@UAE@XZ to 0x00C75400 (from the landed
//   CStereoDelayLine::CStereoDelayLine 0x00C75410, which passes it to `eh vector constructor iterator' as the element
//   destructor); the landed CStereoDelayLine::~CStereoDelayLine 0x00C75440 inlines the same two clears and calls it
//   through `__ehvec_dtor'. The vtable stored is the CDelayLine table 0x012BF074 (slot 0 = the scalar deleting
//   destructor 0x00C75470). The catalog label "Ctor_Init_012bf074_b" is a placeholder (it is a destructor). libpk has
//   no PDB/RTTI: agent444's descriptive class name.
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