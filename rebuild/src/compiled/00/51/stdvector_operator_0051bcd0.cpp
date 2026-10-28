// std::vector::operator=(const vector& x) -- STLport assignment of a vector<CNavigationAreaUpdate::CSwitchableBlockage>: reallocate and copy-construct when x is larger than the capacity, else assign over the live elements and destroy the surplus, or assign the overlap and copy-construct the rest
// Leaf: ??4?$vector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0051BCD0, 285 bytes (0x11B + int3 padding), thiscall `ret 4`, returns this.
// Identity: the 16-byte element is Ego_r.pdb CNavigationAreaUpdate::CSwitchableBlockage {unsigned __int64 UniqueID;
//   list<C2DLineF> LineList} (+0xC padding; landed default ctor 0x005155F0); its four callers are the
//   CNavigationLayerUpdate algorithms (0x0051C6F5 in the record __copy, 0x0051C747 copy_backward, 0x0051CABF fill,
//   0x0051CB11 __copy) assigning the record's SwitchableBlockages vector. The catalog label "Vector_ReAllocate" is
//   descriptive.
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (inline allocate / deallocate), __uninitialized_copy<const
//   CSwitchableBlockage*, CSwitchableBlockage*> 0x0051AC00, __destroy_aux 0x00519EF0, __copy(..., const
//   random_access_iterator_tag&, int*) 0x00519FF0 (all __fastcall; same-TU noinline stand-ins here).
// Shape: STLport 4.6 operator= verbatim (`_M_allocate_and_copy` + `_M_clear` in the growth arm, `__copy_ptrs` +
//   `_Destroy` / `__uninitialized_copy` in the others): with same-TU noinline bodies for the three out-of-line
//   helpers every tag temporary shares the dead x argument slot (which also homes x._M_finish) and __copy's null
//   distance pointer is pushed once above the size() >= xlen test, as retail.
extern "C" void* __cdecl malloc(unsigned int size);       // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                    // 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* where) throw() { return where; }

class C2DVector {
public:
    float X;                                   // +0x0
    float Y;                                   // +0x4
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) {}
};
class C2DLineF {
public:
    C2DVector Point1;                          // +0x0
    C2DVector Point2;                          // +0x8
};

namespace std {
struct __false_type { __false_type() {} };
struct random_access_iterator_tag { random_access_iterator_tag() {} };
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class T> inline void _Destroy(T* p) { p->~T(); }

template <class T> class allocator {
public:
    allocator() {}
    allocator(const allocator&) {}
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    void deallocate(T* p, unsigned int) { if (p != 0) free(p); }
};
template <class V, class T, class A> class _STLP_alloc_proxy : public A {
public:
    V _M_data;
    _STLP_alloc_proxy(const A& a, V p) : A(a), _M_data(p) {}
};

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class T> class list {
public:
    _List_node_base* _M_node;
    list(const list<T>& x);                     // out of line (0x005152C0-based)
    list<T>& operator=(const list<T>& x);       // 0x00517720
    ~list();                                    // out of line (the inline clear + free of 0x00519EF0)
};
}

class CNavigationAreaUpdate {
public:
    class CSwitchableBlockage {
    public:
        unsigned __int64       UniqueID;        // +0x00
        std::list<C2DLineF>   LineList;        // +0x08
    };
};
typedef CNavigationAreaUpdate::CSwitchableBlockage CSwitchableBlockage;

namespace std {
__declspec(noinline) CSwitchableBlockage* __fastcall __uninitialized_copy(const CSwitchableBlockage* first,
    const CSwitchableBlockage* last, CSwitchableBlockage* result, const __false_type&)   // 0x0051AC00
{
    CSwitchableBlockage* cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}
__declspec(noinline) CSwitchableBlockage* __fastcall __copy(const CSwitchableBlockage* first,
    const CSwitchableBlockage* last, CSwitchableBlockage* result, const random_access_iterator_tag&, int*)   // 0x00519FF0
{
    for (int n = int(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}
__declspec(noinline) void __fastcall __destroy_aux(CSwitchableBlockage* first, CSwitchableBlockage* last,
    const __false_type&)   // 0x00519EF0
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
inline void _Destroy(CSwitchableBlockage* first, CSwitchableBlockage* last) { __destroy_aux(first, last, __false_type()); }
inline CSwitchableBlockage* __copy_ptrs(const CSwitchableBlockage* first, const CSwitchableBlockage* last,
    CSwitchableBlockage* result, const __false_type&)
{
    return __copy(first, last, result, random_access_iterator_tag(), (int*)0);
}

template <class T, class A> class _Vector_base {
public:
    _Vector_base(const A& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0) {}
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, T, A> _M_end_of_storage;
};

template <class T, class A = allocator<T> > class vector : public _Vector_base<T, A> {
public:
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef unsigned int size_type;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    protected:
    pointer _M_allocate_and_copy(size_type n, const_pointer first, const_pointer last)
    {
        pointer result = this->_M_end_of_storage.allocate(n);
        __uninitialized_copy(first, last, result, __false_type());
        return result;
    }
    public:
    protected:
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    public:
    vector<T, A>& operator=(const vector<T, A>& x);
};

template <class T, class A>
vector<T, A>& vector<T, A>::operator=(const vector<T, A>& x)
{
    if (&x != this) {
        const size_type xlen = x.size();
        if (xlen > capacity()) {
            pointer tmp = _M_allocate_and_copy(xlen, (const_pointer)x._M_start + 0, (const_pointer)x._M_finish + 0);
            _M_clear();
            this->_M_start = tmp;
            this->_M_end_of_storage._M_data = this->_M_start + xlen;
        }
        else if (size() >= xlen) {
            pointer i = __copy_ptrs((const_pointer)x._M_start + 0, (const_pointer)x._M_finish + 0,
                                    (pointer)this->_M_start, __false_type());
            _Destroy(i, this->_M_finish);
        }
        else {
            __copy_ptrs((const_pointer)x._M_start, (const_pointer)x._M_start + size(), (pointer)this->_M_start,
                        __false_type());
            __uninitialized_copy((const_pointer)x._M_start + size(), (const_pointer)x._M_finish + 0, this->_M_finish,
                                 __false_type());
        }
        this->_M_finish = this->_M_start + xlen;
    }
    return *this;
}
}

template std::vector<CSwitchableBlockage>& std::vector<CSwitchableBlockage>::operator=(
    const std::vector<CSwitchableBlockage>&);