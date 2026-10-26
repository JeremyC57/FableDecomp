// CEngineWorldMap::LoadWaterData() -- when the static-map bank holds the water entry: copy the entry's header record into a buffer, open the entry asynchronously and read its whole data block into a second buffer, wrap both in memory streams and hand them (data, header) to the water renderer's non-patch data loader
// Leaf: LoadWaterData
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B41FA0, length 336 bytes (body 0x14b), plain `ret`.
// Callees: CBankFile::FindEntry(const CCharString&) @ 0x009CCDC0, the byte buffer constructor @ 0x00411910 and resize
//   @ 0x00514E20, the bank entry's range copy @ 0x009CBE90, CMemoryDataInputStream(void*, ulong) @ 0x00994930 /
//   ~CMemoryDataInputStream @ 0x00993B10, CBankFileAsync::CreateEntry(ulong) @ 0x009D58D0, CBankFile::GetEntryDataSize
//   @ 0x009CC240, the async entry's synchronous read @ 0x009D52B0, CEngineWaterRenderer::LoadNonPatchData @ 0x00B6DAF0
//   (global @ 0x01436E54), free @ 0x00BFEA14, operator delete @ 0x00BFE9BC; the entry symbol global @ 0x01436EC8.
// Header: engine/CEngineWorldMap.h flattens MapList/VisibleMapList/StaticMapFile to byte arrays and carries the PDB offsets
//   (StaticMapFile +0x40, GameWorld +0x218) where retail has +0x34/+0x198, so the class is modelled locally.
// Layout: CEngineWorldMap: StaticMapFile +0x34 (its entry table at +0x60 of the map).
// PDB: ?LoadWaterData@CEngineWorldMap@@AAEXXZ (catalogued "LoadMapThemeData", propagated; the PDB's only no-argument
//   loader that feeds CEngineWaterRenderer::LoadNonPatchData(CMemoryDataInputStream&, CMemoryDataInputStream&)).
// Shape: the async entry is a counted pointer whose dtor clears both words after the release; the data size is read through
//   the entry (bank +4, index +8); CMemoryDataInputStream is 0x1c bytes in retail (two of them fix the 0x58 frame).
extern "C" void __cdecl free(void* block);
class CCharString { public: void* PStringData; };
extern CCharString g_GlobalMapEntrySymbol_01436ec8;   // 0x01436EC8
struct CByteBuffer {
    unsigned char* First; unsigned char* Last; unsigned char* End;
    CByteBuffer(unsigned long size);               // 0x00411910
    ~CByteBuffer() { if (First) free(First); }
    unsigned char* GetData() { return (Last - First) ? First : (unsigned char*)this; }
};
struct CBankEntryRecord {
    unsigned long Offset;
    unsigned long Size;                            // +4
    void CopyRange(unsigned long offset, void* destination, unsigned long size);   // 0x009CBE90
};
struct CDataBuffer {
    unsigned char* First; unsigned char* Last; unsigned char* End;
    CDataBuffer(unsigned long size);               // 0x00514E20
    ~CDataBuffer() { if (First) free(First); }
    unsigned char* GetData() { return (Last - First) ? First : (unsigned char*)this; }
};
class CMemoryDataInputStream { public:                    // 0x1c bytes (CDataInputStream + memory source)
    unsigned char _body[0x1c];
    CMemoryDataInputStream(void* data, unsigned long size);   // 0x00994930
    ~CMemoryDataInputStream();                     // 0x00993B10
};
struct SCountBlock { long m_references; void (__fastcall* m_pfnDestroy)(void*); void* m_pObject; };
class CBankFile { public:
    unsigned long GetEntryDataSize(unsigned long index);   // 0x009CC240
};
class CBankFileAsyncEntry { public:
    void*         _vptr;
    CBankFile*    Bank;                            // +0x04
    unsigned long Index;                           // +0x08
    void Read(long offset, void* destination, unsigned long size);   // 0x009D52B0
    unsigned long GetDataSize() { return Bank->GetEntryDataSize(Index); }
};
struct CEntryPointer {
    CBankFileAsyncEntry* Object;
    SCountBlock* Info;
    ~CEntryPointer()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                delete Info;
            }
        }
        Object = 0;
        Info = 0;
    }
};
class CStaticMapBankFile { public:
    unsigned long FindEntry(const CCharString& name);   // 0x009CCDC0
    CEntryPointer CreateEntry(unsigned long index);     // 0x009D58D0
};
class CEngineWaterRenderer { public:
    void LoadNonPatchData(CMemoryDataInputStream& a, CMemoryDataInputStream& b);   // 0x00B6DAF0
};
extern CEngineWaterRenderer* g_pEngineWaterRenderer;   // 0x01436E54
class CEngineWorldMap { public:
    unsigned char      _pad00[0x34];
    CStaticMapBankFile StaticMapFile;              // +0x34
    unsigned char      _pad35[0x60 - 0x35];
    CBankEntryRecord** Entries;                    // +0x60
    void LoadWaterData();
};
void CEngineWorldMap::LoadWaterData()
{
    unsigned long index = StaticMapFile.FindEntry(g_GlobalMapEntrySymbol_01436ec8);
    if (!index)
        return;
    CBankEntryRecord* record = Entries[index];
    unsigned long size = record->Size;
    CByteBuffer header(size);
    record->CopyRange(0, header.GetData(), size);
    CMemoryDataInputStream headerStream(header.GetData(), size);
    CEntryPointer entry = StaticMapFile.CreateEntry(index);
    unsigned long dataSize = entry.Object->GetDataSize();
    CDataBuffer data(dataSize);
    entry.Object->Read(0, data.GetData(), dataSize);
    CMemoryDataInputStream dataStream(data.GetData(), dataSize);
    g_pEngineWaterRenderer->LoadNonPatchData(dataStream, headerStream);
}