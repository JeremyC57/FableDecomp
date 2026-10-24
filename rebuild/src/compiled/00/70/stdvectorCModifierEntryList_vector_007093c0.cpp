// std::vector<CModifierEntryList>::vector(size_type n) -- STLport sized constructor of the hero appearance modifier table: allocate n twelve-byte modifier lists and copy-construct each from an empty default list
// Leaf: ??0CModifierListVector
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007093C0, length 117 bytes. __fastcall (this in ecx), `ret 4`, returns this.
// Callees: malloc (CRT import thunk) @ 0x00BFEA0E, vector<CModifierEntry>::vector(const vector&) @ 0x00708830.
// Identity: the manifest label "std_vector_InitFromRange" is descriptive. The element copy constructor 0x00708830 is the
//   vector<CTCHeroAttachableAppearanceModifiers::CModifierEntry> copy (8-byte elements, `sar 3`); this is the
//   out-of-line instance of `explicit vector(size_type n)` for CTCHeroAttachableAppearanceModifiers::AppearanceModifiers
//   (vector<vector<CModifierEntry>>), the same body the landed constructor 0x00706D10 inlines with n == 3.
// Shape: STLport 4.x `explicit vector(size_type __n) : _Vector_base(__n, allocator_type()) { _M_finish =
//   uninitialized_fill_n(_M_start, __n, _Tp()); }`: _Vector_base zeroes the three words, `allocate(n)` is
//   `n != 0 ? malloc(n * 12) : 0`, start/finish/end are stored, then the empty T() temporary is zeroed in the frame
//   and copied into each slot (placement new keeps its null test, unsigned `n > 0` count-down). As in the landed
//   0x00706D10, the empty temporary has no destructor call in retail, so the element is modelled without one.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E (import thunk)
inline void* operator new(unsigned int, void* where) { return where; }

struct CModifierEntry { unsigned char _body[8]; };     // CTCHeroAttachableAppearanceModifiers::CModifierEntry
struct CModifierEntryList {                            // vector<CModifierEntry> (12 bytes)
    CModifierEntry* _M_start;
    CModifierEntry* _M_finish;
    CModifierEntry* _M_end_of_storage;
    CModifierEntryList() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    CModifierEntryList(const CModifierEntryList& other);   // 0x00708830
};

struct SModifierListAllocator {                        // std::allocator<CModifierEntryList> (empty)
    SModifierListAllocator() { }
    CModifierEntryList* allocate(unsigned int n)
    {
        return n != 0 ? (CModifierEntryList*)malloc(n * sizeof(CModifierEntryList)) : 0;
    }
};
struct SModifierListAllocProxy : SModifierListAllocator {   // STLport _STLP_alloc_proxy: the end-of-storage pointer
    CModifierEntryList* _M_data;
    SModifierListAllocProxy(const SModifierListAllocator&, CModifierEntryList* data) : _M_data(data) { }
};
struct SModifierListVectorBase {                       // STLport _Vector_base
    CModifierEntryList*     _M_start;
    CModifierEntryList*     _M_finish;
    SModifierListAllocProxy _M_end_of_storage;
    SModifierListVectorBase(unsigned int n, const SModifierListAllocator& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0)
    {
        _M_start = _M_end_of_storage.allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + n;
    }
};

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    _ForwardIter __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        new (__cur) _Tp(__x);
    return __cur;
}

struct CModifierListVector : SModifierListVectorBase {  // vector<vector<CModifierEntry>> (12 bytes)
    explicit CModifierListVector(unsigned int n);
};

CModifierListVector::CModifierListVector(unsigned int n)
    : SModifierListVectorBase(n, SModifierListAllocator())
{
    _M_finish = uninitialized_fill_n(_M_start, n, CModifierEntryList());
}