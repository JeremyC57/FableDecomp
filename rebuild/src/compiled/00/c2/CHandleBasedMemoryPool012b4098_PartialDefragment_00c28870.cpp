// CHandleBasedMemoryPool_012b4098::PartialDefragment(long size) -- make room for `size` bytes without a full defragment: first grow the largest free area by moving its movable neighbours away; then walk the free lists from the largest area's bucket down, trying (at most MaxBlocksToTryOnPartialDefrag) small free areas not tried before: grow each until it fits, remembering the ones that would not grow; 1 when a free area of `size` exists
// Leaf: ?PartialDefragment@CHandleBasedMemoryPool_012b4098@@QAEHJ@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C28870, 522 bytes, thiscall `ret 4`, EH-framed (state 0: the vector of tried areas).
//   Callers: Allocate 0x00C28C20 (twice; it binds this spelling). The libpk build of the engine's
//   CHandleBasedMemoryPool::PartialDefragment (Ego_r.pdb `?PartialDefragment@CHandleBasedMemoryPool@@QAE_NJ@Z`, engine copy
//   0x00A02DF0) on this library's pool (vtable 0x012B4098, owner decision 2026-10-31); this build returns int and has no
//   critical section.
// Callees: FindLargestFreeArea @ 0x00C28050 (same-TU body: VC7.1 then knows it keeps ecx, so TestIfAreaIsGrowable and the
//   first GrowFreeArea run on the ecx it left), TestIfAreaIsGrowable(CHeader*, long) @ 0x00C28120, GrowFreeArea(CHeader*&)
//   @ 0x00C286F0 (landed under the catalog's "PartialDefragment" file name), UpdateLargestFreeArea @ 0x00C28080,
//   std::vector<CHeader*>::push_back @ 0x00C29B30 (Dinkumware, declared), operator delete 0x00BFE9BC (the vector's _Tidy).
// Layout: this library's pool (landed Allocate 0x00C28C20 / MoveArea 0x00C28580): MaxBlocksToTryOnPartialDefrag at +0x2c,
//   free lists at +0x3c; modelled locally.
// Shape: one `result` for both halves (1 when the largest free area already fits, else the free-list walk's verdict), returned
//   once at the end: VC7.1 duplicates the early return as its own epilogue (`mov eax,1`) and keeps `result` in esi outside the
//   inner loop (reloaded at the bucket step, `test esi,esi` at the top), which leaves edi to the phase-one zero and then to the
//   vector's _Myfirst (`_Myfirst = 0` coalesces them; the remaining zero stores take a scratch `xor ecx,ecx`). The tried-area
//   vector is Dinkumware's 16-byte {_Alval, _Myfirst, _Mylast, _Myend} at esp+0x24, built with `_Buy(0)`; size() is
//   `_Myfirst == 0 ? 0 : _Mylast - _Myfirst` compared signed; `area == tried[i]` (area first) orders the compare; the area
//   being grown lives in the dead `size` argument slot (size stays in ebp) because GrowFreeArea takes it by reference and the
//   same-TU bodies show VC7.1 that nothing else writes it; the clamp `cap < 0xff ? cap : 0xff` puts the 0xff arm out of line;
//   `blocksTried` is declared before the vector (its zero store comes first).
void __cdecl operator delete(void* block) throw();           // 0x00BFE9BC (Dinkumware <new>: _THROW0, so the vector's _Tidy cannot throw)

struct CHeader {                                // CHandleBasedMemoryPool_012b4098::CHeader, 32 bytes in front of every area
    void*          Resource;                    // +0x00
    long           Handle;                      // +0x04
    long           Size;                        // +0x08
    short          Padding;                     // +0x0c
    unsigned char  Status;                      // +0x0e ('N' free, 'T' taken)
    unsigned char  Locked;                      // +0x0f
    CHeader**      RefAvailable;                // +0x10
    CHeader*       NextAvailable;               // +0x14
    CHeader*       PrevContiguous;              // +0x18
    CHeader*       NextContiguous;              // +0x1c
};

typedef unsigned int size_t;
extern "C" __declspec(dllimport) void* __cdecl memmove(void* dest, const void* src, size_t count);   // IAT 0x01440144 (MSVCR71)
void* __cdecl operator new(size_t size);                                                            // 0x00BFEA1A

// VC7.1 Dinkumware <vector> as the landed push_back 0x00C29B30 / _Insert_n 0x00C295B0 / _Xlen 0x00C28E50 model it (the same
// translation unit instantiates them, so the tried-area vector's address never leaves this TU: VC7.1 then knows that
// TestIfAreaIsGrowable and GrowFreeArea cannot touch it and keeps its _Myfirst in a register across them, reloading it only
// after push_back).
class exception {
public:
    __declspec(dllimport) exception();                       // MSVCR71 import 0x0143FFF8
    __declspec(dllimport) exception(const exception&);
    __declspec(dllimport) virtual ~exception();
    __declspec(dllimport) virtual const char* what() const;
private:
    const char* _m_what;
    int _m_doFree;
};

namespace std {
template<class _Elem> struct char_traits { };
template<class _Ty> class allocator {
public:
    typedef _Ty* pointer;
    typedef unsigned int size_type;
    template<class _Other> struct rebind { typedef allocator<_Other> other; };
    allocator() { }
    allocator(const allocator<_Ty>&) { }
    pointer allocate(size_type _Count) { return ((pointer)operator new(_Count * sizeof (_Ty))); }
    void deallocate(pointer _Ptr, size_type) { operator delete(_Ptr); }
    size_type max_size() const { size_type _Count = (size_type)(-1) / sizeof (_Ty); return (0 < _Count ? _Count : 1); }
};

template<class _Elem, class _Traits, class _Ax> class basic_string {     // 0x1c bytes
public:
    __declspec(dllimport) basic_string(const _Elem* _Ptr);              // MSVCP71 import 0x0143FF98
    __declspec(dllimport) basic_string(const basic_string& _Right);     // MSVCP71 import 0x0143FFA0
    __declspec(dllimport) ~basic_string();
    __declspec(dllimport) const _Elem* c_str() const;
private:
    _Ax _Alval;
    union { _Elem _Buf[16]; _Elem* _Ptr; } _Bx;
    unsigned int _Mysize;
    unsigned int _Myres;
};
typedef basic_string<char, char_traits<char>, allocator<char> > string;

class logic_error : public exception {                                   // vtable 0x012B263C
public:
    explicit logic_error(const string& _Message) : _Str(_Message) { }
    virtual ~logic_error() throw() { }
    virtual const char* what() const throw() { return (_Str.c_str()); }
private:
    string _Str;
};
class length_error : public logic_error {                                // vtable 0x012B2644
public:
    explicit length_error(const string& _Message) : logic_error(_Message) { }
    virtual ~length_error() throw() { }
};

struct _Nonscalar_ptr_iterator_tag { };
struct _Scalar_ptr_iterator_tag { };
struct random_access_iterator_tag { };
template<class _Category, class _Ty, class _Diff, class _Pointer, class _Reference>
struct iterator { };
template<class _Ty, class _Diff, class _Pointer, class _Reference>
struct _Ranit : public iterator<random_access_iterator_tag, _Ty, _Diff, _Pointer, _Reference> { };
template<class _T1, class _T2> inline _Nonscalar_ptr_iterator_tag _Ptr_cat(_T1&, _T2&)
{
    _Nonscalar_ptr_iterator_tag _Cat;
    return (_Cat);
}
template<class _Ty> inline _Scalar_ptr_iterator_tag _Ptr_cat(_Ty**, _Ty**)
{
    _Scalar_ptr_iterator_tag _Cat;
    return (_Cat);
}
template<class _FwdIt, class _Ty> void fill(_FwdIt _First, _FwdIt _Last, const _Ty& _Val);                // 0x00C28FC0 (out of line)
template<class _BidIt1, class _BidIt2> _BidIt2 copy_backward(_BidIt1 _First, _BidIt1 _Last, _BidIt2 _Dest);  // 0x00C29290 (out of line)
template<class _OutIt, class _Diff, class _Ty> inline void fill_n(_OutIt _First, _Diff _Count, const _Ty& _Val)
{
    for (; 0 < _Count; --_Count, ++_First)
        *_First = _Val;
}
template<class _Ty1, class _Ty2, class _Alloc> inline
_Ty2 _Uninit_copy(_Ty1 _First, _Ty1 _Last, _Ty2 _Dest, _Alloc&, _Scalar_ptr_iterator_tag)
{
    size_t _Count = (size_t)(_Last - _First);
    return ((_Ty2)memmove(&*_Dest, &*_First, _Count * sizeof (*_First)) + _Count);
}
template<class _InIt, class _FwdIt, class _Alloc> inline
_FwdIt _Uninitialized_copy(_InIt _First, _InIt _Last, _FwdIt _Dest, _Alloc& _Al)
{
    return (_Uninit_copy(_First, _Last, _Dest, _Al, _Ptr_cat(_First, _Dest)));
}
template<class _FwdIt, class _Diff, class _Tval, class _Alloc>
void __fastcall _Uninit_fill_n(_FwdIt _First, _Diff _Count, const _Tval& _Val, _Alloc& _Al, _Nonscalar_ptr_iterator_tag);
template<class _Ty, class _Diff, class _Tval, class _Alloc> inline
void _Uninit_fill_n(_Ty** _First, _Diff _Count, const _Tval& _Val, _Alloc&, _Scalar_ptr_iterator_tag)
{
    fill_n(_First, _Count, _Val);
}
template<class _FwdIt, class _Diff, class _Tval, class _Alloc> inline
void _Uninitialized_fill_n(_FwdIt _First, _Diff _Count, const _Tval& _Val, _Alloc& _Al)
{
    _Uninit_fill_n(_First, _Count, _Val, _Al, _Ptr_cat(_First, _First));
}

template<class _Ty, class _Alloc> class _Vector_val {
protected:
    _Vector_val(_Alloc _Al = _Alloc()) : _Alval(_Al) { }
    typedef typename _Alloc::template rebind<_Ty>::other _Alty;
    _Alty _Alval;                                                        // +0x00 (empty, padded to a word)
};

template<class _Ty, class _Ax = allocator<_Ty> > class vector : public _Vector_val<_Ty, _Ax> {
public:
    typedef unsigned int size_type;
    typedef int difference_type;
    typedef _Ty* _Tptr;
    typedef _Ty& reference;
    typedef _Ty* pointer;
    typedef const _Ty* _Ctptr;
    typedef const _Ty& const_reference;
    class const_iterator : public _Ranit<_Ty, difference_type, _Ctptr, const_reference> {
    public:                                  // VC7.1: the vector's own iterators wrap one pointer, no user copy constructor
        const_iterator() { _Myptr = 0; }
        const_iterator(_Tptr _Ptr) { _Myptr = _Ptr; }
        reference operator*() const { return (*_Myptr); }
        const_iterator& operator+=(difference_type _Off) { _Myptr += _Off; return (*this); }
        const_iterator operator+(difference_type _Off) const { const_iterator _Tmp = *this; return (_Tmp += _Off); }
        difference_type operator-(const const_iterator& _Right) const { return (_Myptr - _Right._Myptr); }
        _Tptr _Myptr;
    };
    class iterator : public const_iterator {
    public:
        iterator() { }
        iterator(pointer _Ptr) : const_iterator(_Ptr) { }
        reference operator*() const { return (*this->_Myptr); }
        iterator& operator+=(difference_type _Off) { this->_Myptr += _Off; return (*this); }
        iterator operator+(difference_type _Off) const { iterator _Tmp = *this; return (_Tmp += _Off); }
    };
    vector() : _Vector_val<_Ty, _Ax>() { _Buy(0); }
    ~vector() { _Tidy(); }
    iterator begin() { return (iterator(_Myfirst)); }
    iterator end() { return (iterator(_Mylast)); }
    size_type size() const { return (_Myfirst == 0 ? 0 : _Mylast - _Myfirst); }
    size_type capacity() const { return (_Myfirst == 0 ? 0 : _Myend - _Myfirst); }
    size_type max_size() const { return (this->_Alval.max_size()); }
    reference operator[](size_type _Pos) { return (*(begin() + _Pos)); }
    void push_back(const _Ty& _Val)                                      // 0x00C29B30
    {
        if (size() < capacity())
            _Mylast = _Ufill(_Mylast, 1, _Val);
        else
            insert(end(), _Val);
    }
    iterator insert(iterator _Where, const _Ty& _Val)
    {
        size_type _Off = size() == 0 ? 0 : _Where - begin();
        _Insert_n(_Where, (size_type)1, _Val);
        return (begin() + _Off);
    }
protected:
    bool _Buy(size_type _Capacity)
    {
        _Myfirst = 0, _Mylast = 0, _Myend = 0;
        if (_Capacity == 0)
            return (false);
        else if (max_size() < _Capacity)
            _Xlen();
        else {
            _Myfirst = this->_Alval.allocate(_Capacity);
            _Mylast = _Myfirst;
            _Myend = _Myfirst + _Capacity;
        }
        return (true);
    }
    void _Tidy()
    {
        if (_Myfirst != 0)
            this->_Alval.deallocate(_Myfirst, _Myend - _Myfirst);
        _Myfirst = 0, _Mylast = 0, _Myend = 0;
    }
    void _Insert_n(iterator _Where, size_type _Count, const _Ty& _Val);  // 0x00C295B0 (body below)
    template<class _Iter> pointer _Ucopy(_Iter _First, _Iter _Last, pointer _Ptr)
    {
        return (_Uninitialized_copy(_First, _Last, _Ptr, this->_Alval));
    }
    pointer _Ufill(pointer _Ptr, size_type _Count, const _Ty& _Val)
    {
        _Uninitialized_fill_n(_Ptr, _Count, _Val, this->_Alval);
        return (_Ptr + _Count);
    }
    __declspec(noreturn) void _Xlen() const;                             // 0x00C28E50 (body below)
    pointer _Myfirst;                                                    // +0x04
    pointer _Mylast;                                                     // +0x08
    pointer _Myend;                                                      // +0x0c
};

template<class _Ty, class _Ax> void vector<_Ty, _Ax>::_Xlen() const
{
    throw length_error("vector<T> too long");
}

template<class _Ty, class _Ax>
void vector<_Ty, _Ax>::_Insert_n(iterator _Where, size_type _Count, const _Ty& _Val)
{
    _Ty _Tmp = _Val;
    size_type _Capacity = capacity();

    if (_Count == 0)
        ;
    else if (max_size() - size() < _Count)
        _Xlen();
    else if (_Capacity < size() + _Count) {
        _Capacity = max_size() - _Capacity / 2 < _Capacity ? 0 : _Capacity + _Capacity / 2;
        if (_Capacity < size() + _Count)
            _Capacity = size() + _Count;
        pointer _Newvec = this->_Alval.allocate(_Capacity);
        pointer _Ptr = _Newvec;

        _Ptr = _Ucopy(_Myfirst, _Where._Myptr, _Newvec);
        _Ptr = _Ufill(_Ptr, _Count, _Tmp);
        _Ucopy(_Where._Myptr, _Mylast, _Ptr);

        _Count += size();
        if (_Myfirst != 0)
            this->_Alval.deallocate(_Myfirst, _Myend - _Myfirst);
        _Myend = _Newvec + _Capacity;
        _Mylast = _Newvec + _Count;
        _Myfirst = _Newvec;
    } else if ((size_type)(_Mylast - _Where._Myptr) < _Count) {
        _Ucopy(_Where._Myptr, _Mylast, _Where._Myptr + _Count);
        _Ufill(_Mylast, _Count - (_Mylast - _Where._Myptr), _Tmp);
        _Mylast += _Count;
        fill(_Where._Myptr, _Mylast - _Count, _Tmp);
    } else {
        pointer _Oldend = _Mylast;
        _Mylast = _Ucopy(_Oldend - _Count, _Oldend, _Mylast);
        copy_backward(_Where._Myptr, _Oldend - _Count, _Oldend);
        fill(_Where._Myptr, _Where._Myptr + _Count, _Tmp);
    }
}
}

class CHandleBasedMemoryPool_012b4098 { public:                 // this library's pool variant (see Layout)
    virtual void MoveResource(void* resource, long handle, void* from, void* to, unsigned long size);   // slot 0
    virtual int  CanMoveResource(void* resource, long handle);                                        // slot 1
    virtual void ValidateResource(void* resource);                                                    // slot 2
    void*                 Buffer;               // +0x04
    void*                 UnalignedBuffer;      // +0x08
    long                  PoolSize;             // +0x0c
    long                  AllocatedSize;        // +0x10
    long                  NumAllocations;       // +0x14
    long                  LockCount;            // +0x18
    long                  LargestFreeArea;      // +0x1c
    long                  MaxFreeSizeBeforeDefrag;        // +0x20
    long                  MaxFreeSizeBeforeFailure;       // +0x24
    long                  EnableFullDefragIfPartialFails; // +0x28
    long                  MaxBlocksToTryOnPartialDefrag;  // +0x2c
    long                  m_30;                 // +0x30
    void*                 m_defaultHandler;     // +0x34 embedded default handler (one vptr)
    void*                 FailureHandler;       // +0x38
    CHeader*              FreeLists[256];       // +0x3c
    std::vector<CHeader*> Handles;              // +0x43c
    long                  FirstUnusedHandle;    // +0x44c

    CHeader* FindLargestFreeArea();                          // 0x00C28050 (same-TU body below)
    int  TestIfAreaIsGrowable(CHeader* area, long required);   // 0x00C28120 (same-TU body below)
    int  GrowFreeArea(CHeader*& area);                      // 0x00C286F0 (same-TU body below)
    void UpdateLargestFreeArea();                           // 0x00C28080 (same-TU body below)
    void RemoveFreeArea(CHeader* area);                     // 0x00C27F50
    CHeader* FindFreeArea(long size);                       // 0x00C27FD0
    void InsertFreeArea_00c27ef0(CHeader* area);            // 0x00C27EF0 (the landed spelling)
    void MoveArea(CHeader* from, CHeader* to);              // 0x00C28580
    CHeader* CombineAdjacentAreas(CHeader* area);           // 0x00C282E0
    int  PartialDefragment(long size);
};

// Same-TU bodies of the landed callees (CHandleBasedMemoryPool.cpp is one translation unit): VC7.1 then knows that
// FindLargestFreeArea leaves ecx alone, that only GrowFreeArea writes the area it is handed by reference (the register copy
// of `area` survives TestIfAreaIsGrowable and is reloaded after GrowFreeArea), and that none of them keeps the address;
// context only, never inlined.
#pragma auto_inline(off)
CHeader* CHandleBasedMemoryPool_012b4098::FindLargestFreeArea()                      // 0x00C28050
{
    long size = LargestFreeArea;
    unsigned long bucket = (unsigned long)size >> 9;
    if (bucket >= 0xff)
        bucket = 0xff;
    for (CHeader* area = FreeLists[bucket]; area; area = area->NextAvailable) {
        if (area->Size == size)
            return area;
    }
    return 0;
}

void CHandleBasedMemoryPool_012b4098::UpdateLargestFreeArea()                        // 0x00C28080
{
    unsigned long cap = (unsigned long)LargestFreeArea >> 9;
    if (cap >= 0xff)
        cap = 0xff;
    for (long bucket = (long)cap; bucket >= 0; --bucket) {
        if (FreeLists[bucket]) {
            long largest = 0;
            for (CHeader* area = FreeLists[bucket]; area; area = area->NextAvailable) {
                if (largest < area->Size)
                    largest = area->Size;
            }
            LargestFreeArea = largest;
            return;
        }
    }
    LargestFreeArea = 0;
}

int CHandleBasedMemoryPool_012b4098::TestIfAreaIsGrowable(CHeader* area, long required)   // 0x00C28120
{
    char* start = (char*)(area + 1);
    char* end = start + area->Size + area->Padding;
    if (end - start >= required)
        return 1;
    CHeader* header = area->PrevContiguous;
    while (header) {
        if (header->Status != 'N' && (header->Locked || !CanMoveResource(header->Resource, header->Handle)))
            break;
        start = (char*)(header + 1);
        if (end - start >= required)
            return 1;
        header = header->PrevContiguous;
    }
    header = area->NextContiguous;
    while (header) {
        if (header->Status != 'N' && (header->Locked || !CanMoveResource(header->Resource, header->Handle)))
            break;
        if ((char*)(header + 1) + header->Size + header->Padding - start >= required)
            return 1;
        header = header->NextContiguous;
    }
    return 0;
}

int CHandleBasedMemoryPool_012b4098::GrowFreeArea(CHeader*& area)                    // 0x00C286F0
{
    CHeader* p = area->NextContiguous;
    if (p && !p->Locked && CanMoveResource(p->Resource, p->Handle)) {
        RemoveFreeArea(area);
        CHeader* freeArea = FindFreeArea(p->Size);
        area->Status = 'N';
        InsertFreeArea_00c27ef0(area);
        if (freeArea) {
            MoveArea(p, freeArea);
            CombineAdjacentAreas(p);
            return 1;
        }
    }
    p = area->PrevContiguous;
    if (p && !p->Locked && CanMoveResource(p->Resource, p->Handle)) {
        RemoveFreeArea(area);
        CHeader* freeArea = FindFreeArea(p->Size);
        area->Status = 'N';
        InsertFreeArea_00c27ef0(area);
        if (freeArea) {
            MoveArea(p, freeArea);
            area = CombineAdjacentAreas(p);
            return 1;
        }
    }
    return 0;
}
#pragma auto_inline(on)

int CHandleBasedMemoryPool_012b4098::PartialDefragment(long size)
{
    int result = 0;
    CHeader* area = FindLargestFreeArea();
    if (area && TestIfAreaIsGrowable(area, size) && LargestFreeArea < size) {
        do {
            area = FindLargestFreeArea();
            if (!area)
                break;
            if (!GrowFreeArea(area))
                break;
        } while (LargestFreeArea < size);
    }
    if (LargestFreeArea >= size) {
        result = 1;
    } else {

        long blocksTried = 0;
        std::vector<CHeader*> tried;
        unsigned long cap = (unsigned long)LargestFreeArea >> 9;
        for (long bucket = cap < 0xff ? (long)cap : 0xff; bucket >= 0; --bucket) {
            CHeader** list = &FreeLists[bucket];
            if (result)
                break;
            if (blocksTried >= MaxBlocksToTryOnPartialDefrag)
                break;
            area = *list;
            while (area) {
                if (area->Size >= size)
                    break;
                if (blocksTried >= MaxBlocksToTryOnPartialDefrag)
                    break;
                int found = 0;
                for (long i = 0; i < (long)tried.size(); ++i) {
                    if (area == tried[i]) {
                        found = 1;
                        break;
                    }
                }
                CHeader* original = area;
                if (!found) {
                    ++blocksTried;
                    if (TestIfAreaIsGrowable(area, size)) {
                        if (GrowFreeArea(area)) {
                            while (area->Size < size) {
                                if (!GrowFreeArea(area)) {
                                    tried.push_back(original);
                                    break;
                                }
                            }
                            if (area->Size >= size) {
                                result = 1;
                                break;
                            }
                            area = *list;
                            continue;
                        }
                    }
                }
                area = area->NextAvailable;
            }
        }
    UpdateLargestFreeArea();
    }
    return result;
}