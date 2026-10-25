// CStereoDelayLine::Detach() -- detach both delay lines of the libpk reverb's stereo delay from their pooled sample memory: each line's data pointer and size zeroed (the position is kept)
// Leaf: ?Detach@CStereoDelayLine@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C75000, 15 bytes (+ int3 padding), thiscall, plain `ret`.
// Identity (sweep1, 2026-10-25; follows FINDINGS chunk 444, the libpk reverb family):
//   - the reverb (vtable 0x012B9594, agent443's CReverbProcessor) calls it on its +0x25c CStereoDelayLine from its
//     destructor 0x00C6A780 (call at 0x00C6A7DF) and its Shutdown 0x00C6A3D0 (tail jump at 0x00C6A405), right after
//     freeing the pooled memory (+0x280) the lines point into; also 0x00C6A905 and 0x00C69A66;
//   - +8/+0xc and +0x18/+0x1c are the data and size of the two 0x10-byte CDelayLines at +4 (agent444's CStereoDelayLine:
//     vtable 0x012BF078, lines' vtable 0x012BF074, data +4, size +8, position +0xc), the same two stores the destructor
//     0x00C75440 inlines.
//   The landed CQGuildTrainingSkillScript::Init is a propagated manifest label; descriptive names (libpk: no PDB/RTTI).
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
    virtual ~CStereoDelayLine();                        // 0x00C75440
    void Detach();
    CDelayLine m_lines[2];                              // +0x04
};

void CStereoDelayLine::Detach()
{
    m_lines[0].Detach();
    m_lines[1].Detach();
}
