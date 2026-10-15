#include <cstdio>
#include <cstring>
// Behaviour of CWADFile::Open (0x00A39CC0): the entry index is the bank's lookup of the CRC of the narrowed name
// (crc0 over its text, the empty text when the string has no data), and the file position is rewound.
struct CCharStringData { const char* m_pText; unsigned long m_length; };
static const char g_EmptyString[] = "";
static int g_live = 0;
struct CCharString {
    CCharStringData* m_pData;
    CCharString() : m_pData(0) { ++g_live; }
    CCharString(const CCharString& o) : m_pData(o.m_pData) { ++g_live; }
    ~CCharString() { --g_live; }
    unsigned long GetLength() const { return m_pData == 0 ? 0 : m_pData->m_length; }
    const char* GetData() const { return m_pData == 0 ? g_EmptyString : m_pData->m_pText; }
};
static CCharStringData g_narrow;
struct CWideString {
    bool m_empty;
    CCharString ConvertToCharString() const { CCharString s; s.m_pData = m_empty ? 0 : &g_narrow; return s; }
};
static unsigned long Calc_004014a0(unsigned long seed, const void* data, unsigned long length)
{
    unsigned long crc = seed; const unsigned char* p = (const unsigned char*)data;
    for (unsigned long i = 0; i < length; ++i) { crc ^= p[i]; for (int k = 0; k < 8; ++k) crc = (crc & 1) ? (crc >> 1) ^ 0xEDB88320UL : crc >> 1; }
    return crc;
}
inline unsigned long GetNameCRC(const CCharString& name) { return Calc_004014a0(0, name.GetData(), name.GetLength()); }
struct CBankFileAsync {
    unsigned long m_lastCRC;
    unsigned long LookupValue(unsigned long nameCRC) { m_lastCRC = nameCRC; return nameCRC == 0 ? 99 : 7; }
};
enum FileAccessType { FILE_ACCESS_READ };
struct CWADFile {
    void* m_vtable; CBankFileAsync* Bank; unsigned long Index; long FilePos;
    void Open(const CWideString& name, FileAccessType, unsigned long)
    {
        Index = Bank->LookupValue(GetNameCRC(name.ConvertToCharString()));
        FilePos = 0;
    }
};
int main()
{
    g_narrow.m_pText = "abc"; g_narrow.m_length = 3;
    CBankFileAsync bank; CWADFile f; f.Bank = &bank; f.FilePos = 123; f.Index = 0;
    CWideString name; name.m_empty = false;
    f.Open(name, FILE_ACCESS_READ, 0);
    if (f.Index != 7 || f.FilePos != 0 || bank.m_lastCRC != Calc_004014a0(0, "abc", 3) || bank.m_lastCRC == 0 || g_live != 0) { std::printf("00a39cc0_TEST FAIL\n"); return 1; }
    name.m_empty = true; f.FilePos = 5;
    f.Open(name, FILE_ACCESS_READ, 0);
    if (f.Index != 99 || f.FilePos != 0 || bank.m_lastCRC != 0 || g_live != 0) { std::printf("00a39cc0_TEST FAIL\n"); return 1; }
    std::printf("00a39cc0_TEST PASS\n");
    return 0;
}
