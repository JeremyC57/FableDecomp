// _global::__copy(const CDefPointer<const CThingBaseDef>* first, const CDefPointer<const CThingBaseDef>* last, back_insert_iterator result, const random_access_iterator_tag&, int*) -- STLport random-access copy of tattoo def pointers into a back inserter: push_back each def (add-ref'd in place, or through the growth path), return the inserter
// Leaf: __copy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00721F70, 111 bytes (112 with padding), `ret 10h`.
// Label correction: the catalog "Tattoo_CopyToVector" is descriptive; the body is STLport `__copy(first, last, result,
//   const random_access_iterator_tag&, _Distance*)` instantiated for CDefPointer<const CThingBaseDef> into a
//   back_insert_iterator<vector<...> > (callers 0x0071DE75 Tattoo_ApplyChanges, 0x00722056, 0x007220A6, 0x007682F2).
// Callee: vector::_M_insert_overflow(pos, x, const __false_type&, fill_len, atend) @ 0x005B3275 (thiscall, `ret 14h`).
// Convention: /Gr (link-time __fastcall) returning the inserter by value: back_insert_iterator has a user constructor,
//   so it comes back through the hidden pointer in ecx (ebp); first in edx, then last, the 4-byte inserter by value,
//   the tag reference and the null distance pointer on the stack.
// Shape: `for (n = last - first; n > 0; --n) { *result = *first; ++first; ++result; } return result;` with the
//   inserter's operator= / push_back inlined (as 0x00721D30): the container stays in edi, the down-counter in ebx,
//   the growth call's empty tag on the inserter's argument slot; the n <= 0 exit copies the inserter straight from
//   its slot.
inline void* operator new(unsigned int, void* p) { return p; }
struct __false_type { __false_type() {} };
struct random_access_iterator_tag { random_access_iterator_tag() {} };
struct CDefPointeeBase {
    virtual void v00();
    virtual void Release();                             // slot 1 (+0x04)
    long RefCount;                                      // +0x04
};
struct CThingBaseDef;
template <class T> struct CDefPointer {
    CDefPointeeBase* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->RefCount;
    }
};
typedef CDefPointer<const CThingBaseDef> TDefPtr;
template <class T1, class T2>
inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

struct CDefPointerVector {                              // STLport vector<CDefPointer<const CThingBaseDef> > (12 bytes)
    TDefPtr* _M_start;                                  // +0x00
    TDefPtr* _M_finish;                                 // +0x04
    TDefPtr* _M_end_of_storage;                         // +0x08
    void _M_insert_overflow(TDefPtr* pos, const TDefPtr& x, const __false_type&, unsigned int fill_len, bool atend);   // 0x005B3275
    void push_back(const TDefPtr& x)
    {
        if (_M_finish != _M_end_of_storage) {
            _Construct(_M_finish, x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __false_type(), 1UL, true);
    }
};

struct back_insert_iterator {                           // back_insert_iterator<vector<CDefPointer<const CThingBaseDef> > >
    CDefPointerVector* container;                       // +0x00
    explicit back_insert_iterator(CDefPointerVector& x) : container(&x) {}
    back_insert_iterator& operator=(const TDefPtr& value)
    {
        container->push_back(value);
        return *this;
    }
    back_insert_iterator& operator*() { return *this; }
    back_insert_iterator& operator++() { return *this; }
    back_insert_iterator& operator++(int) { return *this; }
};

back_insert_iterator __fastcall __copy(const TDefPtr* first, const TDefPtr* last, back_insert_iterator result,
                                       const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}