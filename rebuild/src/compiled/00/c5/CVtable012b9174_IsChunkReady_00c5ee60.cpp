// CVtable_012b9174::IsChunkReady() -- vtable slot 6 of the silent output: ready while fewer blocks than the buffer holds were submitted; at the end of the buffer, report one not-ready poll, then restart the count and report ready again
// Leaf: ?IsChunkReady@CVtable_012b9174@@UAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5EE60, 51 bytes (+ int3 padding), thiscall, plain `ret`; slot 6 of the
//   vtable 0x012B9174 (word at 0x012B918C).
// Name: descriptive (no PDB/RTTI); the slot parallels the landed CDSoundStreamBuffer::IsChunkReady 0x00C57070. The manifest
//   label `_gapscan::sub_00c5ee60` is a placeholder.
// Shape: one zero register (`xor eax,eax`) for the flag compare and the two clears; the `>=` test falls through.
// The silent (null) output of the audio module (vtable 0x012B9174, no RTTI; ctor 0x00C5EEC0 landed as CVtable_012b9174,
// which nulls +0x604): it plays nothing and paces the mixer by counting blocks. Slots parallel the waveOut output
// 0x012B8FCC: 3 Initialize(channels, bytes) 0x00C5EE30 (+0x604 = bytes >> 7, clears +0x608 / +0x60C), 5 Close 0x00C5EE50
// (+0x604 = 0), 6 the chunk-ready test 0x00C5EE60, 7 the chunk pointer 0x00C5EEA0, 8 the submit 0x00C5EEB0.
class CVtable_012b9174 {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05();
    virtual bool IsChunkReady();                    // slot 6 (0x00C5EE60)
    virtual void* LockChunk();                      // slot 7 (0x00C5EEA0)
    virtual void WriteBuffer(long unused);          // slot 8 (0x00C5EEB0)
    unsigned char _body[0x600];                     // +0x004
    unsigned long m_blockCount;                     // +0x604 blocks per buffer (bytes >> 7)
    unsigned long m_written;                        // +0x608 blocks submitted
    bool          m_waited;                         // +0x60c one empty poll done
};

bool CVtable_012b9174::IsChunkReady()
{
    if (m_written >= m_blockCount) {
        if (!m_waited) {
            m_waited = true;
            return false;
        }
        m_waited = false;
        m_written = 0;
    }
    return true;
}