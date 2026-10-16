// CDefStringTable::CDefStringTable() -- empty string map, empty string stream, zero counts, empty table filename, unlocked and writable; RandomID is a CRC32 of the current system time
// Leaf: ??0CDefStringTable@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D4DB0, length 224 bytes, no arguments, returns `this`.
// Label correction: catalogued as CBankFile::GetRandomValidationValue (propagated from the sibling 0x009CC0B0, whose CRC loop
// is inlined here without the retry); the layout is Ego_r.pdb CDefStringTable (StringMap, StringStream, StringCount,
// SharedCount, RandomID, TableFilename, Locked, ReadOnly).
// Callees: malloc @ 0x00BFEA0E (the 0x18-byte map header node), CMemoryDataOutputStream::CMemoryDataOutputStream()
// @ 0x00996AF0, CWideString::CWideString() @ 0x0099AED0, GetSystemTime (import 0x0143FE18). Data: CRC32 table @ 0x0129A168.
// Shape: the STLport map ctor as the landed CLookDrawRequest 0x007554C0 (header nulled, allocated, count 0, empty
// initialise re-reading the header); the stream and filename are out-of-line ctors; RandomID is an inline CRC32 over the
// 16-byte SYSTEMTIME (unrolled four times).
// Layout (retail, PDB minus the wider STL members): StringMap +0x00 (12 bytes), StringStream +0x0c (0x18), StringCount
// +0x24, SharedCount +0x28, RandomID +0x2c, TableFilename +0x30, Locked +0x34, ReadOnly +0x35.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
struct SYSTEMTIME_ {
    unsigned short wYear, wMonth, wDayOfWeek, wDay, wHour, wMinute, wSecond, wMilliseconds;
};
extern "C" __declspec(dllimport) void __stdcall GetSystemTime(SYSTEMTIME_* time);   // import 0x0143FE18
extern const unsigned long g_crc32Table[256];      // 0x0129A168

inline unsigned long GetCRC32(const void* data, unsigned long size)
{
    unsigned long value = 0;
    const unsigned char* bytes = (const unsigned char*)data;
    for (unsigned long i = 0; i < size; ++i)
        value = g_crc32Table[bytes[i] ^ (value & 0xff)] ^ (value >> 8);
    return value;
}

struct SRbTreeNode {                               // 0x18 bytes: pair<const CCharString, long>
    unsigned char m_color;                         // +0x00 (0 = red)
    SRbTreeNode*  m_pParent;                       // +0x04
    SRbTreeNode*  m_pLeft;                         // +0x08
    SRbTreeNode*  m_pRight;                        // +0x0c
    void*         m_key;                           // +0x10 CCharString
    long          m_value;                         // +0x14
};
struct CRbTreeBase {
    SRbTreeNode* m_pHeader;
    CRbTreeBase() : m_pHeader(0)
    {
        m_pHeader = (SRbTreeNode*)malloc(sizeof(SRbTreeNode));
    }
};
struct CRbTree : CRbTreeBase {
    unsigned long m_nodeCount;
    unsigned char m_compare;                       // less<CCharString>
    CRbTree() : m_nodeCount(0)
    {
        m_pHeader->m_color = 0;
        m_pHeader->m_pParent = 0;
        m_pHeader->m_pLeft = m_pHeader;
        m_pHeader->m_pRight = m_pHeader;
    }
};
struct CMemoryDataOutputStream {
    unsigned char _body[0x18];
    CMemoryDataOutputStream();                     // 0x00996AF0
};
struct CWideString {
    void* PStringData;
    CWideString();                                 // 0x0099AED0
};

class CDefStringTable {
public:
    CRbTree StringMap;                             // +0x00 map<CCharString, long>
    CMemoryDataOutputStream StringStream;          // +0x0c
    unsigned long StringCount;                     // +0x24
    unsigned long SharedCount;                     // +0x28
    unsigned long RandomID;                        // +0x2c
    CWideString TableFilename;                     // +0x30
    bool Locked;                                   // +0x34
    bool ReadOnly;                                 // +0x35
    CDefStringTable();
};

CDefStringTable::CDefStringTable()
    : StringCount(0), SharedCount(0), Locked(false), ReadOnly(false)
{
    SYSTEMTIME_ time;
    GetSystemTime(&time);
    RandomID = GetCRC32(&time, sizeof(time));
}
