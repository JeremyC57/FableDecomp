// CTCInventoryMap::Copy(const CTCBase& other) -- copies the map state from another map component: empties the three name lists and the id list, appends every entry of the other map's lists in order, then runs the base CTCBase::Copy
// Retail: Fable.exe (TLC, VC7.1) @ 0x005FDAD0, 758 bytes, `ret 4`. Speed TU (no pragmas, no frame pointer).
// Callees: vector<CWideString>::erase(first, last) 0x00412450, CWideString copy ctor 0x0099B720 (push_back's placement
//   copy), vector<CWideString>::_M_insert_overflow 0x00412330, malloc 0x00BFEA0E / free 0x00BFEA14 (the id list's
//   inline STLport growth), CTCBase::Copy(const CTCBase&) 0x006868D0.
// Shape: the id list's push_back and _M_insert_overflow are `__forceinline` (STLport loops: null-checked placement,
//   `max(size, 1)` by reference, a separate uninitialized_copy helper whose result is the new finish); the CWideString
//   overflow 0x00412330 needs a same-TU noinline stand-in reading x only, so the empty `__false_type()` tag lands on the
//   live `other` argument slot as in retail (otherwise a 0x14 frame).
// Layout: local view -- three vector<CWideString> at +0x150 / +0x15C / +0x168 and a 4-byte-element id vector at +0x174
//   (12-byte STLport vectors).
extern "C" void* __cdecl malloc(unsigned int size);  // 0x00BFEA0E
extern "C" void __cdecl free(void* block);           // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { __false_type() {} };
struct CWideString {
    void* PStringData;
    CWideString(const CWideString& other);           // 0x0099B720
};
struct CWideStringVector {                           // retail STLport vector<CWideString> (12 bytes)
    CWideString* _M_start;
    CWideString* _M_finish;
    CWideString* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    const CWideString& operator[](unsigned long n) const { return _M_start[n]; }
    CWideString* erase(CWideString* first, CWideString* last);   // 0x00412450
    void clear() { erase(_M_start, _M_finish); }
    void _M_insert_overflow(CWideString* pos, const CWideString& x, const __false_type& tag,
                            unsigned long fillLength, bool atEnd);   // 0x00412330
    void push_back(const CWideString& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                new (_M_finish) CWideString(x);
            ++_M_finish;
        } else {
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
        }
    }
};
void* FindRef_CWideStringVector__M_insert_overflow_0();   // stand-in callee of the stand-in body below
// Same-TU stand-in body for vector<CWideString>::_M_insert_overflow 0x00412330 (out of line in retail): it reads the value
// only through its const reference and never touches the empty tag.
#pragma auto_inline(off)
__declspec(noinline) void CWideStringVector::_M_insert_overflow(CWideString* pos, const CWideString& x, const __false_type& tag, unsigned long fillLength, bool atEnd)
{
    void* p = FindRef_CWideStringVector__M_insert_overflow_0();
    if (*(void**)&x != p)
        return;
    _M_finish = pos;
}
#pragma auto_inline(on)
enum ERegionId { ERegionId_Force32 = 0x7fffffff };
struct CIdVector {                                   // STLport vector of a 4-byte enum (non-trivial traits: loops)
    ERegionId* _M_start;
    ERegionId* _M_finish;
    ERegionId* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    const ERegionId& operator[](unsigned long n) const { return _M_start[n]; }
    static ERegionId* copy(ERegionId* first, ERegionId* last, ERegionId* result)
    {
        for (int n = last - first; n > 0; --n) {
            *result = *first;
            ++first;
            ++result;
        }
        return result;
    }
    void erase(ERegionId* first, ERegionId* last) { _M_finish = copy(last, _M_finish, first); }
    void clear() { erase(_M_start, _M_finish); }
    static ERegionId* uninitialized_copy(ERegionId* first, ERegionId* last, ERegionId* result)
    {
        ERegionId* cur = result;
        for (; first != last; ++first, ++cur)
            if (cur)
                *cur = *first;
        return cur;
    }
    static const unsigned long& max_ref(const unsigned long& a, const unsigned long& b) { return a < b ? b : a; }
    __forceinline void _M_insert_overflow(ERegionId* pos, const ERegionId& x, unsigned long fillLength)
    {
        unsigned long oldSize = size();
        unsigned long len = oldSize + max_ref(oldSize, fillLength);
        ERegionId* newStart = len ? (ERegionId*)malloc(len * sizeof(ERegionId)) : 0;
        ERegionId* newFinish = uninitialized_copy(_M_start, pos, newStart);
        if (newFinish)
            *newFinish = x;
        ++newFinish;
        if (_M_start)
            free(_M_start);
        _M_end_of_storage = newStart + len;
        _M_start = newStart;
        _M_finish = newFinish;
    }
    __forceinline void push_back(const ERegionId& x)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish)
                *_M_finish = x;
            ++_M_finish;
        } else {
            _M_insert_overflow(_M_finish, x, 1);
        }
    }
};
class CTCBase {
public:
    virtual void Copy(const CTCBase& other);         // 0x006868D0 (slot 12)
};
class CTCInventoryMap : public CTCBase {
public:
    unsigned char     _pad04[0x150 - 0x04];
    CWideStringVector Names150;                      // +0x150
    CWideStringVector Names15C;                      // +0x15C
    CWideStringVector Names168;                      // +0x168
    CIdVector         Ids174;                        // +0x174
    void Copy(const CTCBase& other);
};
void CTCInventoryMap::Copy(const CTCBase& other)
{
    const CTCInventoryMap& src = static_cast<const CTCInventoryMap&>(other);
    Names150.clear();
    Names15C.clear();
    Names168.clear();
    Ids174.clear();
    for (unsigned long i = 0; i < src.Names150.size(); ++i)
        Names150.push_back(src.Names150[i]);
    for (unsigned long i = 0; i < src.Names15C.size(); ++i)
        Names15C.push_back(src.Names15C[i]);
    for (unsigned long i = 0; i < src.Names168.size(); ++i)
        Names168.push_back(src.Names168[i]);
    for (unsigned long i = 0; i < src.Ids174.size(); ++i)
        Ids174.push_back(src.Ids174[i]);
    CTCBase::Copy(other);
}
