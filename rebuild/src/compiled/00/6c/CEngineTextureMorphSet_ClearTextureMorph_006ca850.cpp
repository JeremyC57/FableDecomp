// CEngineTextureMorphSet::ClearTextureMorph(long originalTextureIndex) -- erase every engine texture morph of the given original texture from Morphs
// Leaf: ?ClearTextureMorph@CEngineTextureMorphSet@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CA850, 93 bytes, thiscall, `ret 4`.
// Identity: the manifest's "std::vector<CEngineTextureMorph*>::erase" is the inlined body. Ego_r.pdb names
//   ?ClearTextureMorph@CEngineTextureMorphSet@@QAEXJ@Z; CEngineTextureMorphSet is { CArray<CEngineTextureMorph> Morphs; }
//   and CEngineTextureMorph { long OriginalTextureIndex; long MorphID; vector<CCompositeBlendDesc> Blends; } (20 bytes
//   in retail with the 12-byte STLport vector). No direct caller survives (reached through the appearance code's
//   inlined copies).
// Callees: __copy(first, last, result, const random_access_iterator_tag&, int*) 0x006CB200 (`__fastcall`; the catalog's
//   "_Move<CEngineTextureMorph*,..>" is the Dinkumware spelling), free 0x00BFEA14 (the erased tail's Blends storage).
// Shape: `for (it = Morphs.begin(); it != Morphs.end(); ) if (it->OriginalTextureIndex == index) it = Morphs.erase(it);
//   else ++it;` with STLport 4.6 erase(iterator) inline (`if (pos + 1 != end()) __copy_ptrs(pos + 1, finish, pos,
//   __false_type()); --finish; _Destroy(finish); return pos;`): VC7.1 strength-reduces pos + 1 into ebx beside edi,
//   the empty random_access_iterator_tag lands on the (live) index argument slot -- which needs the same-TU noinline
//   __copy stand-in that never touches the tag -- and ~CEngineTextureMorph is the Blends vector's free.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

namespace _STL {

struct __false_type { __false_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

// Same-TU stand-in for the out-of-line forward copy 0x006CB200 (its own row): never touches the tag.
#pragma auto_inline(off)
template <class _RandomAccessIter, class _OutputIter, class _Distance>
__declspec(noinline) _OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                                                   const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
#pragma auto_inline(on)

template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    iterator erase(iterator __position)
    {
        if (__position + 1 != end())
            __copy_ptrs(__position + 1, this->_M_finish, __position, __false_type());
        --this->_M_finish;
        _Destroy(this->_M_finish);
        return __position;
    }
};

} // namespace _STL

template <class T> class CArray : public _STL::vector<T> {
};

class CCompositeBlendDesc {                                 // trivially destructible blend record
public:
    long Data[4];
};

class CEngineTextureMorph {
public:
    long                                OriginalTextureIndex;   // +0x00
    long                                MorphID;                // +0x04
    _STL::vector<CCompositeBlendDesc>   Blends;                 // +0x08
};

class CEngineTextureMorphSet {
public:
    CArray<CEngineTextureMorph> Morphs;                     // +0x00
    void ClearTextureMorph(long originalTextureIndex);
};

void CEngineTextureMorphSet::ClearTextureMorph(long originalTextureIndex)
{
    for (CArray<CEngineTextureMorph>::iterator it = Morphs.begin(); it != Morphs.end(); ) {
        if (it->OriginalTextureIndex == originalTextureIndex)
            it = Morphs.erase(it);
        else
            ++it;
    }
}