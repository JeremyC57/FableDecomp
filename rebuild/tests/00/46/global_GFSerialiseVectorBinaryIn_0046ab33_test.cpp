// Behaviour test for 0x0046AB33 GFSerialiseVectorBinaryIn<CSoundBankEntry>: count, resize(n), one traits read per element
#include <cstdio>
#include <cstring>
struct CSoundBankEntry {
    unsigned int ID; int Filename; int SymbolFilename; int LipSyncBankHandle; bool DVD;
    CSoundBankEntry() : ID(0), Filename(-1), SymbolFilename(-1), LipSyncBankHandle(-1) {}
};
struct EntryVector {
    CSoundBankEntry data[8]; unsigned int count;
    void resize(unsigned int n) { for (unsigned int i = count; i < n; ++i) data[i] = CSoundBankEntry(); count = n; }
    CSoundBankEntry& operator[](unsigned int i) { return data[i]; }
};
struct Stream {
    unsigned int position; const unsigned char* chunkBase; int chunkLength; int overflowCalls;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, int size) { ++overflowCalls; std::memset(buffer, 0, size); }
    void Read(void* buffer, int size)
    {
        long long newPosition = (long long)(unsigned int)(position + size);
        if (!(newPosition >= 0 && newPosition <= 0x7fffffff)) return;
        if (chunkLength >= size) { std::memcpy(buffer, chunkBase, size); chunkBase += size; chunkLength -= size; position += size; }
        else ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};
struct Traits {
    CSoundBankEntry DefaultVal;
    void TransferBinaryIn(Stream& s, CSoundBankEntry* out) const { s.Read(&out->ID, 4); s.Read(&out->Filename, 4); }
};
static void SerialiseIn(Stream& stream, EntryVector& out)
{
    Traits traits;
    int count;
    stream.Read(&count, 4);
    out.resize(count);
    for (int i = 0; i < count; ++i)
        traits.TransferBinaryIn(stream, &out[i]);
}
int main()
{
    unsigned char data[4 + 2 * 8] = { 2, 0, 0, 0, 11, 0, 0, 0, 12, 0, 0, 0, 13, 0, 0, 0, 14, 0, 0, 0 };
    Stream s = { 0, data, (int)sizeof(data), 0 };
    EntryVector v; v.count = 0;
    SerialiseIn(s, v);
    if (v.count != 2 || v[0].ID != 11 || v[0].Filename != 12 || v[1].ID != 13 || v[1].Filename != 14 || v[1].SymbolFilename != -1 || s.position != 20) {
        std::printf("0046ab33_TEST FAIL\n"); return 1;
    }
    std::printf("0046ab33_TEST PASS\n");
    return 0;
}