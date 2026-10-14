// CEngineSurfaceCompositionManager::GetDimensionsAndAdjustMipmapLevel(long* dimensions, CTextureEntry* texture, unsigned long* level) -- the texture's width and height (16-bit at +6 / +8 of its record at +0xc) shifted down by the level; while either exceeds the manager's limit at +0x10c8, halve both and raise the level (a word: `inc dword ptr`)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B2AEF0, length 91 bytes.
// Callees: none. Shape: `movzx` loads shifted by `cl` from the level word's low byte (re-read for the second), the loop re-reads the limit and both dimensions each pass and halves
// with the signed `cdq ; sub ; sar 1`; the loop head is aligned with `lea esp,[esp]`; `ret 0Ch`.
struct STextureRecord_00b2aef0 {
    unsigned char  _pad[6];
    unsigned short m_width;                        // +0x06
    unsigned short m_height;                       // +0x08
};

struct CTextureEntry_00b2aef0 {
    unsigned char            _pad[0xc];
    STextureRecord_00b2aef0* m_pRecord;            // +0x0c
};

struct CEngineSurfaceCompositionManager {
    unsigned char _pad[0x10c8];
    long          m_maxDimension;                  // +0x10c8
    void GetDimensionsAndAdjustMipmapLevel(long* dimensions, CTextureEntry_00b2aef0* texture, unsigned long* level);
};

void CEngineSurfaceCompositionManager::GetDimensionsAndAdjustMipmapLevel(long* dimensions, CTextureEntry_00b2aef0* texture, unsigned long* level)
{
    dimensions[0] = texture->m_pRecord->m_width >> *level;
    dimensions[1] = texture->m_pRecord->m_height >> *level;
    while (dimensions[0] > m_maxDimension || dimensions[1] > m_maxDimension) {
        dimensions[0] /= 2;
        dimensions[1] /= 2;
        ++*level;
    }
}
