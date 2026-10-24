// std::__destroy_aux(CNavigationLayerUpdate* first, CNavigationLayerUpdate* last, const __false_type&) -- destroy a range of CNavigationLayerUpdate (switchable blockages through their own __destroy_aux, then both blocks)
// Leaf: ?__destroy_aux@std@@YIXPAUCNavigationLayerUpdate@FableDestroy_0051cd80@@0ABU__false_type@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051CD80, length 76 bytes, `ret 4`.
// Element: CNavigationLayerUpdate (Ego_r.pdb: LineList vector<C2DLineF> +0, SwitchableBlockages
// vector<CNavigationAreaUpdate::CSwitchableBlockage> +0xC; retail 24 bytes with 12-byte STLport vectors). The implicit
// destructor runs the members in reverse: SwitchableBlockages' `_Destroy(start, finish)` dispatches to the
// out-of-line __destroy_aux<CSwitchableBlockage> 0x00519EF0 (16-byte elements: UniqueID +0, a 4-byte STLport
// list<C2DLineF> +8 cleared node by node) with an empty tag temporary homed in the `push ecx` frame word
// (`lea eax,[esp+0Bh]`), then frees its block; C2DLineF is trivially destructible, so LineList only frees.
// Callers: CWorldMap::ActivateNavMap 0x0050AF10 (landed), 0x00509A20, and the vector helpers 0x0051D0A0..0x0051D800.
// Callees: std::__destroy_aux<CSwitchableBlockage> @ 0x00519EF0 (manifest label `_Move_backward<pair<unsigned long,
// CSoundPair>*>` is propagated), free @ 0x00BFEA14.
// Shape: STLport 4.x `__destroy_aux(first, last, const __false_type&)` from a /Gr TU (`__fastcall`: first in ecx, last
// in edx, the empty tag reference on the stack, `ret 4`): `for (; first != last; ++first) _Destroy(&*first);` with the
// element's implicit destructor inlined.
// Label correction: the catalog's `_Uninit_copy<_Vector_const_iterator<_Vector_val<unsigned int,allocator<unsigned int> > >,
// unsigned int*,allocator<unsigned int> >` is a Dinkumware name propagated over all 40 rows of this chunk; retail's STL
// is STLport 4.x and the `ret 4` (no destination), the element stride and the inline destructor calls name this body.
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
struct __false_type { __false_type() { } };
namespace FableDestroy_0051cd80 {
struct C2DVector { float X, Y; };                      // POD
struct C2DLineF {                                       // Ego_r.pdb, 16 bytes, POD (trivial destructor)
    C2DVector Point1;                                   // +0x00
    C2DVector Point2;                                   // +0x08
};
struct SC2DLineFList {                                  // STLport list<C2DLineF>: 4 bytes (cleared by 0x00519EF0)
    void* _M_node;
};
struct CSwitchableBlockage {                            // CNavigationAreaUpdate::CSwitchableBlockage, retail 16 bytes
    unsigned __int64 UniqueID;                          // +0x00
    SC2DLineFList    LineList;                          // +0x08
};
}
namespace std {
inline void _Destroy(FableDestroy_0051cd80::C2DLineF*, FableDestroy_0051cd80::C2DLineF*) { }   // __true_type: nothing
void __fastcall __destroy_aux(FableDestroy_0051cd80::CSwitchableBlockage* first,
                              FableDestroy_0051cd80::CSwitchableBlockage* last, const __false_type&);   // 0x00519EF0
inline void _Destroy(FableDestroy_0051cd80::CSwitchableBlockage* first, FableDestroy_0051cd80::CSwitchableBlockage* last)
{
    __destroy_aux(first, last, __false_type());
}
}
namespace FableDestroy_0051cd80 {
template <class T>
struct CRetailVector {                                  // STLport vector<T>: 12 bytes (start, finish, end of storage)
    T* _M_start;                                        // +0
    T* _M_finish;                                       // +4
    T* _M_end_of_storage;                               // +8
    ~CRetailVector()
    {
        std::_Destroy(_M_start, _M_finish);
        if (_M_start)
            free(_M_start);                             // _Vector_base: deallocate through the malloc allocator
    }
};
struct CNavigationLayerUpdate {                         // Ego_r.pdb (retail 24 bytes)
    CRetailVector<C2DLineF>            LineList;            // +0x00
    CRetailVector<CSwitchableBlockage> SwitchableBlockages; // +0x0C
};
}
namespace std {
template <class T> inline void _Destroy(T* p) { p->~T(); }
void __fastcall __destroy_aux(FableDestroy_0051cd80::CNavigationLayerUpdate* first, FableDestroy_0051cd80::CNavigationLayerUpdate* last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
}