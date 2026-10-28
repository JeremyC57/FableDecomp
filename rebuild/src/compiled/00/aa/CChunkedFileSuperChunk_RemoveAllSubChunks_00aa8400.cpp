// CChunkedFileSuperChunk::RemoveAllSubChunks() -- unparent every sub-chunk, then clear the SubChunks array
// Leaf: ?RemoveAllSubChunks@CChunkedFileSuperChunk@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AA8400, 89 bytes, thiscall, plain `ret`; /Gr TU (the STLport helpers are __fastcall).
// Flags: /Gr
// Identity (proven): Ego_r.pdb ?RemoveAllSubChunks@CChunkedFileSuperChunk@@QAEXXZ (the ghidra_out PDB symbol table names
//   0x00AA8400 with it); called first by the landed ~CChunkedFileSuperChunk 0x00AA8490. SubChunks +0x10 is
//   CArray<CCountedPointer<CChunkedFileChunk> > (8-byte elements), CChunkedFileChunk::Parent +0x0C.
// Callees: STLport __copy<CCountedPointer<CChunkedFileChunk>*> 0x00AA8F40 and __destroy_aux 0x00AA9140, both under
//   their bound std spellings (callers 0x00AA91F0 / 0x00AA94E0).
// Shape: `for (i < SubChunks.size()) SubChunks[i]->Parent = 0; SubChunks.clear();` with the STLport clear() =
//   erase(begin(), end()) = copy(end, finish, begin) + _Destroy(i, finish) + finish = i; the two empty tag
//   temporaries share the high byte of the `push ecx` slot.
#include "engine/CChunkedFileChunk.h"

class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;                              // +0x00
    CCPPointerInfo* Info;                              // +0x04
    T* operator->() const { return Data; }
};

namespace std {
struct __false_type { __false_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                   const random_access_iterator_tag&, _Distance*);                      // 0x00AA8F40
template <class _ForwardIterator>
void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x00AA9140

template <class _Tp> inline _Tp* copy(_Tp* __first, _Tp* __last, _Tp* __result)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    _Tp& operator[](size_type __n) { return *(begin() + __n); }
    iterator erase(iterator __first, iterator __last)
    {
        iterator __i = copy(__last, this->_M_finish, __first);
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void clear() { erase(begin(), end()); }
};
}

template <class T> class CArray : public std::vector<T> { };

// engine/CChunkedFileSuperChunk.h types SubChunks as opaque bytes; the retail layout is the same (+0x10, 12 bytes).
class CChunkedFileSuperChunk : public CChunkedFileChunk {
public:
    CArray<CCountedPointer<CChunkedFileChunk> > SubChunks;                  // +0x10
    CArray<CCountedPointer<CChunkedFileChunk> > RegisteredSubChunkTypes;    // +0x1c
    unsigned long                               CurrentChunkIndex;          // +0x28
    void RemoveAllSubChunks();
};
FABLE_STATIC_ASSERT(sizeof(CChunkedFileSuperChunk) == 0x2c);

void CChunkedFileSuperChunk::RemoveAllSubChunks()
{
    for (unsigned long i = 0; i < SubChunks.size(); ++i)
        SubChunks[i]->Parent = 0;
    SubChunks.clear();
}