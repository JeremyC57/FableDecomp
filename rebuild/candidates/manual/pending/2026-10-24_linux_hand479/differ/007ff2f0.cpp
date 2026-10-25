// CTCNamedUserActionPoints::OnCreate() -- reset the action-point state (no user thing, no points, empty free/blocked index lists), then take every dummy-object name of the owner's mesh as a named action point, listing each index as free
// Grade: DIFFER(358v360) -- the same this=esi / temps=edi register swap as 0x007FE990, plus retail's begin-copy through ecx (`mov ecx,[names] ; mov edi,ecx`) and one shared tag byte (both push_back overflow tags at +0x13; ours puts the __false_type at +0x12). Tried: 1 grade (sibling of 0x007FE990, whose four grades covered the swap). Next: whatever fixes 0x007FE990's register priority; the tag slot may follow.
// Leaf: ?OnCreate@CTCNamedUserActionPoints@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007FF2F0, 360 bytes (`ret`), CTCNamedUserActionPoints vtable 0x01241F24 slot 4
//   (Ego_r.pdb ?OnCreate@CTCNamedUserActionPoints@@UAEXXZ); the catalog's "CTCGraphicAppearance_InitDummyObjectsWithNames" is descriptive.
// Callees: as 0x007FE990 (no find), plus CCharString(const CCharString&) 0x0099EC30 and vector<CCharString>::
//   _M_insert_overflow 0x00433530 (Names.push_back). Names is the Ego_r.pdb member at retail +0x34.
extern "C" void  __cdecl free(void* p);                    // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int n);   // 0x00BFEAE6

inline void* operator new(unsigned int, void* p) { return p; }

#pragma auto_inline(off)
__declspec(noinline) long __fastcall Compare(const char* a, const char* b)   // 0x00411570 (the landed body)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)

struct CCharStringData {                  // the shared string block
    const char*   Chars;                  // +0x00
    unsigned long Length;                 // +0x04
};

class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData && PStringData->Length == other.PStringData->Length)
            return Compare(PStringData->Chars, other.PStringData->Chars) ? false : true;
        return false;
    }
};

namespace std {

struct __true_type { __true_type() { } };
struct __false_type { __false_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> struct __type_traits {
    typedef __false_type has_trivial_assignment_operator;
    typedef __false_type has_trivial_destructor;
    typedef __false_type is_POD_type;
};
template <> struct __type_traits<long> {
    typedef __true_type has_trivial_assignment_operator;
    typedef __true_type has_trivial_destructor;
    typedef __true_type is_POD_type;
};

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last)
{
    __destroy_aux(__first, __last, typename __type_traits<_Tp>::has_trivial_destructor());
}

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}
template <class _Tp>
inline _Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __true_type&)
{
    return (_Tp*)__copy_trivial(__first, __last, __result);
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    typedef _Alloc allocator_type;
    typedef typename __type_traits<_Tp>::has_trivial_assignment_operator _TrivialAss;
    typedef typename __type_traits<_Tp>::is_POD_type _IsPODType;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __true_type&, size_type __fill_len, bool __atend);
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend);
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        } else
            _M_insert_overflow(this->_M_finish, __x, _IsPODType(), 1UL, true);
    }
    iterator erase(iterator __first, iterator __last)
    {
        pointer __i = __copy_ptrs(__last, this->_M_finish, __first, _TrivialAss());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void clear() { erase(begin(), end()); }
};

#pragma auto_inline(off)
template <class _RandomAccessIter, class _Tp>
__declspec(noinline) _RandomAccessIter __fastcall __find(_RandomAccessIter __first, _RandomAccessIter __last, const _Tp& __val,
                                                         const random_access_iterator_tag&)   // 0x004B8E40 (the landed body)
{
    int __trip_count = (__last - __first) >> 2;
    for ( ; __trip_count > 0 ; --__trip_count) {
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
        if (*__first == __val) return __first;
        ++__first;
    }
    switch (__last - __first) {
    case 3:
        if (*__first == __val) return __first;
        ++__first;
    case 2:
        if (*__first == __val) return __first;
        ++__first;
    case 1:
        if (*__first == __val) return __first;
        ++__first;
    case 0:
    default:
        return __last;
    }
}
#pragma auto_inline(on)

template <class _InputIter, class _Tp>
inline _InputIter find(_InputIter __first, _InputIter __last, const _Tp& __val)
{
    return __find(__first, __last, __val, random_access_iterator_tag());
}

}   // namespace std

extern CCharString g_ActionPointName;                      // 0x013BAC9C ("ACTION_POINT", the landed 0x007FEAF0 global)

namespace NGraphicAppearance {
class CTCGraphicAppearance {
public:
    void GetAllDummyObjectNames(std::vector<CCharString>& names) const;   // 0x004BDB40
};
}

class CThing {
public:
    unsigned char _pad00[0x64];
    NGraphicAppearance::CTCGraphicAppearance* GraphicAppearance;   // +0x64
};

class CBaseIntelligentPointer {
public:
    virtual void v00();
    CThing* m_pItem;                                       // +0x04
    CBaseIntelligentPointer();                             // 0x00A01B10
    ~CBaseIntelligentPointer();                            // 0x00A01C10
    CThing* GetPItem() const;                              // 0x00A01B50
    void SetPItem(CThing* item);                           // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing>: vtable 0x01238C6C
public:
    virtual void v00();
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

class CTCBase {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void OnCreate();                               // slot 4
    CThing* ParentThing;                                   // +0x04
    bool ToBeRemoved;                                      // +0x08
    bool IsInGlobalUpdateTCs;                              // +0x09
};
class CTCUserActionPointBase : public CTCBase {            // Ego_r.pdb (retail offsets: STLport vectors are 12 bytes)
public:
    std::vector<long> FreeActionPointIndexes;              // +0x0c
    std::vector<long> BlockedActionPointIndexes;           // +0x18
    long NumberOfActionPoints;                             // +0x24
    long FrameOfNextCheck;                                 // +0x28
    CIntelligentPointer<CThing> PThing;                    // +0x2c
};
class CTCNamedUserActionPoints : public CTCUserActionPointBase {
public:
    std::vector<CCharString> Names;                        // +0x34
    virtual void OnCreate();
};

void CTCNamedUserActionPoints::OnCreate()
{
    PThing = CIntelligentPointer<CThing>(0);
    FrameOfNextCheck = 0;
    NumberOfActionPoints = 0;
    BlockedActionPointIndexes.clear();
    FreeActionPointIndexes.clear();
    std::vector<CCharString> names;
    ParentThing->GraphicAppearance->GetAllDummyObjectNames(names);
    {
        long index = 0;
        for (CCharString* it = names.begin(); it != names.end(); ++it) {
            Names.push_back(*it);
            FreeActionPointIndexes.push_back(index);
            ++index;
            ++NumberOfActionPoints;
        }
    }
}
