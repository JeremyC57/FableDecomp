// CRegionConnectionNode::SetAsConnectedToRegion(long region, bool connected) -- add the region to (or remove it from) this node's connection list
// Leaf: ?SetAsConnectedToRegion@CRegionConnectionNode
// Retail: Fable.exe (TLC, VC7.1) @ 0x008285D0, 195 bytes (+13 int3), thiscall `ret 8`.
// Identity: Ego_r.pdb name set ?SetAsConnectedToRegion@CRegionConnectionNode@@QAEXJ_N@Z; the caller 0x008286A0 is
//   CRegionConnectionGraph::SetRegionsAsConnected (QAEXJJ_N). The catalog label "StdVector_RemoveIf" is descriptive.
// Layout: Ego_r.pdb CRegionConnectionNode (28 bytes): RegionID +0, Connections (STLport vector<long>, 12 bytes) +4,
//   MostRecentSearchID +0x10, FirstNodeInSearch +0x14.
// Callees: STLport find(first, last, const long&, const random_access_iterator_tag&) @ 0x0050E540 (__fastcall),
//   vector<long>::_M_insert_overflow @ 0x00440990, memmove @ 0x00BFEAE6 (the inline erase's __copy_trivial).
// Shape: `if (connected) { if (find(...) == end()) push_back(region); } else erase(remove(begin(), end(), region), end());`
//   -- STLport remove (find, then the inline remove_copy loop) and the range erase (__copy_trivial memmove, finish
//   stored). Both finds share the tag/value pushes; the tag is homed on the `connected` argument slot. Same-TU noinline
//   stand-ins for the find and the overflow insert as the landed CTCHeroReceiveItems::SetItemAsPickedUp 0x007CB850.
#include <string.h>

inline void* operator new(unsigned int, void* p) { return p; }

namespace _STL {
struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct __true_type { __true_type() { } };

template <class _RandomAccessIter, class _Tp>
__declspec(noinline) _RandomAccessIter __fastcall find(_RandomAccessIter __first, _RandomAccessIter __last,
                                                      const _Tp& __val, const random_access_iterator_tag&)   // 0x0050E540
{
    for (; __first != __last; ++__first)
        if (*__first == __val)
            return __first;
    return __last;
}

template <class _InputIter, class _Tp>
inline _InputIter find(_InputIter __first, _InputIter __last, const _Tp& __val)
{
    return find(__first, __last, __val, random_access_iterator_tag());
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
    return (__last == __first) ? __result
        : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first)))
              + ((const char*)__last - (const char*)__first);
}

template <class T> class vector {
public:
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    T* _M_end_of_storage;                                   // +0x08
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    __declspec(noinline) void _M_insert_overflow(T* __position, const T& __x, const __true_type&, unsigned int __fill_len, bool __atend);   // 0x00440990
    void push_back(const T& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage) {
            new (this->_M_finish) T(__x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __true_type(), 1UL, true);
    }
    T* erase(T* __first, T* __last)
    {
        T* __i = (T*)__copy_trivial(__last, this->_M_finish, __first);
        this->_M_finish = __i;
        return __first;
    }
};

template <class T>
void vector<T>::_M_insert_overflow(T* __position, const T& __x, const __true_type&, unsigned int __fill_len, bool __atend)
{
    if (__x == 0 && __atend)
        this->_M_finish = __position + __fill_len;
}
}

class CRegionConnectionNode {
public:
    long               RegionID;                            // +0x00
    _STL::vector<long> Connections;                         // +0x04
    long               MostRecentSearchID;                  // +0x10
    long               FirstNodeInSearch;                   // +0x14
    void SetAsConnectedToRegion(long region, bool connected);
};

void CRegionConnectionNode::SetAsConnectedToRegion(long region, bool connected)
{
    if (connected) {
        if (_STL::find(Connections.begin(), Connections.end(), region) == Connections.end())
            Connections.push_back(region);
    } else
        Connections.erase(_STL::remove(Connections.begin(), Connections.end(), region), Connections.end());
}