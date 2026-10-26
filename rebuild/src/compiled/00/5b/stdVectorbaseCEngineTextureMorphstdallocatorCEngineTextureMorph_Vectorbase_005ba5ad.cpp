#pragma optimize("s", on)
// std::_Vector_base<CEngineTextureMorph, std::allocator<CEngineTextureMorph> >::_Vector_base(size_t n, const allocator&
//   a) -- STLport sized base constructor: null the three words, allocate n morphs, finish = start, end of storage = start +
//   n
// Leaf: ??0?$_Vector_base@VCEngineTextureMorph@@V?$allocator@VCEngineTextureMorph@@@std@@@std@@QAE@IABV?$allocator@VCEngineTextureMorph@@@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005BA5AD, 51 bytes, `ret 8`; size-optimised TU (allocate out of line, start + n*20
//   as two lea).
// Only caller: vector<CEngineTextureMorph>::vector(const vector&) 0x005BBDF1 (0x005BBE0D).
// Identity (sweep3; FINDINGS chunk 477 "0x5BBDF1/0x5BB346/0x5BBC93 mislabelled STL copy"; was
//   std::_Vector_base<SRecord20, std::allocator<SRecord20> >::_Vector_base (invented SRecord20 record)).
// Element: CEngineTextureMorph = Ego_r.pdb {long OriginalTextureIndex +0; long MorphID +4; vector<CCompositeBlendDesc> Blends
//   +8} (24 bytes with Ego_r's 16-byte vector, 20 with retail's 12-byte STLport vector). Its copy constructor 0x005BB9FA copies
//   the two longs and calls vector<CCompositeBlendDesc>'s copy constructor 0x005BB346 on +8 (sweep1 label); the vector copy
//   constructor 0x005BBDF1's callers include CEnginePrimitive2DMorphedAnimatingMesh's ctor 0x005BC1B0 (Ego_r/FableWin
//   ??0CEnginePrimitive2DMorphedAnimatingMesh@@QAE@_KKABV?$vector@VCEngineTextureMorph@@... takes a const
//   vector<CEngineTextureMorph>&) and CTCHeroAttachableAppearanceModifiers::GetMeshesAndMorphSets 0x00707B10 (its
//   CEngineTextureMorphSet holds the vector); Ego_r has ??0?$vector@VCEngineTextureMorph@@...@QAE@ABV01@@Z and the
//   Dinkumware _Uninit_copy<..CEngineTextureMorph..> twin of this run. Chain: 0x005BBDF1 vector(const vector&) ->
//   0x005BA5AD _Vector_base(n, a) -> 0x005B9DE7 allocator::allocate, and 0x005BBC93 __uninitialized_copy -> 0x005BB9FA.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

class CCompositeBlendDesc;

namespace std {

struct __false_type { __false_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0);
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(unsigned int __n, const _Alloc& __a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    _Tp* _M_start;                                     // +0x0
    _Tp* _M_finish;                                    // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x8
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Alloc allocator_type;
    typedef const _Tp* const_pointer;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    allocator_type get_allocator() const { return allocator_type(); }
    vector(const vector<_Tp, _Alloc>& __x);
};

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                             const __false_type&);

}   // namespace std

class CEngineTextureMorph {                            // 20 bytes (retail)
public:
    long OriginalTextureIndex;                         // +0x0
    long MorphID;                                      // +0x4
    std::vector<CCompositeBlendDesc> Blends;           // +0x8 (copy constructor 0x005BB346)
    CEngineTextureMorph(const CEngineTextureMorph& other);   // 0x005BB9FA
};

template class std::_Vector_base<CEngineTextureMorph, std::allocator<CEngineTextureMorph> >;
