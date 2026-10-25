// CRegion::RemoveMap(long map) -- drop a map id from the region's map list: STLport `Maps.erase(remove(Maps.begin(), Maps.end(), map))` (one-element erase of the remove result)
// Leaf: ?RemoveMap@CRegion@@QAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BC160, 99 bytes (+13 int3 padding), thiscall, `ret 4`, no return value.
// Identity: the manifest's "std::vector<int>::erase" is descriptive (no callers: the PDB-public CRegion::RemoveMap is
//   reached only through data / other TUs). Ego_r.pdb ?RemoveMap@CRegion@@QAEXJ@Z; CRegion = { vector<long> Maps +0;
//   vector<long> VisMaps; CCharString Name; ... } and the neighbours are CRegion members (0x006BC290 transfers the three
//   bools at retail +0x54/+0x55/+0x56 = PDB AppearOnWorldMap.. with 12-byte STLport vectors/map; 0x006BC350
//   GetMiniMapRegionExitTextOffset).
// Callees: STLport __find(first, last, const long&, const random_access_iterator_tag&) 0x0050E540 (`__fastcall`, landed
//   global_Find0050e540, `ret 8`), memmove 0x00BFEAE6.
// Shape: `remove(first, last, v)` = `first = find(first, last, v); if (first == last) return first;
//   return remove_copy(++next, last, first, v);` with find's random_access tag temporary placed at the live `map`
//   argument slot (both pushes are `lea [map]`; the find body sits above under auto_inline(off) so VC7.1 sees it does
//   not write `map`), remove_copy inline (value re-read into ebx after the call), then the STLport single erase
//   `if (pos + 1 != end()) __copy_trivial(pos + 1, finish, pos); --finish;` -- the copy's own `last == first` test folds
//   into the end() compare, leaving one memmove and `add [edi+4],-4`. The source erases only the first slot of the
//   removed tail (the retail behaviour: duplicates past the first leave stale elements).
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size);   // 0x00BFEAE6

namespace _STL {

struct __true_type { __true_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> inline random_access_iterator_tag __iterator_category(const _Tp*)
{
    return random_access_iterator_tag();
}

// The landed 0x0050E540 body (same TU, kept out of line).
#pragma auto_inline(off)
template <class _RandomAccessIter, class _Tp>
_RandomAccessIter __fastcall __find(_RandomAccessIter __first, _RandomAccessIter __last, const _Tp& __val,
                                    const random_access_iterator_tag&)
{
    int __trip_count = (__last - __first) >> 2;
    for (; __trip_count > 0; --__trip_count) {
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
    }
    switch (__last - __first) {
    case 3:
        if (*__first == __val) return __first;
        ++__first;
    case 2:
        if (*__first == __val) return __first;
        ++__first;
    case 1:
        if (*__first == __val) return __first;
        ++__first;
    case 0:
    default:
        return __last;
    }
}
#pragma auto_inline(on)

template <class _InputIter, class _Tp>
inline _InputIter find(_InputIter __first, _InputIter __last, const _Tp& __val)
{
    return __find(__first, __last, __val, __iterator_category(__first));
}

template <class _InputIter, class _OutputIter, class _Tp>
inline _OutputIter remove_copy(_InputIter __first, _InputIter __last, _OutputIter __result, const _Tp& __val)
{
    for (; __first != __last; ++__first)
        if (!(*__first == __val)) {
            *__result = *__first;
            ++__result;
        }
    return __result;
}

template <class _ForwardIter, class _Tp>
inline _ForwardIter remove(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    __first = find(__first, __last, __val);
    if (__first == __last)
        return __first;
    else {
        _ForwardIter __next = __first;
        return remove_copy(++__next, __last, __first, __val);
    }
}

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}

template <class _Tp> inline _Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __true_type&)
{
    return (_Tp*)__copy_trivial(__first, __last, __result);
}

template <class _Tp> inline void _Destroy(_Tp*) { }

template <class _Tp> class allocator {
public:
    allocator() { }
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
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    iterator erase(iterator __position)
    {
        if (__position + 1 != end())
            __copy_ptrs(__position + 1, this->_M_finish, __position, __true_type());
        --this->_M_finish;
        _Destroy(this->_M_finish);
        return __position;
    }
};

} // namespace _STL

class CRegion {
public:
    _STL::vector<long> Maps;                                // +0x00
    _STL::vector<long> VisMaps;                             // +0x0C
    void RemoveMap(long map);
};

void CRegion::RemoveMap(long map)
{
    Maps.erase(_STL::remove(Maps.begin(), Maps.end(), map));
}