// boost::checked_delete<CThingSearchTools>(CThingSearchTools* x) -- the scoped_ptr<CThingSearchTools> deleter: destroy and free it
// Leaf: ??$checked_delete@VCThingSearchTools@@@boost@@YIXPAVCThingSearchTools@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004ADD90, 34 bytes, `__fastcall` (the pointer in ecx), plain `ret`.
// Identity (probable): no reference (dead COMDAT); it sits in the CWorld TU between the landed boost::checked_delete<CGameTimeManager> 0x004ADD30 and checked_delete<CBulletTimeManager> 0x004AE080; FableWin.pdb has ??$checked_delete@VCThingSearchTools@@@boost@@ (CWorld::PThingSearchTools is a scoped_ptr<CThingSearchTools>), and Ego_r.pdb CThingSearchTools {WorldMap, PThingMapManager, ThingManager, vector<NThingMap::CThingMap*> AreaMaps +0xC} is the only CWorld-owned class whose sole non-trivial member is a pointer vector at +0xC.
// Callees: free 0x00BFEA14 (the vector's storage), operator delete 0x00BFE9BC.
// Shape (family M079, representative 0x004ADD90): `delete p;` (null-tested) with the implicit CThingSearchTools destructor inline --
//   only the POD vector at +0xC has work: STLport ~_Vector_base `if (_M_start) deallocate` (free).
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
void __cdecl operator delete(void* p);                      // 0x00BFE9BC
namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
template <class _Tp, class _Alloc> class _Vector_base {
public:
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { }
};
}
class CWorldMap;
class CThingMapManager;
class CThingManager;
namespace NThingMap { class CThingMap; }
class CThingSearchTools {                                 // Ego_r.pdb, no vtable
public:
    CWorldMap* WorldMap;                                  // +0x0
    CThingMapManager* PThingMapManager;                   // +0x4
    CThingManager* ThingManager;                          // +0x8
    std::vector<NThingMap::CThingMap*> AreaMaps;          // +0xc
};
namespace boost {
template <class T> inline void __fastcall checked_delete(T* x)
{
    typedef char type_must_be_complete[sizeof(T)];
    delete x;
}
template void __fastcall checked_delete<CThingSearchTools>(CThingSearchTools* x);
}