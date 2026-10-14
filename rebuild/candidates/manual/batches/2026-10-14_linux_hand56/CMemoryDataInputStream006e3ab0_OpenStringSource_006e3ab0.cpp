// CMemoryDataInputStream_006e3ab0::OpenStringSource(const CCharString& name, long a, long b, long c) -- look the named string up in the world's text vector map (global 0x013B86A0 -> +0x14, CVectorMap::LookupValue @ 0x009CC410 with the name's CRC: Calc @ 0x004014A0 `__fastcall(0, chars, length)`, the empty literal 0x0129AAF4 for a null string), then play it (0x006E3890 with (the entry, the record at +0x38, a, b, c))
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E3AB0, length 96 bytes.
// Leaf: OpenStringSource
// Callees: Calc @ 0x004014A0, CVectorMap::LookupValue @ 0x009CC410, 0x006E3890 (labelled CTCTalk::PlaySpeech). The chunk label is propagated; named by address. Shape: the map read before the string, `this` in esi, the map in edi, `ret 10h`.
struct CCharStringData {
    const char*   m_pChars;                        // +0x00
    unsigned long m_length;                        // +0x04
};
extern const char g_emptyLiteral_0129aaf4[];       // 0x0129AAF4: ""
struct CCharString {
    CCharStringData* m_pStringData;
    unsigned long GetLength() const { return m_pStringData == 0 ? 0 : m_pStringData->m_length; }
    const char* GetChars() const { return m_pStringData == 0 ? g_emptyLiteral_0129aaf4 : m_pStringData->m_pChars; }
};
unsigned long __fastcall Calc_004014a0(unsigned long seed, const char* data, unsigned long length);   // 0x004014A0: CRC

struct CVectorMap_006e3ab0 {
    void* LookupValue(unsigned long key);          // 0x009CC410
};
struct SWorldText_006e3ab0 {
    unsigned char       _pad00[0x14];
    CVectorMap_006e3ab0* m_pTextMap;               // +0x14
};
extern SWorldText_006e3ab0* g_pWorldText_013b86a0;   // 0x013B86A0

struct CMemoryDataInputStream_006e3ab0 {
    unsigned char _pad00[0x38];
    unsigned char m_record[4];                     // +0x38
    void Play(void* entry, void* record, long a, long b, long c);   // 0x006E3890
    void OpenStringSource(const CCharString& name, long a, long b, long c);
};

void CMemoryDataInputStream_006e3ab0::OpenStringSource(const CCharString& name, long a, long b, long c)
{
    CVectorMap_006e3ab0* map = g_pWorldText_013b86a0->m_pTextMap;
    void* entry = map->LookupValue(Calc_004014a0(0, name.GetChars(), name.GetLength()));
    Play(entry, m_record, a, b, c);
}
