// behaviour test for 009d4ab0 CDefStringTable::ReadTable (host g++/VC)
#include <stdio.h>
#include <string.h>
struct Stream {
    unsigned long StreamPos; long StreamSize; void* CurrentSourcePtr; unsigned long SourceChunkStreamPos; long SourceChunkBytesRemaining;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size) { memset(buffer, 0xee, size); }
    void Read(void* buffer, long size) {
        if (size <= 0) return;
        long long np = (long long)(StreamPos + size);
        if (!(np >= 0 && np <= 0x7fffffff)) return;
        if (size <= SourceChunkBytesRemaining) { memcpy(buffer, CurrentSourcePtr, size); CurrentSourcePtr = (unsigned char*)CurrentSourcePtr + size; SourceChunkBytesRemaining -= size; StreamPos += size; }
        else ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    long ReadLong() { long v = 0; Read(&v, 4); return v; }
};
static unsigned char g_blob[64];
struct CMemoryDataOutputStream {
    long size; long readSize;
    void SetSize(long n) { size = n; }
    void ReadFromInputStream(Stream& in, long n) { readSize = n; in.Read(g_blob, n); }
    void* PeekData() { return g_blob; }
};
void* g_pStringTableData = 0;
struct CDefStringTable {
    unsigned char StringMap[0xc]; CMemoryDataOutputStream StringStream; unsigned long StringCount; unsigned long SharedCount; unsigned long RandomID;
    void ReadTable(Stream& in) {
        in.ReadLong();
        RandomID = in.ReadLong();
        StringCount = in.ReadLong();
        long size = in.ReadLong();
        CMemoryDataOutputStream* data = &StringStream;
        data->SetSize(size);
        data->ReadFromInputStream(in, size);
        g_pStringTableData = (unsigned char*)data->PeekData() + 4;
    }
};
int main() {
    unsigned char data[24] = { 0x1e, 0xab, 7, 0, 0x34, 0x12, 0, 0, 3, 0, 0, 0, 8, 0, 0, 0, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 0 };
    Stream s = { 0, 100, data, 0, 24 };
    CDefStringTable t; memset(&t, 0, sizeof t);
    t.ReadTable(s);
    bool ok = t.RandomID == 0x1234 && t.StringCount == 3 && t.StringStream.size == 8 && t.StringStream.readSize == 8
           && memcmp(g_blob, "abcdefg", 8) == 0 && g_pStringTableData == g_blob + 4 && s.StreamPos == 24;
    printf(ok ? "009d4ab0_TEST PASS\n" : "009d4ab0_TEST FAIL\n");
    return ok ? 0 : 1;
}
