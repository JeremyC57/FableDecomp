// CWorldMap::DeleteAllLevels() -- delete every thing, then for each real map (from index 1) drop its navigation map
// and its engine map and clear its info; empty the map array (keeping the null entry 0), zero the map placement grid
// and release the loaded-map list
// Leaf: DeleteAllLevels
// Retail: Fable.exe (TLC, VC7.1) @ 0x005022F0, 359 bytes, `ret` (virtual slot 1, PDB ?DeleteAllLevels@CWorldMap@@UAEXXZ).
// Callees: CWorld virtual slot 3 GetThingManager (+0x0c), CThingManager::DeleteAllThings 0x0051FBA0,
//   CNavigatorManager::RemoveNavigationMap(long) 0x00A155D0 on the world's +0x48, CTBaseSingleton<CEngineManager>::Get
//   0x00436FB0 (__fastcall) and CEngineManager::RemoveMap(long) 0x006403D0, CMapInfo::Clear 0x004FDDE0,
//   CMapInfo::operator= 0x00513BF0 (erase's copy-down), the out-of-line range destroy 0x00515250 (__fastcall first,
//   last, tag), vector<CMapInfo>::resize(unsigned) 0x0051BE20, memmove 0x00BFEAE6, free 0x00BFEA14.
// Layout: retail CWorldMap (vectors are 12 bytes, so offsets sit below rebuild/include/engine/CWorldMap.h, as the
//   landed CWorldMap rows model them): World +0x08, Maps +0x20 (0x48-byte CMapInfo), MapPlacement (unsigned short)
//   +0x6c, LoadedMaps (long) +0x90.
// Shape: STLport erase(begin(), end()) (copy-down, destroy of the tail, finish store) then resize(1); the loaded-map
//   list is emptied by its erase (byte-count memmove copy-down) and its storage freed when it has any capacity.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
namespace std { template <class T> inline void swap(T& a, T& b) { T tmp = a; a = b; b = tmp; } }
struct CThingManager {
    void DeleteAllThings();                             // 0x0051FBA0
};
struct CNavigatorManager {
    void RemoveNavigationMap(long map);                 // 0x00A155D0
};
struct CEngineManager {
    static CEngineManager& __fastcall Get();            // 0x00436FB0
    void RemoveMap(long map);                           // 0x006403D0
};
struct CWorld {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual CThingManager* GetThingManager();           // slot 3 (+0x0c)
    unsigned char      _pad04[0x48 - 4];
    CNavigatorManager* PNavigationManager;              // +0x48
};
struct __false_type { };
struct CMapInfo {                                       // 0x48 bytes
    unsigned char _body[0x48];
    CMapInfo& operator=(const CMapInfo& other);         // 0x00513BF0
    void Clear();                                       // 0x004FDDE0
};
void __fastcall DestroyRange(CMapInfo* first, CMapInfo* last, const __false_type& tag);   // 0x00515250
struct CMapInfoVector {
    CMapInfo* _M_start;
    CMapInfo* _M_finish;
    CMapInfo* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
    CMapInfo& operator[](long n) { return _M_start[n]; }
    CMapInfo* begin() { return _M_start; }
    CMapInfo* end() { return _M_finish; }
    static CMapInfo* copy(CMapInfo* first, CMapInfo* last, CMapInfo* result)
    {
        for (long n = last - first; n > 0; --n) {
            *result = *first;
            ++first;
            ++result;
        }
        return result;
    }
    CMapInfo* erase(CMapInfo* first, CMapInfo* last)
    {
        CMapInfo* i = copy(last, _M_finish, first);
        __false_type tag;
        DestroyRange(i, _M_finish, tag);
        _M_finish = i;
        return first;
    }
    void clear() { erase(begin(), end()); }
    void resize(unsigned int n);                        // 0x0051BE20
};
struct CShortVector {
    unsigned short* _M_start;
    unsigned short* _M_finish;
    unsigned short* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    unsigned short& operator[](unsigned int n) { return _M_start[n]; }
};
struct CLongArray {
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    unsigned int capacity() const { return _M_end_of_storage - _M_start; }
    static long* CopyDown(long* src, long* srcEnd, long* dest)
    {
        if (src == srcEnd)
            return dest;
        unsigned int bytes = (char*)srcEnd - (char*)src;
        return (long*)((char*)memmove(dest, src, bytes) + bytes);
    }
    void Erase(long* eraseFirst, long* eraseLast)
    {
        _M_finish = CopyDown(eraseLast, _M_finish, eraseFirst);
    }
    CLongArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CLongArray() { if (_M_start) free(_M_start); }
    void swap(CLongArray& x)
    {
        std::swap(_M_start, x._M_start);
        std::swap(_M_finish, x._M_finish);
        std::swap(_M_end_of_storage, x._M_end_of_storage);
    }
    void Reset()
    {
        if (capacity() > 0) {
            Erase(_M_start, _M_finish);
            CLongArray empty;
            swap(empty);
        }
    }
};
struct CWorldMap {
    virtual ~CWorldMap();
    virtual void DeleteAllLevels();                     // slot 1
    long           f4;                                  // +0x04
    CWorld*        World;                               // +0x08
    unsigned char  _pad0c[0x20 - 0x0c];
    CMapInfoVector Maps;                                // +0x20
    unsigned char  _pad2c[0x6c - 0x2c];
    CShortVector   MapPlacement;                        // +0x6c
    unsigned char  _pad78[0x90 - 0x78];
    CLongArray     LoadedMaps;                          // +0x90
};
void CWorldMap::DeleteAllLevels()
{
    World->GetThingManager()->DeleteAllThings();
    for (long i = 1; i < Maps.size(); ++i) {
        World->PNavigationManager->RemoveNavigationMap(i);
        CEngineManager::Get().RemoveMap(i);
        Maps[i].Clear();
    }
    Maps.clear();
    Maps.resize(1);
    for (unsigned int j = 0; j < MapPlacement.size(); ++j)
        MapPlacement[j] = 0;
    LoadedMaps.Reset();
}