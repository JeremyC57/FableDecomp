// C3DMeshStatsOwner_00a36100::HasDummyObject(const CCharString& name) -- ask the mesh statistics object at +8 whether the named dummy object exists (0x00A2EA20 with the name's CRC: Calc @ 0x004014A0 `__fastcall(0, chars, length)`, the empty literal 0x0129AAF4 for a null string)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A36100, length 64 bytes.
// Leaf: HasDummyObject
// Callees: Calc @ 0x004014A0, C3DMeshStats::HasDummyObject @ 0x00A2EA20. The chunk label (CMemoryDataInputStream::OpenStringSource) is propagated; named by address. Shape: the string accessors inlined before `push esi`, the statistics object in esi, `ret 4`.
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

struct C3DMeshStats {
    long HasDummyObject(unsigned long nameHash);   // 0x00A2EA20
};

struct C3DMeshStatsOwner_00a36100 {
    long          m_0;
    long          m_4;
    C3DMeshStats* m_pStats;                        // +0x08
    long HasDummyObject(const CCharString& name);
};

long C3DMeshStatsOwner_00a36100::HasDummyObject(const CCharString& name)
{
    unsigned long length = name.GetLength();
    const char* chars = name.GetChars();
    C3DMeshStats* stats = m_pStats;
    return stats->HasDummyObject(Calc_004014a0(0, chars, length));
}