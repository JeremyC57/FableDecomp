// CMusicManager_00780410::ResetMusicEntriesOffsets() -- clear the offset (+0xc) of every entry of the vector at +0x10 (0x2c-byte entries)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00780410, length 32 bytes.
// Leaf: ResetMusicEntriesOffsets
// The chunk label is propagated; named by address. Shape: no frame, the last pointer re-read into esi each iteration, the loop head aligned, `ret`.
struct SMusicEntry_00780410 {
    unsigned char _pad00[0xc];
    long          m_offset;                        // +0xc
    unsigned char _pad[0x2c - 0xc - 4];
};

struct CMusicManager_00780410 {
    unsigned char      _pad00[0x10];
    SMusicEntry_00780410* m_first;                    // +0x10
    SMusicEntry_00780410* m_last;
    SMusicEntry_00780410* m_end;
    void ResetMusicEntriesOffsets();
};

void CMusicManager_00780410::ResetMusicEntriesOffsets()
{
    for (SMusicEntry_00780410* entry = m_first; entry != m_last; ++entry)
        entry->m_offset = 0;
}