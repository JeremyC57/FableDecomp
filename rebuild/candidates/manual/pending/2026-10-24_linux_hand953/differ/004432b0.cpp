// std::deque::~deque() -- (deque<std::pair<CCharString, long>>) destroy every element through the out-of-line __destroy_aux, then the inlined ~_Deque_base frees the node buffers and the map
// Grade: DIFFER(116v150) -- the inlined _Destroy/__destroy pass the iterators straight into the argument slots; retail first copies _M_finish into four registers and _M_start into a 0x10-byte frame temp, then pushes them (frame 0x14 vs 4, 116 vs 150 bytes)
// Leaf: ??1?$deque@U?$pair@VCCharString@@J@std@@V?$allocator@U?$pair@VCCharString@@J@std@@@2@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004432B0, 150 bytes (+ int3 padding), thiscall, plain `ret`; 0x14-byte frame (the copied start iterator and the empty __false_type).
// Identity (proven): std::deque<std::pair<CCharString, long>, std::allocator<std::pair<CCharString, long>> >::~deque() (FableWin.pdb name set); evidence: its __destroy_aux steps 8-byte elements (0x80 per node) and runs ~CCharString 0x0099EAE0 on +0 = pair<CCharString, long>, the only Ego_r deque of such an element (CDrawGuildSeal::Messages, queue<pair<CCharString, long> >); its callers are 0x00444ED0, which destroys it at this+0x68 just before the vector at +0x50 (Ego_r CDrawGuildSeal: LevelUpPositions, then Messages), and the `jmp` 0x00443E10 (queue::~queue); the catalog already binds this address as ??1?$deque@U?$pair@VCCharString@@J@std@@@std@@QAE@XZ
// Label: the manifest's "FreeBuffer" (CBankFileAsyncData::FreeBuffer) is wrong: the row is a deque destructor (family J049: five byte-similar deque destructors, one per element type).
// Callees: std::__destroy_aux<_Deque_iterator<std::pair<CCharString, long>, _Nonconst_traits<std::pair<CCharString, long>> > >(first, last, const __false_type&) 0x00440C00 (__fastcall: the two
//   16-byte iterators on the stack, the traits reference in ecx; steps 8-byte elements, ~CCharString on each), _Deque_base::_M_destroy_nodes 0x0043F560 (protected), free 0x00BFEA14.
// Body: STLport 4.6 _deque.h `~deque() { _Destroy(_M_start, _M_finish); }` with the base destructor inlined: `if (_M_map._M_data) {
//   _M_destroy_nodes(_M_start._M_node, _M_finish._M_node + 1); _M_map.deallocate(_M_map._M_data, _M_map_size._M_data); }`, the
//   allocator's deallocate being `if (p) free(p)`.
// Layout (retail STLport deque 0x28): _M_start +0x00, _M_finish +0x10 (16-byte iterators: cur, first, last, node), _M_map +0x20, _M_map_size +0x24.
// Model: the landed 0x0048C820 _Deque_base (deque<C2DVector>::_M_destroy_nodes) with STLport's ~deque, _Destroy, __destroy and __type_traits.
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
class CCharStringData;
class CCharString {                                         // Ego_r.pdb 4 bytes
public:
    ~CCharString();                                         // 0x0099EAE0
    CCharStringData* PStringData;
};
namespace std {
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
}
namespace std {
struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };   // every class type
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; typedef _Tp* pointer; };
template <class _Tp> struct _Deque_iterator_base {          // 16 bytes
    _Tp*  _M_cur;                                           // +0x00
    _Tp*  _M_first;                                         // +0x04
    _Tp*  _M_last;                                          // +0x08
    _Tp** _M_node;                                          // +0x0c
};
template <class _Tp, class _Traits> struct _Deque_iterator : public _Deque_iterator_base<_Tp> { };
template <class _Tp> inline _Tp* value_type(const _Deque_iterator_base<_Tp>&) { return (_Tp*)0; }

template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // out of line
template <class _ForwardIterator, class _Tp>
inline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy(__first, __last, value_type(__first));
}

template <class _Tp, class _Alloc> class _Deque_base {      // STLport 4.6 _deque.h
public:
    typedef _Deque_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    ~_Deque_base()
    {
        if (_M_map._M_data) {
            _M_destroy_nodes(_M_start._M_node, this->_M_finish._M_node + 1);
            _M_map.deallocate(_M_map._M_data, _M_map_size._M_data);
        }
    }
protected:
    void _M_destroy_nodes(_Tp** __nstart, _Tp** __nfinish);   // 0x0043F560
    iterator _M_start;                                      // +0x00
    iterator _M_finish;                                     // +0x10
    _STLP_alloc_proxy<_Tp**, _Tp*, allocator<_Tp*> > _M_map;            // +0x20
    _STLP_alloc_proxy<unsigned int, _Tp, _Alloc>     _M_map_size;       // +0x24
};
template <class _Tp, class _Alloc = allocator<_Tp> > class deque : protected _Deque_base<_Tp, _Alloc> {
public:
    ~deque() { _Destroy(this->_M_start, this->_M_finish); }
};
template class deque<pair<CCharString, long>, allocator<pair<CCharString, long> > >;
}   // namespace std
