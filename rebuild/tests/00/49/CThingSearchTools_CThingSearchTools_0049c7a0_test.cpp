// test for 0049c7a0 CThingSearchTools ctor
#include <stdio.h>
#include <string.h>
#include <new>
namespace NThingMap { struct CThingMap_; }
class CThingManager;
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
public:
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

int main()
{
    unsigned char wbuf[0x14]; memset(wbuf, 0, sizeof(wbuf));
    CThingMapManager* mm = (CThingMapManager*)0x1234;
    memcpy(wbuf + 0x10, &mm, 4);
    CWorldMap& wm = *(CWorldMap*)wbuf;
    CThingManager& tm = *(CThingManager*)0x5678;
    unsigned char buf[0x18]; memset(buf, 0x77, sizeof(buf));
    CThingSearchTools* t = new (buf) CThingSearchTools(tm, wm);
    int ok = t->WorldMap == &wm && t->ThingManager == &tm && t->PThingMapManager == mm
          && t->AreaMaps._M_start == 0 && t->AreaMaps._M_finish == 0 && t->AreaMaps._M_end_of_storage._M_data == 0
          && sizeof(*t) == 0x18;
    printf(ok ? "0049c7a0_TEST PASS\n" : "0049c7a0_TEST FAIL\n");
    return ok ? 0 : 1;
}