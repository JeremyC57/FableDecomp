// std::vector::_M_fill_insert(pair<EBodyLocation,CWoundMorphs::CEntry>* position, unsigned int n, const pair<EBodyLocation,CWoundMorphs::CEntry>& x) -- insert n copies of a wound-morph map entry: slide the tail inside the spare capacity, or grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00759290, 294 bytes, thiscall `ret 0Ch`.
// Label correction: catalog "Vector_Insert_Range_Shift" -> STLport 4.x
//   vector<pair<EBodyLocation, CWoundMorphs::CEntry> >::_M_fill_insert (the landed fill 0x00758220 names this caller;
//   reached by a tail jump from insert(pos, n, x) 0x00759520 and from 0x00759648).
// Callees: __uninitialized_copy 0x007586E0, __copy_backward_ptrs 0x007587A0, fill 0x00758220, uninitialized_fill_n
//   0x007587E0, _M_insert_overflow 0x00759190 (all out of line in this TU).
// Shape: STLport 4.6 `_M_fill_insert` verbatim: `if (n != 0)`; spare capacity >= n: the 44-byte x_copy (pair's user
//   copy constructor: the key, then a 10-dword `rep movsd`), elems_after = finish - position; elems_after > n:
//   copy the last n up, finish += n, copy_backward the middle, fill the gap; else fill_n past the end, copy the tail
//   up, fill the old range; else _M_insert_overflow(position, x, __false_type(), n). The empty tags are homed on
//   position's (live) argument slot: the tag-taking callees are same-TU noinline stand-ins that never touch the tag.
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

enum EBodyLocation { EBodyLocation_FIRST };

class CWoundMorphs {
public:
    class CEntry {                              // Ego_r.pdb, 40 bytes
    public:
        EBodyLocation Location;                 // +0x00
        long  ReplacingTextureIndex;            // +0x04
        long  ReplacingBumpMapIndex;            // +0x08
        long  WoundTextureIndex;                // +0x0c
        long  WoundBumpMapIndex;                // +0x10
        long  ScarTextureIndex;                 // +0x14
        long  ScarBumpMapIndex;                 // +0x18
        long  FadeToScarDuration;               // +0x1c
        long  TotalDuration;                    // +0x20
        float PermanentScarStrength;            // +0x24
    };
};

namespace std {

struct __false_type { __false_type() {} };
struct random_access_iterator_tag { random_access_iterator_tag() {} };

template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair& o) : first(o.first), second(o.second) {}
};

template<class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

#pragma auto_inline(off)
// Same-TU stand-ins (real bodies: 0x007586E0, 0x007587A0, 0x00758220, 0x007587E0).
template<class T> __declspec(noinline) T* __fastcall __uninitialized_copy(const T* first, const T* last, T* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        _Construct(result, *first);
    return result;
}
template<class T> __declspec(noinline) T* __fastcall __copy_backward_ptrs(T* first, T* last, T* result, const __false_type&)
{
    for (int n = int(last - first); n > 0; --n)
        *--result = *--last;
    return result;
}
template<class T> __declspec(noinline) void __fastcall fill(T* first, T* last, const T& val)
{
    for (; first != last; ++first)
        *first = val;
}
template<class T> __declspec(noinline) T* __fastcall uninitialized_fill_n(T* first, unsigned int n, const T& x)
{
    for (; n > 0; --n, ++first)
        _Construct(first, x);
    return first;
}
#pragma auto_inline(on)

template<class T> class vector {
public:
    T* _M_start;                                // +0x00
    T* _M_finish;                               // +0x04
    T* _M_end_of_storage;                       // +0x08
#pragma auto_inline(off)
    // 0x00759190 (stand-in)
    __declspec(noinline) void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend = false)
    {
        if (atend && fill_len == 1)
            _Construct(position, x);
    }
#pragma auto_inline(on)
    void _M_fill_insert(T* __position, unsigned int __n, const T& __x);
};

template<class T>
void vector<T>::_M_fill_insert(T* __position, unsigned int __n, const T& __x)
{
    if (__n != 0) {
        if ((unsigned int)(this->_M_end_of_storage - this->_M_finish) >= __n) {
            T __x_copy = __x;
            const unsigned int __elems_after = this->_M_finish - __position;
            T* __old_finish = this->_M_finish;
            if (__elems_after > __n) {
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, __false_type());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, __false_type());
                fill(__position, __position + __n, __x_copy);
            }
            else {
                uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, __false_type());
                this->_M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        }
        else
            _M_insert_overflow(__position, __x, __false_type(), __n);
    }
}

template class vector<pair<EBodyLocation, CWoundMorphs::CEntry> >;

}