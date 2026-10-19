// Behaviour test for CDefStringTable::AddString (0x009D4F90): empty -> -1; a new string is appended after a 4-byte
// zero header as CRC word + NUL-terminated text and mapped to its offset; a repeated string bumps SharedCount and
// returns the stored offset; the global data pointer skips the 4-byte header.
#include <cstdio>
#include <cstring>
struct CCharStringData { char* Text; long Length; };
static const char g_emptyString[] = "";
struct CCharString {
    CCharStringData* PStringData;
    long GetLength() const { return PStringData == 0 ? 0 : PStringData->Length; }
    const char* GetPString() const { return PStringData == 0 ? g_emptyString : PStringData->Text; }
};
static unsigned long Calc(unsigned long seed, const void* data, unsigned long length)
{
    const unsigned char* p = (const unsigned char*)data; unsigned long h = seed;
    for (unsigned long i = 0; i < length; ++i) h = h * 31 + p[i];
    return h;
}
struct CMemoryDataOutputStream {
    unsigned char buf[256]; long size;
    void Write(const void* data, long length) { std::memcpy(buf + size, data, length); size += length; }
    long GetSize() { return size; }
    void WriteRepeatedByte(unsigned char value, long count) { while (count--) Write(&value, 1); }
    void WriteNullTerminatedString(const CCharString& t) { Write(t.GetPString(), t.GetLength() + 1); }
    void* PeekData() { return buf; }
    void WriteULong(unsigned long value) { unsigned int v = (unsigned int)value; Write(&v, 4); }
};
struct SStringNode { const char* key; long Offset; };
struct CStringMap {
    SStringNode nodes[8]; int count; SStringNode* Header;
    SStringNode* find(const CCharString& k)
    {
        for (int i = 0; i < count; ++i) if (!std::strcmp(nodes[i].key, k.GetPString())) return &nodes[i];
        return Header;
    }
    long& operator[](const CCharString& k) { SStringNode* n = find(k); if (n == Header) { n = &nodes[count++]; n->key = k.GetPString(); } return n->Offset; }
};
static unsigned char* g_pDefStringTableData = 0;
static int g_unlocks = 0;
class CDefStringTable {
public:
    CStringMap StringMap; CMemoryDataOutputStream StringStream; unsigned long StringCount; unsigned long SharedCount; bool Locked;
    void Unlock() { ++g_unlocks; Locked = false; }
    long AddString(const CCharString& text);
};
long CDefStringTable::AddString(const CCharString& text)
{
    if (text.GetLength() == 0)
        return -1;
    if (Locked)
        Unlock();
    SStringNode* node = StringMap.find(text);
    if (node != StringMap.Header) {
        ++SharedCount;
        return node->Offset;
    }
    if (StringStream.GetSize() == 0)
        StringStream.WriteRepeatedByte(0, 4);
    long offset = StringStream.GetSize();
    StringStream.WriteULong(Calc(0, text.GetPString(), text.GetLength()));
    StringStream.WriteNullTerminatedString(text);
    StringMap[text] = offset;
    ++StringCount;
    g_pDefStringTableData = (unsigned char*)StringStream.PeekData() + 4;
    return offset;
}
int main()
{
    SStringNode header;
    CDefStringTable t; t.StringMap.count = 0; t.StringMap.Header = &header; t.StringStream.size = 0;
    t.StringCount = 0; t.SharedCount = 0; t.Locked = true;
    char a[] = "SWORD", b[] = "AXE";
    CCharStringData da = { a, 5 }, db = { b, 3 }, de = { a, 0 };
    CCharString sa = { &da }, sb = { &db }, empty = { &de }, null = { 0 };
    int bad = 0;
    if (t.AddString(null) != -1 || t.AddString(empty) != -1 || g_unlocks) bad = 1;
    long oa = t.AddString(sa);
    if (oa != 4 || g_unlocks != 1 || t.StringCount != 1 || t.StringStream.size != 4 + 4 + 6) bad = 2;
    unsigned int crc; std::memcpy(&crc, t.StringStream.buf + 4, 4);
    if (crc != (unsigned int)Calc(0, "SWORD", 5) || std::strcmp((char*)t.StringStream.buf + 8, "SWORD")) bad = 3;
    if (g_pDefStringTableData != t.StringStream.buf + 4) bad = 4;
    long ob = t.AddString(sb);
    if (ob != 14 || t.StringCount != 2) bad = 5;
    if (t.AddString(sa) != 4 || t.SharedCount != 1 || t.StringCount != 2) bad = 6;
    if (bad) { std::printf("009d4f90_TEST FAIL\n"); return 1; }
    std::printf("009d4f90_TEST PASS\n");
    return 0;
}