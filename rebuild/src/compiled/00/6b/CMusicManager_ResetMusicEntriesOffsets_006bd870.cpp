// CMusicManager_006bd870::ResetMusicEntriesOffsets() -- clear the offset (+0x10) of every entry of the vector at +0x1c (0x18-byte entries)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BD870, length 32 bytes.
// Leaf: ResetMusicEntriesOffsets
// The chunk label is propagated; named by address. Shape: no frame, the last pointer re-read into esi each iteration, the loop head aligned, `ret`.
struct SMusicEntry_006bd870 {
    unsigned char _pad00[0x10];
    long          m_offset;                        // +0x10
    unsigned char _pad[0x18 - 0x10 - 4];
};

struct CMusicManager_006bd870 {
    unsigned char      _pad00[0x1c];
    SMusicEntry_006bd870* m_first;                    // +0x1c
    SMusicEntry_006bd870* m_last;
    SMusicEntry_006bd870* m_end;
    void ResetMusicEntriesOffsets();
};

void CMusicManager_006bd870::ResetMusicEntriesOffsets()
{
    for (SMusicEntry_006bd870* entry = m_first; entry != m_last; ++entry)
        entry->m_offset = 0;
}