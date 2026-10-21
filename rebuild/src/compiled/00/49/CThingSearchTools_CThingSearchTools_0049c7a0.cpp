// CThingSearchTools::CThingSearchTools(CThingManager&, CWorldMap&) -- bind thing/world managers, empty area-map vector, cache the world's thing-map manager
// Retail: Fable.exe @ 0x0049C7A0, 31 bytes; Ego_r.pdb ??0CThingSearchTools@@QAE@AAVCThingManager@@AAVCWorldMap@@@Z.
// Layout: 0x18 bytes = Ego_r.pdb CThingSearchTools (0x1c) minus 4 for the 12-byte STLport vector (retail zeroes only
// +0x0c/+0x10/+0x14). PThingMapManager is read from CWorldMap +0x10 (the PThingMapManager scoped_ptr's raw pointer).
// Shape: thing-map manager read through an inline const getter, assigned in the body after the vector is built.
namespace NThingMap { struct CThingMap_; }
struct CThingManager;
struct CThingMapManager;
template <class T> class allocator { public: allocator() {} };
template <class V, class A> struct _STLP_alloc_proxy : public A {
    V _M_data;
    _STLP_alloc_proxy(const A& a, V p) : A(a), _M_data(p) {}
};
template <class T, class A> class _Vector_base {
public:
    _Vector_base(const A& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0) {}
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, A> _M_end_of_storage;
};
template <class T, class A = allocator<T> > class vector : public _Vector_base<T, A> {
public:
    explicit vector(const A& a = A()) : _Vector_base<T, A>(a) {}
};
class CWorldMap {
public:
    CThingMapManager* GetThingMapManager() const { return PThingMapManager_ptr; }
private:
    char              _pad_00[0x10];
    CThingMapManager* PThingMapManager_ptr;              // +0x10
};
class CThingSearchTools {
public:
    CThingSearchTools(CThingManager& thing_manager, CWorldMap& world_map);
    CWorldMap*                       WorldMap;           // +0x00
    CThingMapManager*                PThingMapManager;   // +0x04
    CThingManager*                   ThingManager;       // +0x08
    vector<NThingMap::CThingMap_*>   AreaMaps;           // +0x0c
};
CThingSearchTools::CThingSearchTools(CThingManager& thing_manager, CWorldMap& world_map)
    : WorldMap(&world_map), ThingManager(&thing_manager)
{
    PThingMapManager = world_map.GetThingMapManager();
}