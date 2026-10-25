// std::vector::_M_insert_overflow(pair<CEntry,float>* position, const pair<CEntry,float>& x, const __false_type&, unsigned int fill_len, bool atend) -- grow the weighted skeletal-morph vector and insert fill_len copies of x
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00720F40, 270 bytes, `ret 0x14`.
// Label correction: catalog "Vector_InsertAndReplaceMulti_12byte" -> STLport 4.x
//   vector<pair<CSkeletalMorphs::CEntry, float> >::_M_insert_overflow (called from the landed
//   `anonymous namespace'::AddMorph 0x0071CF70 and the inline push_back 0x007216F0; 12 bytes: 0x2AAAAAAB / `sar edx,1`).
// Element: pair<CEntry {CDefString MorphName; short Priority; bool SupressesLowerMorphs;}, float> -- trivially
//   destructible, so _M_clear is only the free.
// Callees: malloc 0x00BFEA0E, free 0x00BFEA14, the tail __uninitialized_copy 0x0071F180.
// Shape: STLport `_M_insert_overflow` with inline helpers: VC7.1 inlines the head __uninitialized_copy and the
//   __uninitialized_fill_n (placement copies with their null tests, three dword moves each) but leaves the tail copy
//   out of line (`call 0x0071F180` with position still in ecx).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);           // 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

class CDefString {
public:
    long TablePos;                              // +0x00
};

class CSkeletalMorphs {
public:
    class CEntry {                              // Ego_r.pdb, 8 bytes
    public:
        CDefString MorphName;                   // +0x00
        short Priority;                         // +0x04
        bool SupressesLowerMorphs;              // +0x06
    };
};

namespace std {

struct __false_type { __false_type() {} };

template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) {}
    pair(const pair& o) : first(o.first), second(o.second) {}
};

template<class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }

template<class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template<class T> class allocator {
public:
    allocator() {}
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    void deallocate(T* p, unsigned int) { if (p) free(p); }
};

template<class InputIter, class ForwardIter>
inline ForwardIter __fastcall __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}
template<class ForwardIter, class Size, class T>
inline ForwardIter __fastcall __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}

template<class T, class Alloc = allocator<T> > class vector {
public:
    T* _M_start;                                // +0x00
    T* _M_finish;                               // +0x04
    T* _M_end_of_storage;                       // +0x08
    Alloc _M_alloc() const { return Alloc(); }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_clear()
    {
        _M_alloc().deallocate(_M_start, (unsigned int)(_M_end_of_storage - _M_start));
    }
    void _M_set(T* s, T* f, T* e)
    {
        _M_start = s;
        _M_finish = f;
        _M_end_of_storage = e;
    }
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend);
};

template<class T, class Alloc>
void vector<T, Alloc>::_M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend)
{
    const unsigned int old_size = size();
    const unsigned int len = old_size + (max)(old_size, fill_len);

    T* new_start = _M_alloc().allocate(len);
    T* new_finish = __uninitialized_copy(_M_start, position, new_start, __false_type());
    if (fill_len == 1) {
        _Construct(new_finish, x);
        ++new_finish;
    }
    else
        new_finish = __uninitialized_fill_n(new_finish, fill_len, x, __false_type());
    if (!atend)
        new_finish = __uninitialized_copy(position, _M_finish, new_finish, __false_type());
    _M_clear();
    _M_set(new_start, new_finish, new_start + len);
}

template class vector<pair<CSkeletalMorphs::CEntry, float> >;

}