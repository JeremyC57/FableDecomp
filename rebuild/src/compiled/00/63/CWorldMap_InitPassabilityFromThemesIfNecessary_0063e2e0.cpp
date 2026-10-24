// CWorldMap::InitPassabilityFromThemesIfNecessary(const C2DBoxI& area) -- clip the area to the world map and let every map under it initialise its passability from themes
// Leaf: ?InitPassabilityFromThemesIfNecessary@CWorldMap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0063E2E0, 210 bytes (+ int3 padding), `ret 4`.
// Identity: Ego_r.pdb `?InitPassabilityFromThemesIfNecessary@CWorldMap@@IAEXABVC2DBoxI@@@Z` (protected, non-virtual);
// the per-map callee is the landed CMap::InitPassabilityFromThemesIfNecessary 0x008201E0. Catalog label
// "CWorldMap_PostLoadTopology_Helper" is descriptive (PostLoadTopology is the caller).
// Callees: virtual slot 13 GetDimensions (+0x34), C2DBoxI::ClipTo(const C2DBoxI&) 0x0049C810, virtual slot 20
// GetAllMapsInArea(const C2DBoxI&, vector<unsigned long>&) const (+0x50), ClipAndConvertWorldMapAreaToLocalArea(long,
// const C2DBoxI&) const 0x004FC4D0 (hidden C2DBoxI result), virtual slot 18 GetMap(long) (+0x48), CMap::
// InitPassabilityFromThemesIfNecessary 0x008201E0, free (the STLport vector's malloc allocator).
// Shape: the area is copied into a local box through C2DBoxI's memberwise copy ctor (the implicit copy loads TLX
// first; retail loads TLY first) and clipped in place; the map is a named reference (GetMap runs before &local is pushed); the walk compares with `<`
// (retail's jae / jb), the map number re-read from the iterator for GetMap; an empty clipped box (zero width or height,
// `sub ; je`) is skipped. The vector dies through STLport's `if (_M_start) free(_M_start)`.
// ---- local CWorldMap model (the landed 0x0063A660 / 0x004FF310 models: retail vtable 0x01244AEC; the PDB header
// rebuild/include/engine/CWorldMap.h uses 16-byte STL containers where retail STLport vectors are 12 bytes).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);             // 0x00BFEA14

struct C3DVector {
    float X;
    float Y;
    float Z;
};

struct C2DCoordI {
    long x;
    long y;
};

struct C2DBoxI {
    long TLX;   // +0x00
    long TLY;   // +0x04
    long BRX;   // +0x08
    long BRY;   // +0x0C
    C2DBoxI() {}
    C2DBoxI(const C2DBoxI& o) : TLX(o.TLX), TLY(o.TLY), BRX(o.BRX), BRY(o.BRY) {}
    long GetWidth() const { return BRX - TLX; }
    long GetHeight() const { return BRY - TLY; }
    void ClipTo(const C2DBoxI& other);                                // 0x0049C810
};

namespace std {

template <class T> class allocator {
public:
    allocator() {}
    void deallocate(T* p, unsigned int) { free(p); }
};

template <class T> class vector {
public:
    typedef T* iterator;
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~vector() { if (_M_start) allocator<T>().deallocate(_M_start, _M_end_of_storage - _M_start); }
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
};

}

class CMap
{
public:
    void InitPassabilityFromThemesIfNecessary(const C2DBoxI& area);  // 0x008201E0
};

class CWorldMap
{
public:
    virtual ~CWorldMap();                                            // slot 0
    virtual void DeleteAllLevels();                                  // slot 1
    virtual void DeleteAllRegions();                                 // slot 2
    virtual void LoadFromFile();                                     // slot 3
    virtual void SaveToFile();                                       // slot 4
    virtual void RemoveLevel(long map);                              // slot 5
    virtual void LoadTopologyFromFile();                             // slot 6
    virtual void PostLoadTopology(long map);                         // slot 7
    virtual void ForceLoad(long map);                                // slot 8
    virtual void ForceUnload(long map);                              // slot 9
    virtual bool IsMapLoaded(long map) const;                        // slot 10 (+0x28)
    virtual bool IsMapUsed(long map) const;                          // slot 11
    virtual long GetGranularity() const;                             // slot 12
    virtual const C2DBoxI& GetDimensions() const;                    // slot 13 (+0x34)
    virtual CMap& GetMapAt(const C2DCoordI& pos);                    // slot 14
    virtual const CMap& PeekMapAt(const C2DCoordI& pos) const;       // slot 15
    // overloaded virtuals are laid out in reverse declaration order (C3DVector form is slot 16)
    virtual long GetMapNumberAt(const C2DCoordI& pos) const;         // slot 17 (+0x44)
    virtual long GetMapNumberAt(const C3DVector& pos) const;         // slot 16 (+0x40)
    virtual CMap& GetMap(long map);                                  // slot 18 (+0x48)
    virtual const CMap& PeekMap(long map) const;                     // slot 19 (+0x4C)
    virtual void GetAllMapsInArea(const C2DBoxI& area, std::vector<unsigned long>& maps) const;   // slot 20 (+0x50)

    C2DBoxI ClipAndConvertWorldMapAreaToLocalArea(long map, const C2DBoxI& area) const;   // 0x004FC4D0
    void InitPassabilityFromThemesIfNecessary(const C2DBoxI& area);
};

void CWorldMap::InitPassabilityFromThemesIfNecessary(const C2DBoxI& area)
{
    C2DBoxI box = area;
    box.ClipTo(GetDimensions());
    std::vector<unsigned long> maps;
    GetAllMapsInArea(box, maps);
    for (std::vector<unsigned long>::iterator it = maps.begin(); it < maps.end(); ++it) {
        C2DBoxI local = ClipAndConvertWorldMapAreaToLocalArea(*it, box);
        if (local.GetWidth() != 0 && local.GetHeight() != 0) {
            CMap& map = GetMap(*it);
            map.InitPassabilityFromThemesIfNecessary(local);
        }
    }
}