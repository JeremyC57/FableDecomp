// std::vector::erase(iterator _Where) -- VC7.1 Dinkumware vector<PKCRequest*>::erase of one element: memmove the tail down one slot, drop the last element, return _Where
// Grade: RELOCATION_MATCH -- the raw-pointer copy body `copy(_Where.base() + 1, _Mylast, _Where.base())` of its byte-identical twin 0x00C27020 (regraded at landing)
// Leaf: ?erase@?$vector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C26BC0, 49 bytes (`ret 8`: the iterator is a class, returned through the hidden
//   pointer). Descriptive element type (no PDB covers the libpk audio library); the catalog's
//   "Container_RemoveAtIndex" is propagated. Only caller: the worker thread's ProcessNextRequest
//   (0x00C26B7A, `m_pRequests->erase(m_pRequests->begin())`); this library uses VC7.1's own Dinkumware STL
//   (16-byte vectors: allocator word, _Myfirst +4, _Mylast +8, _Myend +0xc), not STLport.
// Callee: MSVCR71 memmove (import 0x01440144) -- the scalar-pointer copy; _Destroy of a pointer is empty and
//   `--_Mylast` is `add dword ptr [esi+8], -4`.
// Shape: the tail copy runs on raw pointers (`copy(_Where.base() + 1, _Mylast, _Where.base())`, the scalar memmove):
//   retail forms `_Where + 1` in eax and _Mylast in ecx (`sub ecx,eax`). Copying through the iterators
//   (`copy(_Where + 1, end(), _Where)`, with or without tag dispatch, unwrapping overloads or /Gr) swaps the pair.
extern "C" __declspec(dllimport) void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x01440144

namespace std {
template<class _Ty> class allocator { };

template<class _Ty> class _Ptrit {              // VC7.1 Dinkumware vector iterator: a class over one pointer
public:
    _Ptrit() { }
    explicit _Ptrit(_Ty* _Ptr) : current(_Ptr) { }
    _Ty& operator*() const { return (*current); }
    _Ty* base() const { return (current); }
    _Ty* current;
};

template<class _Ty> inline
_Ty* copy(_Ty* _First, _Ty* _Last, _Ty* _Dest)
{                                               // the scalar-pointer copy: one memmove
    int _Off = _Last - _First;
    return ((_Ty*)memmove(_Dest, _First, _Off * sizeof(*_First)) + _Off);
}

template<class _Ty> inline
void _Destroy(_Ty*, _Ty*)
{
}

template<class _Ty, class _Ax = allocator<_Ty> > class vector {
public:
    typedef _Ptrit<_Ty> iterator;
    unsigned int size() const { return (_Myfirst == 0 ? 0 : _Mylast - _Myfirst); }
    bool empty() const { return (size() == 0); }
    iterator begin() { return (iterator(_Myfirst)); }
    iterator end() { return (iterator(_Mylast)); }
    _Ty& front() { return (*begin()); }
    iterator erase(iterator _Where)
    {
        copy(_Where.base() + 1, _Mylast, _Where.base());
        _Destroy(_Mylast - 1, _Mylast);
        --_Mylast;
        return (_Where);
    }
    _Ax  _Alval;                                // +0x00 (an empty allocator padded to a word)
    _Ty* _Myfirst;                              // +0x04
    _Ty* _Mylast;                               // +0x08
    _Ty* _Myend;                                // +0x0c
};
}

struct PKCRequest;
template class std::vector<PKCRequest*>;