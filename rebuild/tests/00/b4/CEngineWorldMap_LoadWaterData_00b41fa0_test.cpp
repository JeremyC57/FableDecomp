#include <cstdio>
#include <cstdlib>
#include <cstring>
// behaviour test: CEngineWorldMap::LoadWaterData reads the header record and the entry data into two streams for the water renderer
static int g_destroyed = 0, g_loaded = 0; static unsigned long g_headerSize = 0, g_dataSize = 0;
struct CByteBuffer {
    unsigned char* First; unsigned char* Last;
    CByteBuffer(unsigned long size) { First = (unsigned char*)std::malloc(size ? size : 1); Last = First + size; }
    ~CByteBuffer() { std::free(First); }
    unsigned char* GetData() { return First; }
};
struct CBankEntryRecord { unsigned long Offset, Size; void CopyRange(unsigned long, void* dst, unsigned long size) { std::memset(dst, 0x11, size); } };
struct CMemoryDataInputStream { const unsigned char* data; unsigned long size; CMemoryDataInputStream(void* d, unsigned long s) : data((unsigned char*)d), size(s) {} };
struct SCountBlock { long m_references; };
struct CBankFile { unsigned long GetEntryDataSize(unsigned long index) { return index * 10; } };
struct CBankFileAsyncEntry {
    CBankFile* Bank; unsigned long Index;
    void Read(long, void* dst, unsigned long size) { std::memset(dst, 0x22, size); }
    unsigned long GetDataSize() { return Bank->GetEntryDataSize(Index); }
};
struct CEntryPointer {
    CBankFileAsyncEntry* Object; SCountBlock* Info;
    CEntryPointer() : Object(0), Info(0) {}
    CEntryPointer(const CEntryPointer& o) : Object(o.Object), Info(o.Info) { if (Info) ++Info->m_references; }
    ~CEntryPointer() { if (Info) { --Info->m_references; if (Info->m_references == 0) { ++g_destroyed; delete Info; } } Object = 0; Info = 0; }
};
static CBankFile g_bank; static CBankFileAsyncEntry g_entry = { &g_bank, 0 };
struct CStaticMapBankFile {
    unsigned long found;
    unsigned long FindEntry() { return found; }
    CEntryPointer CreateEntry(unsigned long index) { g_entry.Index = index; CEntryPointer p; p.Object = &g_entry; p.Info = new SCountBlock; p.Info->m_references = 1; return p; }
};
struct CEngineWaterRenderer {
    void LoadNonPatchData(CMemoryDataInputStream& data, CMemoryDataInputStream& header)
    { ++g_loaded; g_dataSize = data.size; g_headerSize = header.size; if (data.data[0] != 0x22 || header.data[0] != 0x11) g_loaded = -100; }
};
static CEngineWaterRenderer g_water;
struct CEngineWorldMap {
    CStaticMapBankFile StaticMapFile; CBankEntryRecord** Entries;
    void LoadWaterData();
};
void CEngineWorldMap::LoadWaterData()
{
    unsigned long index = StaticMapFile.FindEntry();
    if (!index) return;
    CBankEntryRecord* record = Entries[index];
    unsigned long size = record->Size;
    CByteBuffer header(size);
    record->CopyRange(0, header.GetData(), size);
    CMemoryDataInputStream headerStream(header.GetData(), size);
    CEntryPointer entry = StaticMapFile.CreateEntry(index);
    unsigned long dataSize = entry.Object->GetDataSize();
    CByteBuffer data(dataSize);
    entry.Object->Read(0, data.GetData(), dataSize);
    CMemoryDataInputStream dataStream(data.GetData(), dataSize);
    g_water.LoadNonPatchData(dataStream, headerStream);
}
int main()
{
    CBankEntryRecord r0 = { 0, 4 }, r3 = { 0, 6 };
    CBankEntryRecord* table[4] = { &r0, 0, 0, &r3 };
    CEngineWorldMap w; w.Entries = table; w.StaticMapFile.found = 0;
    w.LoadWaterData();
    bool ok = g_loaded == 0;
    w.StaticMapFile.found = 3;
    w.LoadWaterData();
    ok = ok && g_loaded == 1 && g_headerSize == 6 && g_dataSize == 30 && g_destroyed == 1;
    if (!ok) { std::printf("00b41fa0_TEST FAIL\n"); return 1; }
    std::printf("00b41fa0_TEST PASS\n");
    return 0;
}