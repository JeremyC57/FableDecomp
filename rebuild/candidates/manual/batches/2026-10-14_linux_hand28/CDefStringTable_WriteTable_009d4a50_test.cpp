#include <cstdio>
static unsigned long g_longs[8]; static int g_n = 0; static const void* g_data = 0; static unsigned long g_dataSize = 0;
struct CDataOutputStream {
    virtual void v0() { } virtual void v1() { } virtual void v2() { } virtual void v3() { }
    virtual void WriteData(const void* d, unsigned long s) { g_data = d; g_dataSize = s; }
    void WriteULong(unsigned long v) { g_longs[g_n++] = v; }
};
static char g_blob[] = "blob";
struct CMemoryDataOutputStream { unsigned long GetSize() { return 4; } void* PeekData() { return g_blob; } };
struct CDefStringTable {
    unsigned char StringMap[0xc]; CMemoryDataOutputStream StringStream; unsigned char _stream[0x18 - sizeof(CMemoryDataOutputStream)];
    unsigned long StringCount, SharedCount, RandomID;
    void WriteTable(CDataOutputStream& s) {
        s.WriteULong(0x7ab1e); s.WriteULong(RandomID); s.WriteULong(StringCount);
        CMemoryDataOutputStream* data = &StringStream; unsigned long size = data->GetSize(); s.WriteULong(size); s.WriteData(data->PeekData(), size); }
};
int main()
{
    CDefStringTable t; t.StringCount = 7; t.SharedCount = 1; t.RandomID = 0xabcd; CDataOutputStream s;
    t.WriteTable(s);
    if (g_n != 4 || g_longs[0] != 0x7ab1e || g_longs[1] != 0xabcd || g_longs[2] != 7 || g_longs[3] != 4 || g_data != g_blob || g_dataSize != 4
        || (char*)&t.StringCount - (char*)&t != 0x24 || (char*)&t.RandomID - (char*)&t != 0x2c) { std::printf("009d4a50_TEST FAIL\n"); return 1; }
    std::printf("009d4a50_TEST PASS\n"); return 0;
}
