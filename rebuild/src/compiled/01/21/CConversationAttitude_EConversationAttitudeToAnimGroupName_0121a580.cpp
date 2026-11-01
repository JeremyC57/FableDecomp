// CConversationAttitude::$E_ConversationAttitudeToAnimGroupName() -- `dynamic initializer for 'CConversationAttitude::ConversationAttitudeToAnimGroupName'': build the static std::vector<CCharString> with 12 copies of CCharString(""), then register its atexit destructor
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121A580, 139 bytes, in `.text$yc`, reached only from the .CRT$XCU table entry
//   0x013742EC, plain `ret`. Registers the landed atexit destructor 0x01228D80 (`_$E2`, inlined ~vector<CCharString>) with
//   _atexit 0x004012BC over the vector at 0x013B8A44.
// Identity (proven, two sources):
//   (1) initializer order: FableWin.pdb runs `CConversationAttitude::GeneralAttitudeToAnimName` 03FDF2A0 ->
//       `ConversationAttitudeToAnimGroupName` 03FDF330 -> `ConversationAttitudeToAnimGroupName` 03FDF3C0 (0x90 bytes apart) ->
//       `CSubtitledMessage::MostRecentGroupQueried` -> `AttitudeSymbols`; retail's .CRT$XCU runs 0x0121A460 / 0x0121A4F0 /
//       0x0121A580 (0x90 bytes apart) -> 0x0121A610 (a default CCharString at 0x013B8A50) -> 0x0121A630 (AttitudeSymbols, landed);
//       Ego_r.pdb lists the same three CConversationAttitude initializers and atexit destructors in the same order.
//   (2) the vector's elements are assigned twelve anim-group names by the code at 0x004CD8B0 (`mov ecx,[0x013B8A44] ; push str ; call
//       CCharString::operator=`), i.e. ?Init@CConversationAttitude@@SIXXZ, and FableWin.pdb has the instantiation
//       ??0?$vector@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@QAE@IABVCCharString@@@Z at 01B6E0A0, right after
//       CConversationAttitude::Init / GetAnimName / GetAnimGroupName (01B6CEF0..01B6DC80): the statics are built by
//       STLport's vector(size_type, const T&). The landed atexit destructor 0x01228D80 already names this object.
//   The catalog row was a gap-scan placeholder (_gapscan::sub_0121a580).
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0 (the "" temporary, length -1), malloc 0x00BFEA0E
//   (allocator<CCharString>::allocate(12)), CCharString::CCharString(const CCharString&) 0x0099EC30 (placement copy per element),
//   CCharString::~CCharString 0x0099EAE0 (the temporary), _atexit 0x004012BC.
// Shape: STLport 4.6 `vector(size_type n, const T& val, const allocator_type& a = allocator_type()) : _Vector_base(n, a)
//   { _M_finish = uninitialized_fill_n(_M_start, n, val); }` -- the base's three zero stores, allocate (malloc), start /
//   finish / end stores, the __false_type fill loop (placement new null test, copy ctor), the _M_finish store, then the
//   temporary's destructor. The temporary comes from the implicit conversion through CCharString(const char*, long = -1).
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14

inline void* operator new(unsigned int, void* p) { return p; }

class CCharString {
public:
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
private:
    void* PStringData;
};

namespace std {

struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    typedef _Alloc allocator_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    _Vector_base(unsigned int __n, const _Alloc& __a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Alloc allocator_type;
    vector(size_type __n, const _Tp& __val, const allocator_type& __a = allocator_type())
        : _Vector_base<_Tp, _Alloc>(__n, __a)
    {
        this->_M_finish = uninitialized_fill_n(this->_M_start, __n, __val);
    }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};

}   // namespace std

class CConversationAttitude {                            // Ego_r.pdb: static members only
public:
    static std::vector<CCharString> ConversationAttitudeToAnimGroupName;   // 0x013B8A44
};

std::vector<CCharString> CConversationAttitude::ConversationAttitudeToAnimGroupName(12, "");   // 12 conversation attitudes