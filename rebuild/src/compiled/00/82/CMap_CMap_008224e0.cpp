// CMap::CMap(const CGameDefinitionManager& defs, CWorld& world, unsigned long* worldSeed, long, const C2DBoxI& area, CAFile& file, CASuspendableProcess* process) -- build a map covering a world area and load its contents from a level file
// Leaf: ??0CMap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008224E0, length 348 bytes (padded to 350), `ret 1Ch`.
// Ego_r.pdb: `??0CMap@@QAE@ABVCGameDefinitionManager@@AAVCWorld@@PAKJABVC2DBoxI@@AAVCAFile@@PAVCASuspendableProcess@@@Z`; the long
//   argument is unused (MapUID is cleared, not taken from it).
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable() @ 0x0099A2F0, malloc @ 0x00BFEA0E (brush-list sentinel), __ftol2 @ 0x00BFEA70,
//          list<CScriptedMapBrush>::clear() @ 0x00824580, vector<CScriptedMapBrushUpdateArea>::erase @ 0x00825CF0,
//          CMap::InitHeightMap @ 0x0081ED90, CMap::InitGameMap @ 0x0081EE00, CMap::InitThemePalette @ 0x008F71A0 (protected, FableWin/Ego_r.pdb ?InitThemePalette@CMap@@IAEXXZ),
//          CMap::LoadFromFile(CAFile&, CASuspendableProcess*) @ 0x00821A80.
// Vtables / layout: as the sibling constructor 0x00820CC0 (same TU): CIDrawMap 0x01272494, CMap 0x0127252C / 0x01272524.
// Shape: the UID counters and the passability flag are set first; world size = area extents, game-map size = world size / 4,
//   height-map size = game-map size * 4, world position = area minimum; then the containers are cleared, the maps built and
//   the file loaded. The vectors use the STLport _Vector_base + _STLP_alloc_proxy model. WorldPos is assigned from a
//   C2DCoordI(x, y) temporary (both words loaded before the stores; a plain struct copy interleaves them).
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E

struct CWorld;
struct CGameDefinitionManager;
struct CCachedVolumes;
struct CGameMap;
struct CAFile;
struct CASuspendableProcess;

struct C2DCoordI {
    long X, Y;
    C2DCoordI() {}
    C2DCoordI(long x, long y) : X(x), Y(y) {}
};

struct C2DBoxI {
    C2DCoordI Min, Max;
};

struct CMapInit {
    long WorldSizeX;
    long WorldSizeY;
};

struct CThemePaletteEntry {
    unsigned long ThemeDefIndex;
    bool          ThemePaletteIndexUsed;
};

struct CHeightMap {
    void Clear();                                       // 0x008F6050
};

template <class T> struct scoped_ptr {
    T* ptr;
    scoped_ptr() : ptr(0) {}
    T* operator->() const { return ptr; }
};

struct CScriptedMapBrushList {                         // retail STLport list<CScriptedMapBrush>: the sentinel pointer
    struct node { node* next; node* prev; unsigned char value[0x54]; };
    node* head;
    CScriptedMapBrushList() : head(0)
    {
        node* sentinel = (node*)malloc(sizeof(node));
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
        head = sentinel;
    }
    void clear();                                       // 0x00824580
};

template <class T> struct CAllocator {              // STLport allocator<T>: empty
    CAllocator() {}
};
template <class T> struct CAllocProxy : CAllocator<T> {   // STLport _STLP_alloc_proxy<T*, T, allocator<T> >
    T* m_data;
    CAllocProxy(const CAllocator<T>& alloc, T* p) : CAllocator<T>(alloc), m_data(p) {}
};
template <class T> struct CVectorBase {             // STLport _Vector_base<T, allocator<T> >
    T*             _M_start;
    T*             _M_finish;
    CAllocProxy<T> _M_end_of_storage;
    CVectorBase(const CAllocator<T>& alloc) : _M_start(0), _M_finish(0), _M_end_of_storage(alloc, 0) {}
};
template <class T> struct CRetailVector : CVectorBase<T> {   // STLport vector<T> (12 bytes)
    CRetailVector() : CVectorBase<T>(CAllocator<T>()) {}
    T* begin() { return this->_M_start; }
    T* end() { return this->_M_finish; }
    T* erase(T* first, T* last);
    void clear() { erase(begin(), end()); }
};

struct CScriptedMapBrushUpdateArea {
    C2DBoxI Area;
};

template <> CScriptedMapBrushUpdateArea* CRetailVector<CScriptedMapBrushUpdateArea>::erase(
    CScriptedMapBrushUpdateArea* first, CScriptedMapBrushUpdateArea* last);   // 0x00825CF0

class CIDrawMap {
public:
    virtual ~CIDrawMap() {}
    virtual long DrawGetHeightMapSizeX() const = 0;
    virtual long DrawGetHeightMapSizeY() const = 0;
};

class CBaseClassNonCopyable {
public:
    CBaseClassNonCopyable();                            // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();
};

class CMap : public CIDrawMap, public CBaseClassNonCopyable {
public:
    CMap(const CGameDefinitionManager& defs, CWorld& world, unsigned long* worldSeed, long, const C2DBoxI& area, CAFile& file,
         CASuspendableProcess* process);
    virtual ~CMap();
    virtual long DrawGetHeightMapSizeX() const;
    virtual long DrawGetHeightMapSizeY() const;

protected:
    void InitHeightMap();                               // 0x0081ED90
    void InitGameMap();                                 // 0x0081EE00
    void InitThemePalette();                            // 0x008F71A0
public:
    void LoadFromFile(CAFile& file, CASuspendableProcess* process);   // 0x00821A80
    void Clear()
    {
        HeightMap->Clear();
        NoChangedHeightAreas = 0;
        NoChangedGameAreas = 0;
        SeaLevel = 0;
    }

    scoped_ptr<CCachedVolumes>                CachedVolumes;                  // +0x08
    CScriptedMapBrushList                     ScriptedMapBrushes;             // +0x0c
    CRetailVector<CScriptedMapBrushUpdateArea> ScriptedMapBrushUpdateAreas;   // +0x10
    const CGameDefinitionManager*             DefinitionManager;              // +0x1c
    CWorld*                                   World;                          // +0x20
    unsigned long*                            WorldSeed;                      // +0x24
    float                                     RippleBuffer[41][41];           // +0x28
    CRetailVector<CThemePaletteEntry>         ThemePaletteEntryList;          // +0x1a6c
    scoped_ptr<CGameMap>                      GameMap;                        // +0x1a78
    scoped_ptr<CHeightMap>                    HeightMap;                      // +0x1a7c
    C2DBoxI                                   ChangedHeightAreas[20];         // +0x1a80
    C2DBoxI                                   ChangedGameAreas[20];           // +0x1bc0
    long                                      NoChangedHeightAreas;           // +0x1d00
    long                                      NoChangedGameAreas;             // +0x1d04
    long                                      SeaLevel;                       // +0x1d08
    long                                      WorldSizeX;                     // +0x1d0c
    long                                      WorldSizeY;                     // +0x1d10
    long                                      HeightMapSizeX;                 // +0x1d14
    long                                      HeightMapSizeY;                 // +0x1d18
    long                                      GameMapSizeX;                   // +0x1d1c
    long                                      GameMapSizeY;                   // +0x1d20
    C2DCoordI                                 WorldPos;                       // +0x1d24
    unsigned long                             MapUID;                         // +0x1d2c
    unsigned __int64                          ThingUIDCount;                  // +0x1d30
    bool                                      InitialisePassabilityFromThemes; // +0x1d38
};

CMap::CMap(const CGameDefinitionManager& defs, CWorld& world, unsigned long* worldSeed, long, const C2DBoxI& area, CAFile& file,
           CASuspendableProcess* process)
    : DefinitionManager(&defs),
      World(&world),
      WorldSeed(worldSeed)
{
    InitialisePassabilityFromThemes = false;
    MapUID = 0;
    ThingUIDCount = 1;
    WorldSizeX = area.Max.X - area.Min.X;
    WorldSizeY = area.Max.Y - area.Min.Y;
    GameMapSizeX = (long)(WorldSizeX * 0.25f);
    GameMapSizeY = (long)(WorldSizeY * 0.25f);
    HeightMapSizeX = GameMapSizeX * 4;
    HeightMapSizeY = GameMapSizeY * 4;
    ScriptedMapBrushes.clear();
    ScriptedMapBrushUpdateAreas.clear();
    WorldPos = C2DCoordI(area.Min.X, area.Min.Y);
    InitHeightMap();
    InitGameMap();
    InitThemePalette();
    LoadFromFile(file, process);
}