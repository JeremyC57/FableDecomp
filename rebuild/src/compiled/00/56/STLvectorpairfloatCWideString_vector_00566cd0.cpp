// std::vector<pair<float, CWideString> >::vector(const vector& x) -- STLport vector copy constructor for the online scoreboard's (score, name) rows: allocate x.size() elements (none for an empty source), set start/finish/end of storage, copy-construct the elements through the out-of-line uninitialized copy
// Leaf: ??0?$vector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00566CD0, 87 bytes, `ret 4` (thiscall: &x).
// Callees: malloc 0x00BFEA0E (the STLport malloc allocator), `__uninitialized_copy(first, last, result, const
//   __false_type&)` 0x005654B0 (/Gr `__fastcall`; per element: the float word, then the CWideString copy ctor 0x0099B720).
// Callers: seven sites in the CTCHeroOnlineScoreboard TU (0x00568A43 .. 0x0056A6D9).
// Label correction: catalogued as "Vector_Copy_Uint"; the element is the 8-byte pair<float, CWideString> of Ego_r.pdb
//   CTCHeroOnlineScoreboard::DownloadedClanScoreboard (map<EOnlineScoreboardActivity, vector<pair<float, CWideString> > >).
// Layout: STLport `_Vector_base` = { _M_start +0, _M_finish +4, _M_end_of_storage (an _STLP_alloc_proxy over the empty
//   allocator) +8 }.
// Shape: STLport 4.x `vector(const _Self& __x) : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator()) { _M_finish =
//   __uninitialized_copy(__x._M_start, __x._M_finish, _M_start, _IsPODType()); }` with `_Vector_base(n, a) : _M_start(0),
//   _M_finish(0), _M_end_of_storage(a, 0) { _M_start = allocate(n); _M_finish = _M_start; _M_end_of_storage._M_data =
//   _M_start + n; }`: the three zero stores share the size compare's zero register, the `__false_type` temporary (a pair
//   is not POD to STLport's type traits) lands in the dead `x` argument slot (`lea edx,[esp+10h]`).
extern "C" void* __cdecl malloc(unsigned int size);                       // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                                // 0x00BFEA14

class CWideString {
public:
    void* PData;                                                          // CBasicString<wchar_t>* (4 bytes)
    CWideString(const CWideString& other);                                // 0x0099B720
    ~CWideString();                                                       // 0x0099B510
};

namespace std {
struct __false_type { __false_type() { } };

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair& p) : first(p.first), second(p.second) { }
};

template <class _Tp> class allocator {                                    // STLport allocator over the malloc allocator: empty
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {                     // the end-of-storage word over the empty allocator
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(unsigned int __n, const _Alloc& __a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    _Tp* _M_start;                                                        // +0x0
    _Tp* _M_finish;                                                       // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;               // +0x8
};

typedef pair<float, CWideString> SScoreRow;
SScoreRow* __fastcall __uninitialized_copy(const SScoreRow* first, const SScoreRow* last, SScoreRow* result,
                                           const __false_type&);          // 0x005654B0

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    allocator_type get_allocator() const { return allocator_type(); }
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    vector(const vector& __x) : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const _Tp*)__x._M_start, (const _Tp*)__x._M_finish, this->_M_start,
                                               __false_type());
    }
};

template vector<SScoreRow>::vector(const vector<SScoreRow>&);
}