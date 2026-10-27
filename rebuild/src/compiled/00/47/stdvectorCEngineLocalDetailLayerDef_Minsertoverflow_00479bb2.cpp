// std::vector<CEngineLocalDetailLayerDef>::_M_insert_overflow(CEngineLocalDetailLayerDef* position, const CEngineLocalDetailLayerDef& x, const __false_type&, unsigned int fillLength, bool atEnd) -- STLport growth path: allocate old+max(old,n), copy-construct the head, n copies of x and the tail unless inserting at the end, clear the old block, adopt the new one
// Leaf: ?_M_insert_overflow@?$vector@VCEngineLocalDetailLayerDef@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00479BB2, 226 bytes, thiscall `ret 14h`; size TU with a frame pointer.
// Identity (proven): every placement copy calls 0x004773F1, bound ??0CEngineLocalDetailLayerDef@@QAE@ABV0@@Z (the CEngineLocalDetailLayerDef copy constructor, 0x54-byte
//   stride); the old block goes through _M_clear 0x004773CE; its only caller is the vector's _M_fill_insert 0x00479A5E (resize 0x004799C6).
// Callees: malloc 0x00BFEA0E, CEngineLocalDetailLayerDef::CEngineLocalDetailLayerDef(const CEngineLocalDetailLayerDef&) 0x004773F1, vector<CEngineLocalDetailLayerDef>::_M_clear() 0x004773CE.
// Shape (family K102, representative 0x00476816): the landed 0x0046AECC _M_insert_overflow (K100) with a non-trivial element:
//   the null-checked placement copies call the out-of-line copy constructor and _M_clear (destroy + free) is out of line.
#pragma optimize("y", off)
#pragma optimize("s", on)
extern "C" void* __cdecl malloc(unsigned int bytes);                                   // 0x00BFEA0E

inline void* operator new(unsigned int, void* where) { return where; }

class CEngineLocalDetailLayerDef {                                            // retail 0x54 bytes (members not touched here)
public:
    CEngineLocalDetailLayerDef(const CEngineLocalDetailLayerDef& other);                           // 0x004773F1
    unsigned char _data[0x54];
};

namespace std {
struct __false_type { };
template <class T> __forceinline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }
template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class T> __forceinline T* __uninitialized_copy(T* first, T* last, T* result, const __false_type&)
{
    T* cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(cur, *first);
    return cur;
}
template <class T> __forceinline T* __uninitialized_fill_n(T* first, unsigned int n, const T& x, const __false_type&)
{
    T* cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(cur, x);
    return cur;
}
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;                    // +0x00
    T* _M_finish;                   // +0x04
    T* _M_end_of_storage;           // +0x08
    unsigned int size() const { return _M_finish - _M_start; }
    static __forceinline T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    void _M_clear();                                    // 0x004773CE
    __forceinline void _M_set(T* s, T* f, T* e) { _M_start = s; _M_finish = f; _M_end_of_storage = e; }
protected:
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fillLength, bool atEnd);
};
template <class T, class A> void vector<T, A>::_M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fillLength, bool atEnd)
{
    const unsigned int oldSize = size();
    const unsigned int length = oldSize + (max)(oldSize, fillLength);
    T* newStart = allocate(length);
    T* newFinish = __uninitialized_copy(_M_start, position, newStart, __false_type());
    if (fillLength == 1) {
        _Construct(newFinish, x);
        ++newFinish;
    }
    else
        newFinish = __uninitialized_fill_n(newFinish, fillLength, x, __false_type());
    if (!atEnd)
        newFinish = __uninitialized_copy(position, _M_finish, newFinish, __false_type());
    _M_clear();
    _M_set(newStart, newFinish, newStart + length);
}
template void vector<CEngineLocalDetailLayerDef >::_M_insert_overflow(CEngineLocalDetailLayerDef*, const CEngineLocalDetailLayerDef&, const __false_type&, unsigned int, bool);
}