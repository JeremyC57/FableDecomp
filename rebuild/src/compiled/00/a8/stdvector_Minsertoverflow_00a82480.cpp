// std::vector::_M_insert_overflow(pair<unsigned __int64,CANavQuadTreeNode*>* position, const pair<unsigned __int64,CANavQuadTreeNode*>& x, const __false_type&, unsigned int fill_len, bool atend) -- grow CNavQuadTree's switchable-node map vector and insert fill_len copies of x
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A82480, 252 bytes, thiscall `ret 14h`.
// Identity: element 16 bytes (`sar eax,4`), copied as three dwords -- pair<unsigned __int64, CANavQuadTreeNode*> (8 + 4,
//   padded to the __int64 alignment): Ego_r.pdb CNavQuadTree::SwitchableNodes is CVectorMap<unsigned __int64,
//   CANavQuadTreeNode*>, whose vector this is (callers: CNavQuadTree::LoadFromFile's push_back 0x00A837E0, the insert
//   at 0x00A82EC8 and 0x00A7B61D). The catalog's "Vector_InsertN_16ByteElements" is descriptive.
// Callees: malloc 0x00BFEA0E, free 0x00BFEA14, the tail __uninitialized_copy (out of line, position still in ecx).
// Shape: the landed vector<pair<CSkeletalMorphs::CEntry, float> >::_M_insert_overflow 0x00720F40 with a 16-byte
//   element: the head __uninitialized_copy and the fill inline (placement copies with null tests), STLport pair's user copy
//   constructor copying the __int64 key and the pointer.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);           // 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

class CANavQuadTreeNode;   // Ego_r.pdb: class (mangled PAVCANavQuadTreeNode)
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

template class vector<pair<unsigned __int64, CANavQuadTreeNode*> >;

}