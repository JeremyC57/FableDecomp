// CEngineMap::OpenDevelopmentStaticMap(CStaticMapBankFile& bank, long mapID) -- open a development static map from the bank: close the current map, find the map's entry by name, read its header block into a memory stream and parse the map info block; when the info's dimensions match this map's, take the map extents, and for map 1 open the bank entry asynchronously, hand it to the block manager and open the landscape (and the detail cache, when present) from their stream offsets; finally record the map ID
// Leaf: OpenDevelopmentStaticMap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B42530, length 536 bytes (`ret 8`).
// Callees: CEngineMap::CloseStaticMap @ 0x00B3EF40 (landed), CBankFile::FindEntry(const CCharString&) @ 0x009CCDC0,
//   CDataInputStream::CDataInputStream @ 0x00993B20, vector<unsigned char>::_M_fill_insert @ 0x004110D0, the bank entry's range copy
//   @ 0x009CBE90, CMemoryDataInputStream::OpenMemorySource @ 0x00994570, ReadMapInfoBlock (__fastcall) @ 0x00B3EFA0,
//   CBankFileAsync::CreateEntry @ 0x009D58D0, CCountedPointer<CBankFileAsyncEntry> operator= @ 0x00A5F6D0 / copy ctor @ 0x009D69A0 /
//   dtor @ 0x009C9FC0, CEngineStaticMapFileBlockManager::SetBankFileEntry @ 0x00BE03A0, CDataInputStream::SetPosition @ 0x00993BC0,
//   CEngineLandscapeMap::OpenStaticMap @ 0x00BDD0E0, CLocalDetailCacheMap::OpenStaticMap @ 0x00BDF010, ~CMemoryDataInputStream
//   @ 0x00993B10, free @ 0x00BFEA14. Vtable 0x0123675C CMemoryDataInputStream.
// Layout: CEngineMap modelled locally as the landed CloseStaticMap 0x00B3EF40 (scene +0, detail cache +4, name +0x1c, map ID +0x20,
//   extents +0x24, the async entry counted pointer +0x3c, the block manager +0x44, the expected dimensions as ushorts at +0x5c).
// Shape: the header buffer is a STLport vector<unsigned char> resized inline (shrink = finish store, grow = _M_fill_insert with the
//   zero byte by reference) and read through the landed GetData idiom; the memory stream's inline constructor stores its vtable
//   before the embedded zeroed member (landed 0x00485400 model).
extern "C" void __cdecl free(void* block);              // 0x00BFEA14

class CCharString { public: void* PStringData; };

struct CByteVector {                               // STLport vector<unsigned char>, 12 bytes
    unsigned char* _M_start;
    unsigned char* _M_finish;
    unsigned char* _M_end_of_storage;
    CByteVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CByteVector() { if (_M_start) free(_M_start); }
    unsigned long size() const { return _M_finish - _M_start; }
    void _M_fill_insert(unsigned char* position, unsigned long count, const unsigned char& value);   // 0x004110D0
    void resize(unsigned long newSize, const unsigned char& value)
    {
        if (newSize < size())
            _M_finish = _M_start + newSize;
        else
            _M_fill_insert(_M_finish, newSize - size(), value);
    }
    unsigned char* GetData() { return (_M_finish - _M_start) ? _M_start : (unsigned char*)this; }
};

class CDataInputStream { public:
    virtual void v00();
    unsigned char _pad04[0x14];
    CDataInputStream();                            // 0x00993B20
    void SetPosition(unsigned long position);      // 0x00993BC0
};
struct SZeroed {
    long m_00;
    SZeroed() : m_00(0) { }
};
class CMemoryDataInputStream : public CDataInputStream { public: // vtable 0x0123675C, 0x1c bytes
    SZeroed m_source;                              // +0x18
    virtual void v00();
    CMemoryDataInputStream() { }
    virtual ~CMemoryDataInputStream();                     // 0x00993B10
    void OpenMemorySource(void* data, unsigned long size);   // 0x00994570
};

struct CBankEntryRecord {
    unsigned long Offset;
    unsigned long Size;                            // +4
    void CopyRange(unsigned long offset, void* destination, unsigned long size);   // 0x009CBE90
};
class CBankFileAsyncEntry;
struct CAsyncEntryPointer {                        // CCountedPointer<CBankFileAsyncEntry>
    CBankFileAsyncEntry* Object;
    void* Info;
    CAsyncEntryPointer(const CAsyncEntryPointer& other);   // 0x009D69A0
    ~CAsyncEntryPointer();                         // 0x009C9FC0
    CAsyncEntryPointer& operator=(const CAsyncEntryPointer& other);   // 0x00A5F6D0
};
class CStaticMapBankFile { public:
    unsigned char      _pad00[0x2c];
    CBankEntryRecord** Entries;                    // +0x2c
    unsigned long FindEntry(const CCharString& name);   // 0x009CCDC0
    CAsyncEntryPointer CreateEntry(unsigned long index);   // 0x009D58D0
};

struct CMapExtents { long V[6]; };
struct CStaticMapInfoBlock {                       // 0x5c bytes
    unsigned char _pad00[0x10];
    unsigned long LandscapeOffset;                 // +0x10
    unsigned long DetailCacheOffset;               // +0x14
    unsigned char _pad18[0x8];
    long          Width;                           // +0x20
    long          Height;                          // +0x24
    long          Depth;                           // +0x28
    long          Count;                           // +0x2c
    unsigned char _pad30[0x10];
    CMapExtents   Extents;                         // +0x40
    unsigned long EndOffset;                       // +0x58
};
void __fastcall ReadMapInfoBlock(CStaticMapInfoBlock* info, CDataInputStream* stream);   // 0x00B3EFA0

class CEngineStaticMapFileBlockManager { public:
    unsigned char _body[0x18];
    void SetBankFileEntry(CAsyncEntryPointer entry);   // 0x00BE03A0
};
class CEngineLandscapeMap { public:
    void OpenStaticMap(CEngineStaticMapFileBlockManager* blocks, CDataInputStream* stream);   // 0x00BDD0E0
};
class CLocalDetailCacheMap { public:
    void OpenStaticMap(CEngineStaticMapFileBlockManager* blocks, CDataInputStream* stream);   // 0x00BDF010
};

class CEngineMap { public:
    CEngineLandscapeMap*  m_pLandscape;            // +0x00
    CLocalDetailCacheMap* m_pDetailCache;          // +0x04
    unsigned char         _pad08[0x14];
    CCharString           m_mapName;               // +0x1c
    long                  m_staticMapID;           // +0x20
    CMapExtents           m_extents;               // +0x24
    CAsyncEntryPointer    m_bankEntry;             // +0x3c
    CEngineStaticMapFileBlockManager m_blocks;     // +0x44
    unsigned short        m_width;                 // +0x5c
    unsigned short        m_height;                // +0x5e
    unsigned short        m_depth;                 // +0x60
    unsigned short        m_count;                 // +0x62
    void CloseStaticMap();                         // 0x00B3EF40
    void OpenDevelopmentStaticMap(CStaticMapBankFile& bank, long mapID);
};

void CEngineMap::OpenDevelopmentStaticMap(CStaticMapBankFile& bank, long mapID)
{
    if (m_staticMapID == mapID)
        return;
    CloseStaticMap();
    unsigned long index = bank.FindEntry(m_mapName);
    if (!index)
        return;
    CByteVector buffer;
    CMemoryDataInputStream stream;
    CBankEntryRecord* record = bank.Entries[index];
    unsigned long size = record->Size;
    buffer.resize(size, 0);
    record->CopyRange(0, buffer.GetData(), size);
    stream.OpenMemorySource(buffer.GetData(), size);
    CStaticMapInfoBlock info;
    ReadMapInfoBlock(&info, &stream);
    if (info.Width == m_width && info.Height == m_height && info.Depth == m_depth && info.Count == m_count) {
        m_extents = info.Extents;
        if (mapID == 1) {
            m_bankEntry = bank.CreateEntry(index);
            m_blocks.SetBankFileEntry(m_bankEntry);
            stream.SetPosition(info.LandscapeOffset);
            m_pLandscape->OpenStaticMap(&m_blocks, &stream);
            if (m_pDetailCache) {
                stream.SetPosition(info.DetailCacheOffset);
                m_pDetailCache->OpenStaticMap(&m_blocks, &stream);
            }
        }
        stream.SetPosition(info.EndOffset);
        m_staticMapID = mapID;
    }
}