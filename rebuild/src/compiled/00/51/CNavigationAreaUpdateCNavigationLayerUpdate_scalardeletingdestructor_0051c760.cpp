// CNavigationAreaUpdate::CNavigationLayerUpdate::`scalar_deleting_destructor'(unsigned int flags) -- destroy a navigation layer update (switchable blockages, then both vector blocks) and free it when bit 0 of flags is set
// Leaf: ??_GCNavigationLayerUpdate@CNavigationAreaUpdate@@QAEPAXI@Z
// Retail: Fable.exe @ 0x0051C760, 72 bytes (16-aligned /O2 entry), thiscall `ret 4`, returns this. No vptr store: the class
// is not polymorphic, so this is the non-virtual scalar deleting destructor VC7.1 emits for a `delete` of the class.
// Identity (probable): the body destroys exactly a retail CNavigationAreaUpdate::CNavigationLayerUpdate (Ego_r.pdb:
// LineList vector<C2DLineF> +0, SwitchableBlockages vector<CSwitchableBlockage> +0xC; 0x18 bytes with STLport vectors):
// the +0xC range goes through __destroy_aux<CSwitchableBlockage*> 0x00519EF0 (landed; its callers are all the area-update
// TU's layer code, e.g. __destroy_aux<CNavigationLayerUpdate> 0x0051CD80 over the same +0xC vector), the +0 block is only
// freed (C2DLineF is trivially destructible). No call or address reference to this body survives in retail. The catalog
// label CLandscapeBackgroundLoadTask::`scalar_deleting_destructor' is propagated.
// Callees: std::__destroy_aux<CNavigationAreaUpdate::CSwitchableBlockage*>(first, last, const __false_type&) 0x00519EF0
// (__fastcall, the PDB-style spelling of its landed definition); free 0x00BFEA14 (twice); operator delete 0x00BFE9BC.
// Shape: STLport ~vector() = _Destroy(start, finish) then ~_Vector_base frees behind a null test, members in reverse order.
// Retail homes the __false_type tag in the still-live `flags` argument slot ([esp+8]); that needs the same-TU noinline
// __destroy_aux body below (auto_inline off; declared-only, VC7.1 gives the tag its own frame byte). KillLayer is a
// minimal same-TU `delete` that makes VC7.1 emit the ??_G (it is not part of the retail row).
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
void __cdecl operator delete(void* p);                  // 0x00BFE9BC
class C2DVector { public: float X, Y; };
class C2DLineF { public: C2DVector Point1; C2DVector Point2; };
namespace std {
template <class _Tp> class allocator { };
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list { public:
    _List_node_base* _M_node;
    void clear()
    {
        _List_node_base* cur = _M_node->_M_next;
        while (cur != _M_node) {
            _List_node_base* tmp = cur;
            cur = cur->_M_next;
            free(tmp);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
    ~list()
    {
        clear();
        if (_M_node)
            free(_M_node);
    }
};
struct __true_type { __true_type() {} };
struct __false_type { __false_type() {} };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
#pragma auto_inline(off)
template <class _ForwardIterator>
__declspec(noinline) void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)   // 0x00519EF0
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
#pragma auto_inline(on)
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) {}
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <> struct __type_traits<C2DLineF> { typedef __true_type has_trivial_destructor; };
template <class _ForwardIterator, class _Tp>
inline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy(__first, __last, __first); }
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Tp, class _Alloc> class _Vector_base { public:
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> { public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}
class CNavigationAreaUpdate { public:
    class CSwitchableBlockage { public:
        unsigned __int64        UniqueID;               // +0x00
        std::list<C2DLineF>     LineList;               // +0x08
    };
    class CNavigationLayerUpdate { public:
        std::vector<C2DLineF>             LineList;            // +0x00
        std::vector<CSwitchableBlockage>  SwitchableBlockages; // +0x0C
    };
};
void KillLayer(CNavigationAreaUpdate::CNavigationLayerUpdate* p) { delete p; }   // emits the ??_G (not a retail row)