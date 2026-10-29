// CRegion::LoadBinary(CMemoryDataInputStream* stream) -- read the map and visible-map counts, then each map id and visible-map id; read the name, display name and region def name (looking the region def up by that name), the mini-map graphic, the appear-on-world-map / creature-generation / sound-theme flags, the mini-map scale and offset, the world-map offset, and finally the mini-map region-exit text offsets keyed by exit name
// Leaf: LoadBinary
// Retail: Fable.exe (TLC, VC7.1) @ 0x006BC510, 1480 bytes, `ret 4`.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0, ReadPresizedString 0x00996390, C2DVector read
//   0x00995030, vector<long>::resize 0x004428F0, vector<ENavigatorType> fill insert 0x00440F00, memmove 0x00BFEAE6,
//   CCharString op= 0x0099EFB0 / dtor 0x0099EAE0, CGameDefinitionManager::Get 0x0044C6B0,
//   CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410 / GetDef<CRegionDef> 0x006BD060, map operator[] 0x006BD6A0.
// Layout: Ego_r.pdb CRegion with 12-byte retail vectors/maps: Maps +0x00, VisMaps +0x0c, Name +0x18, DisplayName +0x1c,
//   RegionDefName +0x20, PDef +0x24, MiniMapGraphic +0x28, MiniMapScale +0x2c, MiniMapOffset +0x30,
//   MiniMapRegionExitTextOffsets +0x38, WorldMapOffset +0x44, NameGraphicOffset +0x4c, flags +0x54..+0x56.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)
class CCharString { public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
class C2DVector { public: float X, Y; };
class CDataInputStream { friend class CMemoryDataInputStream;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0 (private: Ego_r ?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z)
};
class CMemoryDataInputStream : public CDataInputStream { public:
    void* m_pVtable;                       // +0
    unsigned long m_position;              // +4
    long StreamSize;
    const unsigned char* m_pData;          // +0xC
    long SourceChunkStreamPos;
    long m_remaining;                      // +0x14
    CCharString ReadPresizedString();                                  // 0x00996390
    void Read(C2DVector* vector);                                      // 0x00995030
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(m_position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (size <= m_remaining) {
            memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    __forceinline long ReadLong()
    {
        long value;
        Read(&value, sizeof(value));
        return value;
    }
    __forceinline float ReadFloat()
    {
        float value;
        Read(&value, sizeof(value));
        return value;
    }
    __forceinline bool ReadBool()
    {
        unsigned char value;
        Read(&value, sizeof(value));
        return value != 0;
    }
};
class CRegionDef;
template <class T> class CDefPointer { public: const T* Def; };
class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name);           // 0x009AD410
    template <class T> bool GetDef(long index, CDefPointer<T>* pointer);   // 0x006BD060
};
class CGameDefinitionManager : public CDefinitionManager { public: static CGameDefinitionManager& __fastcall Get(); };   // 0x0044C6B0
enum ENavigatorType { NAVIGATOR_TYPE_NONE };
struct CLongVector {
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    void resize(unsigned int count, const long& value);   // 0x004428F0
    void resize(unsigned int count) { resize(count, 0); }
};
struct CNavigatorTypeVector {
    ENavigatorType* _M_start;
    ENavigatorType* _M_finish;
    ENavigatorType* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    void _M_fill_insert(ENavigatorType* position, unsigned int count, const ENavigatorType& value);   // 0x00440F00
    static void* __copy_trivial(const void* first, const void* last, void* result)
    {
        return (last == first) ? result
            : ((char*)memmove(result, first, (const char*)last - (const char*)first)) + ((const char*)last - (const char*)first);
    }
    void erase(ENavigatorType* first, ENavigatorType* last) { _M_finish = (ENavigatorType*)__copy_trivial(last, _M_finish, first); }
    void resize(unsigned int count, const ENavigatorType& value)
    {
        if (count < size())
            erase(_M_start + count, _M_finish);
        else
            _M_fill_insert(_M_finish, count - size(), value);
    }
    void resize(unsigned int count) { resize(count, ENavigatorType()); }
};
struct CExitTextOffsetMap {
    void* _M_header;
    long  _M_node_count;
    long  _M_compare;
    C2DVector& operator[](const CCharString& key);   // 0x006BD6A0
};
class CRegion { public:
    CLongVector          Maps;                      // +0x00
    CNavigatorTypeVector VisMaps;                   // +0x0c
    CCharString          Name;                      // +0x18
    CCharString          DisplayName;               // +0x1c
    CCharString          RegionDefName;             // +0x20
    CDefPointer<CRegionDef> PDef;                   // +0x24
    CCharString          MiniMapGraphic;            // +0x28
    float                MiniMapScale;              // +0x2c
    C2DVector            MiniMapOffset;             // +0x30
    CExitTextOffsetMap   MiniMapRegionExitTextOffsets;   // +0x38
    C2DVector            WorldMapOffset;            // +0x44
    C2DVector            NameGraphicOffset;         // +0x4c
    bool                 AppearOnWorldMap;          // +0x54
    bool                 CreatureGenerationEnabled; // +0x55
    bool                 SoundThemesEnabled;        // +0x56
    __forceinline void SetRegionDefName(const CCharString& name)
    {
        RegionDefName = name;
        long index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(RegionDefName);
        if (index > 0)
            CGameDefinitionManager::Get().GetDef(index, &PDef);
    }
    void LoadBinary(CMemoryDataInputStream* stream);
};

void CRegion::LoadBinary(CMemoryDataInputStream* stream)
{
    long numMaps = stream->ReadLong();
    long numVisMaps = stream->ReadLong();
    Maps.resize(numMaps);
    for (long i = 0; i < numMaps; ++i)
        Maps._M_start[i] = stream->ReadLong();
    VisMaps.resize(numVisMaps);
    for (long j = 0; j < numVisMaps; ++j)
        VisMaps._M_start[j] = (ENavigatorType)stream->ReadLong();
    Name = stream->ReadPresizedString();
    DisplayName = stream->ReadPresizedString();
    SetRegionDefName(stream->ReadPresizedString());
    MiniMapGraphic = stream->ReadPresizedString();
    AppearOnWorldMap = stream->ReadBool();
    CreatureGenerationEnabled = stream->ReadBool();
    SoundThemesEnabled = stream->ReadBool();
    MiniMapScale = stream->ReadFloat();
    MiniMapOffset.X = (float)stream->ReadLong();
    MiniMapOffset.Y = (float)stream->ReadLong();
    WorldMapOffset.X = (float)stream->ReadLong();
    WorldMapOffset.Y = (float)stream->ReadLong();
    long numExitTextOffsets = stream->ReadLong();
    for (long k = 0; k < numExitTextOffsets; ++k) {
        CCharString exitName = stream->ReadPresizedString();
        C2DVector offset;
        stream->Read(&offset);
        MiniMapRegionExitTextOffsets[exitName] = offset;
    }
}