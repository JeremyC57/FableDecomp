// back_insert_iterator<vector<CDefPointer<const CThingBaseDef> > >::operator=(const CDefPointer<const CThingBaseDef>& value) -- STLport back inserter of the hero-morph tattoo defs: push_back the def pointer (in place with an add-ref, or through the out-of-line growth path) and return the iterator
// Leaf: ??4back_insert_iterator
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00721D30, 68 bytes, `ret 4`.
// Label correction: the catalog "Vector_Push_RefCounted" is descriptive; the body is STLport
//   `back_insert_iterator::operator=(const value_type& v) { container->push_back(v); return *this; }` over the
//   CTCHeroMorph Tattoos vector (the inline copy of 0x00721F70 `__copy` uses the same push_back).
// Callee: vector::_M_insert_overflow(pos, x, const __false_type&, fill_len, atend) @ 0x005B3275 (thiscall, `ret 14h`;
//   catalog "Tattoo_ReallocateAndCopy").
// Shape: STLport push_back: `if (_M_finish != _M_end_of_storage) { _Construct(_M_finish, x); ++_M_finish; } else
//   _M_insert_overflow(_M_finish, x, __false_type(), 1, true);` -- the construct is the CDefPointer copy
//   constructor behind the placement-new null test, the increment a memory `add [ecx+4],4`, and the empty tag
//   temporary sits on the (live) value argument slot (`lea edx,[esp+10h]`).
inline void* operator new(unsigned int, void* p) { return p; }
struct __false_type { __false_type() {} };

struct CDefPointeeBase {
    virtual void v00();
    virtual void Release();                             // slot 1 (+0x04)
    long RefCount;                                      // +0x04
};
struct CThingBaseDef;
template <class T> struct CDefPointer {                 // 4 bytes
    CDefPointeeBase* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->RefCount;
    }
};
typedef CDefPointer<const CThingBaseDef> TDefPtr;

template <class T1, class T2>
inline void _Construct(T1* p, const T2& value)
{
    new (p) T1(value);
}

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
    back_insert_iterator& operator=(const TDefPtr& value);
};

back_insert_iterator& back_insert_iterator::operator=(const TDefPtr& value)
{
    container->push_back(value);
    return *this;
}