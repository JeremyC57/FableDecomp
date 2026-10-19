// Behaviour test for CPersistContext::CheckCRC (0x00404500): binary load skips the 4-byte key CRC, binary save writes the CRC of the key.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
typedef long long i64;
#else
typedef __int64 i64;
#endif
struct CCharStringData { const char* Text; unsigned long Length; };
static CCharStringData g_block;
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long = -1) { g_block.Text = text; g_block.Length = (unsigned long)std::strlen(text); PStringData = text[0] ? &g_block : 0; }
    ~CCharString() {}
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->Length; }
    const char* GetPString() const { return PStringData == 0 ? "" : PStringData->Text; }
};
unsigned long __fastcall Calc(unsigned long seed, const void* data, unsigned long length)
{ unsigned long h = seed; const unsigned char* p = (const unsigned char*)data; for (unsigned long i = 0; i < length; ++i) h = h * 31 + p[i]; return h + 1000 * length; }
inline unsigned long GetCRC(const CCharString& text) { return Calc(0, text.GetPString(), text.GetLength()); }
struct CMemoryDataInputStream {
    void* m_pVtable; unsigned long m_position; long m_08; const unsigned char* m_pData; long m_10; long m_remaining; int overflows;
    void ReadWithSrcChunkOverflow(unsigned char*, long) { ++overflows; }
    static bool IsValidPosition(i64 position) { return position >= 0 && position <= 0x7fffffff; }
    void Read(void* buffer, long size)
    {
        i64 newPosition = (i64)(m_position + size);
        if (!IsValidPosition(newPosition)) return;
        if (m_remaining >= size) { std::memcpy(buffer, m_pData, size); m_pData += size; m_remaining -= size; m_position += size; }
        else ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    unsigned long ReadULong() { unsigned long value = 0; Read(&value, 4); return value; }
};
struct CDataOutputStream { int writes; unsigned long last; void WriteULong(unsigned long v) { ++writes; last = v; } };
struct CPersistContext {
    long Mode; CMemoryDataInputStream* LoadStream; CDataOutputStream* SaveStream;
    void CheckCRC(const char* name)
    {
        if (Mode == 2) LoadStream->ReadULong();
        else if (Mode == 3) SaveStream->WriteULong(GetCRC(CCharString(name)));
    }
};
int main()
{
    unsigned char buf[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
    CMemoryDataInputStream in; std::memset(&in, 0, sizeof(in)); in.m_pData = buf; in.m_remaining = 6;
    CDataOutputStream out = { 0, 0 };
    CPersistContext c; c.LoadStream = &in; c.SaveStream = &out;
    c.Mode = 2; c.CheckCRC("Key");
    if (in.m_position != 4 || in.m_remaining != 2 || in.m_pData != buf + 4 || in.overflows != 0) { std::printf("00404500_TEST FAIL\n"); return 1; }
    c.CheckCRC("Key");   /* 2 bytes left: overflow read */
    if (in.overflows != 1 || in.m_position != 4) { std::printf("00404500_TEST FAIL\n"); return 1; }
    in.m_position = 0x7ffffffe; in.m_remaining = 100; c.CheckCRC("Key");   /* would pass 0x7FFFFFFF: rejected */
    if (in.overflows != 1 || in.m_remaining != 100) { std::printf("00404500_TEST FAIL\n"); return 1; }
    c.Mode = 3; c.CheckCRC("ab");
    if (out.writes != 1 || out.last != Calc(0, "ab", 2)) { std::printf("00404500_TEST FAIL\n"); return 1; }
    c.CheckCRC("");
    if (out.writes != 2 || out.last != 0) { std::printf("00404500_TEST FAIL\n"); return 1; }
    c.Mode = 0; c.CheckCRC("x"); c.Mode = 1; c.CheckCRC("x"); c.Mode = 4; c.CheckCRC("x");
    if (out.writes != 2 || in.overflows != 1) { std::printf("00404500_TEST FAIL\n"); return 1; }
    std::printf("00404500_TEST PASS\n");
    return 0;
}
