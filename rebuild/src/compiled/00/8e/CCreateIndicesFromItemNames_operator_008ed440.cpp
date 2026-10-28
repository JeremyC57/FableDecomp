// CCreateIndicesFromItemNames::operator()(const CCharString&) -- append (definition index of the named def, "") to ItemIndices
// Leaf: ??RCCreateIndicesFromItemNames@@
// Retail 0x008ED440, 0x8F bytes, thiscall `ret 4`. Manifest label "std_vector_push_back<DefinitionIndex_CCharString>" is
//   descriptive; PDB: `??RCCreateIndicesFromItemNames@@QAEXABVCCharString@@@Z`. Only caller: for_each 0x008ED510
//   (loading branch of CTCContainer::OnSerialise).
// Callees: CGameDefinitionManager::Get @ 0x0044C6B0, CDefinitionManager::GetDefGlobalIndexFromName @ 0x009AD410,
//   CCharString(const char*, long = -1) @ 0x0099EBF0 ("" @ 0x0122D70E), copy ctor 0x0099EC30, dtor 0x0099EAE0 (twice:
//   the pair temporary, then the "" temporary), vector<pair<long, CCharString>>::_M_insert_overflow @ 0x0074FCE0.
// Shape: the index is a named local (fetched before the "" string is built); an EXPLICIT `CCharString("")` temporary
//   (its constructor's eax feeds the pair copy; the implicit "" conversion re-forms the address with `lea`) in the dead
//   `name` slot; the pair temporary is the 8-byte frame; STLport's pair copy ctor stores `first` before the string
//   copy call; the finish advances as `add [esi+4],8`.
// TU: CTCContainer (0x008ED040 RemoveItem, 0x008ED1F0 InsertItem landed, 0x008ED2B0 OnSerialise, the two functors at
//   0x008ED3B0 / 0x008ED440 and their std::for_each instantiations 0x008ED4D0 / 0x008ED510). Names from the FableWin PDB
//   name set: CCreateItemNamesFromIndices (ItemNames vector<CCharString>*), CCreateIndicesFromItemNames (ItemIndices
//   vector<pair<long, CCharString>>*), both with `operator()` non-const; Ego_r.pdb: CTCContainer::ContainerItems +0x0C,
//   CDefString::TablePos, CPersistContext::Mode +0x18.
// Shape notes shared by the TU: STLport 4.x vector (`_Vector_base` + `_STLP_alloc_proxy`), push_back with
//   `_M_insert_overflow(finish, x, __false_type(), 1, true)` as a same-TU noinline STAND-IN body (only the call is
//   graded; it reads `x` and never touches the tag, so the empty tag shares the dead-argument slot of the live
//   temporary, GOTCHAS b48) and STLport's MSVC `pair` copy constructor.
extern "C" void __cdecl free(void* block);                                       // 0x00BFEA14
inline void* operator new(unsigned int, void* __p) { return __p; }

class CCharString {
public:
    void* PStringData;                          // +0x00
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);      // 0x0099EC30
    ~CCharString();                             // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

class CDefString {                              // 4 bytes, an index into the def string table
public:
    long TablePos;                              // +0x00
    CDefString() {}
};

class CDefStringTable {
public:
    CCharString GetString(long tablePos) const; // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;        // 0x013CA828

class CDefinitionManager {
public:
    CDefString GetDefNameFromGlobalIndex(long index) const;          // 0x009ACCC0
    long GetDefGlobalIndexFromName(const CCharString& name) const;   // 0x009AD410
};

class CGameDefinitionManager : public CDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();                 // 0x0044C6B0
};

namespace std {
struct __false_type { __false_type() {} };
struct random_access_iterator_tag { random_access_iterator_tag() {} };

template <class _T1, class _T2> struct pair {
    _T1 first;                                  // +0x00
    _T2 second;                                 // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}   // STLport's MSVC copy ctor
};

template <class _Tp> class allocator {
public:
    allocator() {}
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) {}
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                              // +0x00
    _Tp* _M_finish;                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) {}
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) {}
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
#pragma auto_inline(off)
    // CCharString 0x00433530, pair<long, CCharString> 0x0074FCE0. Same-TU noinline STAND-IN body (only its call
    // matters): it reads `x` and never touches the empty tag, so VC7.1 homes the tag on the live temporary's slot.
    protected:
    __declspec(noinline) void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&,
                                                 unsigned int __fill_len, bool __atend)
    {
        if (__atend && __fill_len == 1 && __position == this->_M_finish)
            _Construct(this->_M_start, __x);
    }
    public:
#pragma auto_inline(on)
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};

template <class _InputIter, class _Function>
_Function __fastcall for_each(_InputIter __first, _InputIter __last, _Function __f);   // 0x008ED4D0 / 0x008ED510
}

typedef std::pair<long, CCharString> CContainerItem;

class CCreateItemNamesFromIndices {
public:
    std::vector<CCharString>* ItemNames;       // +0x00
    CCreateItemNamesFromIndices(std::vector<CCharString>& itemNames) : ItemNames(&itemNames) {}
    void operator()(const CContainerItem& item);
};

class CCreateIndicesFromItemNames {
public:
    std::vector<CContainerItem>* ItemIndices;  // +0x00
    CCreateIndicesFromItemNames(std::vector<CContainerItem>& itemIndices) : ItemIndices(&itemIndices) {}
    void operator()(const CCharString& name);
};

class CPersistContext {
public:
    unsigned char _pad00[0x18];
    long Mode;                                  // +0x18 (EMode)
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    template <class T> void TransferVector(const char* name, std::vector<T>& value);   // <CCharString> 0x0049B8D0
};

class CTCContainer {
public:
    unsigned char _base[0xc];                   // +0x00 CTCBase (vptr, parent thing, flags)
    std::vector<CContainerItem> ContainerItems;   // +0x0c
    void OnSerialise(CPersistContext& context, bool savedInGame);
};

void CCreateIndicesFromItemNames::operator()(const CCharString& name)
{
    long defIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name);
    ItemIndices->push_back(CContainerItem(defIndex, CCharString("")));
}