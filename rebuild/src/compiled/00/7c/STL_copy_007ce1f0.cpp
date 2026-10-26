// std::__copy(CThing** first, CThing** last, back_insert_iterator<vector<CIntelligentPointer<CThing> > > result, const random_access_iterator_tag&, int*) -- STLport random-access copy of a thing-pointer array into a vector of intelligent pointers through a back inserter: each pointer is wrapped in a temporary CIntelligentPointer<CThing> and pushed back; returns the inserter
// Leaf: ??$__copy@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007CE1F0, 201 bytes (+7 int3), `ret 10h`, `and esp,-8` frame: /Gr __fastcall
//   returning the 4-byte back_insert_iterator through the hidden pointer in ecx (it has a user constructor), `first`
//   in edx, then last, the inserter by value, the tag reference and the distance pointer on the stack.
// Identity: called by the CTCBurntEffect ctor 0x007CDFE0 (landed; "STLport __copy into a back_insert_iterator
//   0x007CE1F0", copy(things.begin(), things.end(), back_inserter(TintList)) over a vector<CThing*>), the two
//   copy() wrappers 0x007CE2C0 / 0x007CE2F0, CTCElectrocutedEffect 0x007CEB00 and 0x007CF110. Catalog label
//   "Vector_InsertElements_SmartPtr" is descriptive.
// Callees: CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor @ 0x00A01B10 / 0x00A01B50 / 0x00A01B90 / 0x00A01C10
//   (CIntelligentPointer<CThing> vtable 0x01238C6C), vector<CIntelligentPointer<CThing> >::_M_insert_overflow(pointer,
//   const T&, const __false_type&, size_type, bool) @ 0x004AC940.
// Shape: STLport `for (_Distance __n = __last - __first; __n > 0; --__n) { *__result = *__first; ++__first;
//   ++__result; } return __result;` -- the assignment converts the CThing* into a temporary intelligent pointer and
//   runs the inserter's operator= (vector::push_back inline: placement-new null test, copy constructor, `add [edi+4],8`,
//   else _M_insert_overflow with its own __false_type byte); 1-aligned intelligent pointer (the landed GiveItems
//   0x007CA9E0 model).
inline void* operator new(unsigned int, void* p) { return p; }

class CThing;
class CBaseObject;

#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                               // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CThing* item);                        // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C for CThing
    virtual void v00();
    CIntelligentPointer(T* item) { SetPItem((CThing*)item); }
    CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
};
#pragma pack(pop)

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct __false_type { __false_type() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class T> class vector {
public:
    typedef T value_type;
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    T* _M_end_of_storage;                                   // +0x08
    void _M_insert_overflow(T* __position, const T& __x, const __false_type&, unsigned int __fill_len, bool __atend);   // 0x004AC940
    void push_back(const T& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};

template <class _Container> class back_insert_iterator {
protected:
    _Container* container;
public:
    explicit back_insert_iterator(_Container& __x) : container(&__x) { }
    back_insert_iterator<_Container>& operator=(const typename _Container::value_type& __value)
    {
        container->push_back(__value);
        return *this;
    }
    back_insert_iterator<_Container>& operator*() { return *this; }
    back_insert_iterator<_Container>& operator++() { return *this; }
};

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

template back_insert_iterator<vector<CIntelligentPointer<CThing> > > __fastcall
__copy<CThing**, back_insert_iterator<vector<CIntelligentPointer<CThing> > >, int>(
    CThing**, CThing**, back_insert_iterator<vector<CIntelligentPointer<CThing> > >,
    const random_access_iterator_tag&, int*);
}