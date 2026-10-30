// std::vector::insert(iterator position, const value_type& x) -- (vector<pair<unsigned long, CCountedPointer<CVertexBufferWin32> > >) STLport single-element insert: construct at the end when position is end(), else copy the last element up one, shift [position, finish - 2) back by one and assign a copy of x at position; with no spare capacity grow through _M_insert_overflow; returns begin() + the original index
// Leaf: ?insert@?$vector@U?$pair@KV?$CCountedPointer@VCVertexBufferWin32@@@@@std@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B8F950, 293 bytes (+ int3 padding), thiscall, `ret 8`.
// Identity (proven): the landed _M_insert_overflow 0x00B8F840 and __copy_backward 0x00B8EF60 are this vector's
//   (vector<pair<unsigned long, CCountedPointer<CVertexBufferWin32> > >, CEngineOcclusionManager::FlareVertexBuffers;
//   both rows name this function as their caller); the only caller is CEngineOcclusionManager::SelectFlareTestVertexBuffer
//   0x00B8DCD0 (call 0x00B8DEED). STLport 4.6 vector.h `insert(iterator, const _Tp&)` verbatim. The manifest label
//   "Vector_InsertWithMove_Element" is descriptive.
// Callees: __copy_backward 0x00B8EF60 (via the inline __copy_backward_ptrs: tag homed in the dead x slot, null distance),
//   pair::operator= 0x00B8EC90 (the implicit copy assignment, here out of line; its only caller is this function),
//   _M_insert_overflow 0x00B8F840 (tag homed in the live x slot), operator delete 0x00BFE9BC and the count block's
//   DeleteFunc (`call [Info+4]`, Data in ecx) from x_copy's inline ~CCountedPointer.
// Shape: the tag-taking callees are same-TU bodies that never read their tags (the live-slot tag rule); x_copy is the
//   12-byte frame; both placement constructions keep VC7.1's null test.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
void __cdecl operator delete(void* p);                      // 0x00BFE9BC
inline void* operator new(unsigned int, void* p) { return p; }

class CCPPointerInfo {                                      // 12 bytes
public:
    unsigned long RefCount;                                 // +0x00
    void (__fastcall* DeleteFunc)(void* data);              // +0x04, Data in ecx
    void* Data;                                             // +0x08
};
template <class T> class CCountedPointer {                  // 8 bytes
public:
    T*              Data;                                   // +0x00
    CCPPointerInfo* Info;                                   // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer() { ReleaseData(); Data = 0; Info = 0; }
    CCountedPointer& operator=(const CCountedPointer& other) { ShareData(other.Data, other.Info); return *this; }
    void ReleaseData()
    {
        if (Info) {
            if (--Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    void ShareData(T* data, CCPPointerInfo* info)
    {
        if (Info != info) {
            ReleaseData();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
    }
};
class CVertexBufferWin32;

namespace std {

struct __false_type { __false_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};

template <class _T1, class _T2> inline void _Copy_Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                             const __false_type&);                             // 0x00B8EDE0
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                               const __false_type&);                           // 0x00B8EE20
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x00B8F470
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last) { __destroy_aux(__first, __last, __false_type()); }

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                                                   _BidirectionalIter __result,
                                                                   const random_access_iterator_tag&, _Distance*)
{                                                                                              // 0x00B8EF60
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __fastcall __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                           _BidirectionalIter2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
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
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
protected:
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    __declspec(noinline) void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                                                 size_type __fill_len, bool __atend = false)
    {                                                                                          // 0x00B8F840
        const size_type __old_size = size();
        const size_type __len = __old_size + (max)(__old_size, __fill_len);
        pointer __new_start = this->_M_end_of_storage.allocate(__len);
        pointer __new_finish = __new_start;
        __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
        if (__fill_len == 1) {
            _Copy_Construct(__new_finish, __x);
            ++__new_finish;
        } else
            __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
        if (!__atend)
            __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
        _M_clear();
        _M_set(__new_start, __new_finish, __new_start + __len);
    }
public:
    iterator insert(iterator __position, const _Tp& __x);
};

template <class _Tp, class _Alloc>
typename vector<_Tp, _Alloc>::iterator vector<_Tp, _Alloc>::insert(iterator __position, const _Tp& __x)
{
    size_type __n = __position - begin();
    if (this->_M_finish != this->_M_end_of_storage._M_data) {
        if (__position == end()) {
            _Copy_Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else {
            _Copy_Construct(this->_M_finish, *(this->_M_finish - 1));
            ++this->_M_finish;
            _Tp __x_copy = __x;
            __copy_backward_ptrs(__position, this->_M_finish - 2, this->_M_finish - 1, __false_type());
            *__position = __x_copy;
        }
    }
    else
        _M_insert_overflow(__position, __x, __false_type(), 1UL);
    return begin() + __n;
}

typedef pair<unsigned long, CCountedPointer<CVertexBufferWin32> > _Entry;
template vector<_Entry>::iterator vector<_Entry>::insert(_Entry*, const _Entry&);

} // namespace std