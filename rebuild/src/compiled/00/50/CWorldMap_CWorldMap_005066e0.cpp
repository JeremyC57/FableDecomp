// CWorldMap::CWorldMap(const CMainGameComponent& component, CWorld& world, const C2DBoxI& area, unsigned long granularityShift) -- build an empty world map over the area: one default map info and region, four empty player-position slots, the map-placement grid sized from the granularity, and a fresh thing map manager; sound themes start disabled
// Leaf: ??0CWorldMap@@QAE@ABVCMainGameComponent@@AAVCWorld@@ABVC2DBoxI@@K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005066E0, 601 bytes, `ret 10h`. Ego_r.pdb / FableWin.pdb
//   ??0CWorldMap@@QAE@ABVCMainGameComponent@@AAVCWorld@@ABVC2DBoxI@@K@Z.
// Callees: CBaseClassNonCopyable() 0x0099A2F0, malloc 0x00BFEA0E (STLport tree header / vector storage),
//   CArray<CMapInfo>(unsigned int) 0x00515AD0, CRegion() 0x006BC410, CRegion(const CRegion&) 0x0051A900,
//   ~CRegion 0x0051A110, CBaseIntelligentPointer() 0x00A01B10, vector<unsigned short>::resize 0x00517900,
//   operator new 0x00BFEA1A, NThingMap::CThingMapManager() 0x005191E0.
// Layout (retail; Ego_r.pdb CWorldMap with 12-byte STLport containers, everything after +0x14 sits lower than the PDB):
//   CBaseClassNonCopyable vptr +0x00, CIDrawWorldMap vptr +0x04, World +0x08, Component +0x0C, PThingMapManager +0x10,
//   LoadedScriptedMapBrushes +0x14, Maps +0x20, Regions +0x2C, RegionTime +0x38, PRegionGraph +0x44,
//   SoundThemesDisabled +0x4C, Granularity +0x50, GranularityShift +0x54, MapPlacementPitch +0x58, Area +0x5C,
//   MapPlacement +0x6C, PlayerPositionInfo +0x78 (13-byte packed entries), HeightChangedAreas +0x84, LoadedMaps +0x90,
//   LoadedRegion +0x9C, LevelWADBank +0xA0, UseLevelWADBank +0xA8, CurrentMapUIDCount +0xAC,
//   DestroyedThemePositions +0xB0, PLevelLoader +0xBC, PCameraRegionParticle +0xC4, PHeroBreathParticle +0xCC,
//   RegionUnloading +0xD4, PassabilityBeingShown +0xD5.
// Shape: the body divides by `1 << granularityShift` (the parameter, CSE'd with the Granularity initialiser, so retail
//   keeps it in ebp across the resize call) and reads the area through C2DBoxI::GetHeight / GetWidth after the copy.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
void __cdecl operator delete(void* block);              // 0x00BFE9BC
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
class C2DBoxI {
public:
    long TLX, TLY, BRX, BRY;
    long GetWidth() const { return BRX - TLX; }
    long GetHeight() const { return BRY - TLY; }
};
class C2DCoordI {
public:
    long X, Y;
};
class CCharString {
public:
    void* PStringData;
};

class CBaseClassNonCopyable {
protected:
    CBaseClassNonCopyable();                           // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();                  // 0x0099A300
};
class CIDrawWorldMap {
public:
    CIDrawWorldMap() { }
    virtual ~CIDrawWorldMap() { }
};

class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
};
class CBaseObjectPointer;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                         // 0x00A01B10
    virtual ~CBaseIntelligentPointer();
    CBaseObjectPointer* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() { }
    virtual ~CIntelligentPointer() { }
};
template <class T> class scoped_ptr {
public:
    T* ptr;
    explicit scoped_ptr(T* p = 0) : ptr(p) { }
    void reset(T* p = 0)
    {
        T* old = ptr;
        ptr = p;
        if (old)
            delete old;
    }
};

extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);
namespace std {
inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) + ((const char*)__last - (const char*)__first);
}
struct __true_type { };
struct __false_type { };
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
    _Tp* allocate(unsigned int __n) { return (_Tp*)malloc(__n * sizeof(_Tp)); }
};
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Tp> struct less { };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _ForwardIter, class _Size, class _Tp>
__forceinline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__forceinline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    __forceinline _Vector_base(unsigned int __n, const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) { }
    __forceinline explicit vector(unsigned int __n) : _Vector_base<_Tp, _Alloc>(__n, _Alloc())
    {
        this->_M_finish = uninitialized_fill_n(this->_M_start, __n, _Tp());
    }
    _Tp* begin() { return this->_M_start; }
    _Tp* end() { return this->_M_finish; }
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    _Tp* erase(_Tp* __first, _Tp* __last)
    {
        _Tp* __i = (_Tp*)__copy_trivial(__last, this->_M_finish, __first);
        this->_M_finish = __i;
        return __first;
    }
    void insert(_Tp* __pos, unsigned int __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void _M_fill_insert(_Tp* __pos, unsigned int __n, const _Tp& __x);   // 0x005149F0 for unsigned short
    void resize(unsigned int __new_size, const _Tp& __x);   // 0x00517900 for unsigned short
};

struct _Rb_tree_node_base {
    char                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base { _Value _M_value_field; };
template <class _Tp, class _Alloc> class _Rb_tree_base {
public:
    _Rb_tree_base(const _Alloc& __a) : _M_header(__a, 0) { _M_header._M_data = _M_header.allocate(1); }
    _STLP_alloc_proxy<_Rb_tree_node<_Tp>*, _Rb_tree_node<_Tp>, _Alloc> _M_header;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    _Rb_tree(const _Compare& __comp = _Compare(), const _Alloc& __a = _Alloc())
        : _Rb_tree_base<_Value, _Alloc>(__a), _M_node_count(0), _M_key_compare(__comp)
    {
        _M_empty_initialize();
    }
    void _M_empty_initialize()
    {
        this->_M_header._M_data->_M_color = 0;
        this->_M_header._M_data->_M_parent = 0;
        this->_M_header._M_data->_M_left = this->_M_header._M_data;
        this->_M_header._M_data->_M_right = this->_M_header._M_data;
    }
    unsigned int _M_node_count;
    _Compare     _M_key_compare;
};
template <class _Pair> struct _Select1st { };
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    map() : _M_t(_Compare(), _Alloc()) { }
    _Rb_tree<_Key, pair<const _Key, _Tp>, _Select1st<pair<const _Key, _Tp> >, _Compare, _Alloc> _M_t;
};

// Same-TU body of 0x00517900 (STLport 4.6 resize; retail calls it out of line, and it calls _M_fill_insert 0x005149F0).
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::resize(unsigned int __new_size, const _Tp& __x)
{
    if (__new_size < size())
        erase(begin() + __new_size, end());
    else
        insert(end(), __new_size - size(), __x);
}
#pragma auto_inline(on)
}

template <class T> class CArray : public std::vector<T> {
public:
    CArray() { }
    explicit CArray(unsigned int count);               // 0x00515AD0 for CMapInfo
};
class CMapInfo { unsigned char _body[0x48]; };
class CRegion {
public:
    CRegion();                                         // 0x006BC410
    CRegion(const CRegion& other);                     // 0x0051A900
    ~CRegion();                                        // 0x0051A110
    unsigned char _body[0x58];
};
class CThing;
class CWorld;
class CMainGameComponent;
class CWADBank;
class CRegionConnectionGraph;
class CLevelLoader;
namespace NThingMap {
class CThingMapManager {
public:
    CThingMapManager();                                // 0x005191E0
    virtual ~CThingMapManager();                       // ??_G slot 0
    unsigned char _body[0x34];
};
}

class CWorldMap : public CBaseClassNonCopyable, public CIDrawWorldMap {
public:
#pragma pack(push, 1)
    class CPlayerPositionInfo {
    public:
        bool      Set;
        C3DVector Pos;
        CPlayerPositionInfo() : Set(false) { }
        CPlayerPositionInfo(const CPlayerPositionInfo& other) : Set(other.Set), Pos(other.Pos) { }
    };
#pragma pack(pop)
    CWorldMap(const CMainGameComponent& component, CWorld& world, const C2DBoxI& area, unsigned long granularityShift);
    virtual ~CWorldMap();                               // 0x00506940
    CWorld*                                         World;                    // +0x08
    const CMainGameComponent*                       Component;                // +0x0C
    scoped_ptr<NThingMap::CThingMapManager>         PThingMapManager;         // +0x10
    std::map<CCharString, long>                     LoadedScriptedMapBrushes; // +0x14
    CArray<CMapInfo>                                Maps;                     // +0x20
    std::vector<CRegion>                            Regions;                  // +0x2C
    std::vector<long>                               RegionTime;               // +0x38
    CCountedPointer<CRegionConnectionGraph>         PRegionGraph;             // +0x44
    bool                                            SoundThemesDisabled;      // +0x4C
    long                                            Granularity;              // +0x50
    long                                            GranularityShift;         // +0x54
    long                                            MapPlacementPitch;        // +0x58
    C2DBoxI                                         Area;                     // +0x5C
    std::vector<unsigned short>                     MapPlacement;             // +0x6C
    std::vector<CPlayerPositionInfo>                PlayerPositionInfo;       // +0x78
    std::vector<C2DBoxI>                            HeightChangedAreas;       // +0x84
    CArray<long>                                    LoadedMaps;               // +0x90
    long                                            LoadedRegion;             // +0x9C
    CCountedPointer<CWADBank>                       LevelWADBank;             // +0xA0
    bool                                            UseLevelWADBank;          // +0xA8
    unsigned long                                   CurrentMapUIDCount;       // +0xAC
    std::vector<C2DCoordI>                          DestroyedThemePositions;  // +0xB0
    CCountedPointer<CLevelLoader>                   PLevelLoader;             // +0xBC
    CIntelligentPointer<CThing>                     PCameraRegionParticle;    // +0xC4
    CIntelligentPointer<CThing>                     PHeroBreathParticle;      // +0xCC
    bool                                            RegionUnloading;          // +0xD4
    bool                                            PassabilityBeingShown;    // +0xD5
};

CWorldMap::CWorldMap(const CMainGameComponent& component, CWorld& world, const C2DBoxI& area, unsigned long granularityShift)
    : World(&world), Component(&component), Maps(1), Regions(1),
      Granularity(1 << granularityShift), GranularityShift(granularityShift), PlayerPositionInfo(4),
      LoadedRegion(0), UseLevelWADBank(false), CurrentMapUIDCount(1), RegionUnloading(false), PassabilityBeingShown(false)
{
    Area = area;
    MapPlacement.resize((Area.GetHeight() / (1 << granularityShift)) * (Area.GetWidth() / (1 << granularityShift)), 0);
    MapPlacementPitch = Area.GetWidth() / (1 << granularityShift);
    PThingMapManager.reset(new NThingMap::CThingMapManager());
    SoundThemesDisabled = true;
}