// std::vector::_M_insert_overflow(CCombatRing* position, const CCombatRing& x, const __false_type&, unsigned int fill_len, bool atend) -- grow a defending combatant's combat-ring vector and insert fill_len copies of x
// Leaf: ?_M_insert_overflow@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006F4750, 250 bytes, `ret 0x14`.
// Label correction: catalog "Vector_InsertRepeatElements_Complex" -> STLport 4.x
//   vector<CDefendingCombatantInfo::CCombatRing>::_M_insert_overflow (the landed __uninitialized_copy 0x006F40A0 names
//   the element; 28 bytes: the 0x92492493 / `add edx,ecx ; sar edx,4` divide, `imul 1Ch`).
// Element: CCombatRing (Ego_r.pdb; retail 0x1c): RotationAngle, DeltaRotation, Index, RingThickness, then
//   Prongs = vector<CProng> at +0x10 with its out-of-line copy constructor 0x006F20A0.
// Callees: malloc 0x00BFEA0E, free 0x00BFEA14, __uninitialized_copy 0x006F40A0, __uninitialized_fill_n 0x006F40F0,
//   vector<CProng> copy constructor 0x006F20A0, __destroy_aux 0x006F44E0.
// Shape: STLport `_M_insert_overflow` (see 0x006FC6A0): len = size + max(size, fill_len) via the const-ref `max`,
//   allocate, copy the head, construct one element in place (four dword copies then the prong vector's copy
//   constructor) or fill_n, copy the tail unless atend, _M_clear, _M_set. The empty `__false_type` tags are homed in
//   the high argument slot; the tag-taking callees are same-TU noinline stand-ins.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* block);           // 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* where) { return where; }

namespace CDefendingCombatantInfo {
class CProng;
class CProngVector {                           // std::vector<CProng> (retail STLport vector, 12 bytes)
public:
    CProng* _M_start;
    CProng* _M_finish;
    CProng* _M_end_of_storage;
    CProngVector(const CProngVector& x);       // 0x006F20A0
    ~CProngVector() { if (_M_start) free(_M_start); }
};
class CCombatRing {                            // Ego_r.pdb, retail 0x1c
public:
    float RotationAngle;                       // +0x00
    float DeltaRotation;                       // +0x04
    long  Index;                               // +0x08
    float RingThickness;                       // +0x0C
    CProngVector Prongs;                       // +0x10
};
}

namespace std {

struct __false_type { __false_type() {} };

template<class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }

template<class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template<class T> class allocator {
public:
    allocator() {}
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    void deallocate(T* p, unsigned int) { if (p) free(p); }
};

#pragma auto_inline(off)
// Same-TU stand-ins (real bodies: 0x006F40A0, 0x006F40F0, 0x006F44E0).
template<class T> __declspec(noinline) T* __fastcall __uninitialized_copy(const T* first, const T* last, T* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        _Construct(result, *first);
    return result;
}
template<class T> __declspec(noinline) T* __fastcall __uninitialized_fill_n(T* first, unsigned int n, const T& x, const __false_type&)
{
    for (; n > 0; --n, ++first)
        _Construct(first, x);
    return first;
}
template<class T> __declspec(noinline) void __fastcall __destroy_aux(T* first, T* last, const __false_type&)
{
    for (; first != last; ++first)
        first->~T();
}
#pragma auto_inline(on)

template<class T> inline void _Destroy(T* first, T* last) { __destroy_aux(first, last, __false_type()); }

template<class T, class Alloc = allocator<T> > class vector {
public:
    T* _M_start;                                // +0x00
    T* _M_finish;                               // +0x04
    T* _M_end_of_storage;                       // +0x08
    Alloc _M_alloc() const { return Alloc(); }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    void _M_clear()
    {
        _Destroy(_M_start, _M_finish);
        _M_alloc().deallocate(_M_start, (unsigned int)(_M_end_of_storage - _M_start));
    }
    void _M_set(T* s, T* f, T* e)
    {
        _M_start = s;
        _M_finish = f;
        _M_end_of_storage = e;
    }
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend);
};

template<class T, class Alloc>
void vector<T, Alloc>::_M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend)
{
    const unsigned int old_size = size();
    const unsigned int len = old_size + (max)(old_size, fill_len);

    T* new_start = _M_alloc().allocate(len);
    T* new_finish = __uninitialized_copy(_M_start, position, new_start, __false_type());
    if (fill_len == 1) {
        _Construct(new_finish, x);
        ++new_finish;
    }
    else
        new_finish = __uninitialized_fill_n(new_finish, fill_len, x, __false_type());
    if (!atend)
        new_finish = __uninitialized_copy(position, _M_finish, new_finish, __false_type());
    _M_clear();
    _M_set(new_start, new_finish, new_start + len);
}

template class vector<CDefendingCombatantInfo::CCombatRing>;

}