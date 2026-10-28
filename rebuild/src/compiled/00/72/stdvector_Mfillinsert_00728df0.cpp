// std::vector::_M_fill_insert(pointer position, size_type n, const value_type& x) -- (vector<CTCAppearanceMorphBase::CMeshMorphSet::CMeshMorphSetParts>) STLport insert(pos, n, x) of mesh morph-set parts: with room, shift the tail in place (uninitialized copy, copy_backward, fill from a copy of x), else grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@VCMeshMorphSetParts@CMeshMorphSet@CTCAppearanceMorphBase@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00728DF0, 284 bytes, thiscall, `ret 0Ch`.
// Identity (proven): the landed resize 0x00728F20 (vector<CTCAppearanceMorphBase::CMeshMorphSet::CMeshMorphSetParts>, its
//   __destroy_aux 0x00722130 bound over CMeshMorphSetParts) reaches this body at 0x00728F70 (and the insert wrapper jumps here
//   from 0x00728F10); the no-room arm calls the landed CMeshMorphSetParts _M_insert_overflow 0x00728D10. The 16-byte element
//   is Ego_r.pdb CMeshMorphSetParts {ulong MeshIndex +0; CEngineTextureMorphSet TextureMorphs +4} with
//   CEngineTextureMorphSet {CArray<CEngineTextureMorph> Morphs} (a 12-byte STLport vector): x_copy copies MeshIndex, then
//   the morph vector through vector<CEngineTextureMorph>'s copy constructor 0x005BBDF1, and is destroyed through its
//   destructor 0x005BC0D3. Catalog label "vector_InsertMultiple" is behavioural.
// Callees: vector<CEngineTextureMorph>::vector(const vector&) 0x005BBDF1, __uninitialized_copy 0x00728B10, __copy_backward
//   0x00728BB0 (random_access tag + null Distance*), fill 0x00728C10, uninitialized_fill_n 0x00728B50 (tag-less wrapper),
//   _M_insert_overflow 0x00728D10, vector<CEngineTextureMorph>::~vector() 0x005BC0D3.
// Model: this chunk's 0x006D4850 (landed twin 0x0049B5A0 recipe), STLport 4.6 _M_fill_insert verbatim with the out-of-line
//   helpers as same-TU noinline bodies (they need the element's implicit operator=, hence vector::operator= 0x00728830).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);            // 0x00BFEA14

inline void* operator new(unsigned int, void* p) { return p; }

class CEngineTextureMorph;

namespace std {
struct __false_type { __false_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    vector(const vector<_Tp, _Alloc>& __x);                                    // 0x005BBDF1 (CEngineTextureMorph)
    ~vector();                                                                 // 0x005BC0D3 (CEngineTextureMorph)
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);            // 0x00728830 (CEngineTextureMorph)
    protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                              // 0x00728D10
    public:
protected:
    public:
    void _M_fill_insert(pointer __position, size_type __n, const _Tp& __x);
    protected:
};
}

template <class T> class CArray : public std::vector<T, std::allocator<T> > { };

class CEngineTextureMorphSet {                              // Ego_r.pdb (retail 12 bytes)
public:
    CArray<CEngineTextureMorph> Morphs;                     // +0x00
};

class CTCAppearanceMorphBase {
public:
    class CMeshMorphSet {
    public:
        class CMeshMorphSetParts {                          // Ego_r.pdb (retail 16 bytes)
        public:
            unsigned long          MeshIndex;               // +0x00
            CEngineTextureMorphSet TextureMorphs;           // +0x04
        };
    };
};

namespace std {
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                             const __false_type&)
{                                                                                 // 0x00728B10
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__forceinline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{                                                                                 // 0x00728B50
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                          _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*)
{                                                                                 // 0x00728BB0
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{                                                                                 // 0x00728C10
    for ( ; __first != __last; ++__first)
        *__first = __val;
}
#pragma auto_inline(on)

template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                _BidirectionalIter2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(pointer __position, size_type __n, const _Tp& __x)
{
    if (__n != 0) {
        if (size_type(this->_M_end_of_storage._M_data - this->_M_finish) >= __n) {
            _Tp __x_copy = __x;
            const size_type __elems_after = this->_M_finish - __position;
            pointer __old_finish = this->_M_finish;
            if (__elems_after > __n) {
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, __false_type());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, __false_type());
                fill(__position, __position + __n, __x_copy);
            } else {
                uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, __false_type());
                this->_M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        } else
            _M_insert_overflow(__position, __x, __false_type(), __n);
    }
}

typedef CTCAppearanceMorphBase::CMeshMorphSet::CMeshMorphSetParts TMeshMorphSetParts;
template void vector<TMeshMorphSetParts>::_M_fill_insert(TMeshMorphSetParts*, unsigned int, const TMeshMorphSetParts&);
}