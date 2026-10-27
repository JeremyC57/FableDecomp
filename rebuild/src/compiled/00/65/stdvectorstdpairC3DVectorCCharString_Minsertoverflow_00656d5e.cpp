// std::vector<std::pair<C3DVector, CCharString>>::_M_insert_overflow(std::pair<C3DVector, CCharString>* position, const std::pair<C3DVector, CCharString>& x, const __false_type&, unsigned int fillLength, bool atEnd) -- STLport growth path: allocate old+max(old,n), copy the head, n copies of x and the tail unless inserting at the end, clear the old block, adopt the new one
// Leaf: ?_M_insert_overflow@?$vector@U?$pair@VC3DVector@@VCCharString@@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00656D5E, 174 bytes, thiscall `ret 14h`; size TU with a frame pointer.
// Identity (probable): its copy ctor 0x00654DD4 copies three dwords then a CCharString (0x0099EC30); the push_back 0x00656EF2 is called only by 0x0064DD9A, the NPlayerGui::CDrawMiniMap region-exit builder (SetRegion / ToggleCentering) -- Ego_r.pdb CDrawMiniMap::RegionExits is vector<pair<C3DVector, CCharString> >; caller push_back 0x00656EF2.
// Callees: malloc 0x00BFEA0E, __uninitialized_copy 0x006555CE (__fastcall, head and tail), the copy constructor 0x00654DD4
//   (fillLength == 1), __uninitialized_fill_n 0x006555F9 (__fastcall), _M_clear 0x006568CF.
// Shape (family K121, representative 0x004E3526): STLport 4.6 `_M_insert_overflow(.., const __false_type&, ..)` in a frame/size
//   TU (the K102 shape with the copy/fill helpers out of line: their STLport bodies sit in this TU as noinline stand-ins, as in
//   the landed 0x007B6EF0 / 0x005BC0FB rows, so VC7.1 homes each tag in the top byte of the atEnd slot [ebp+1Bh]).
#pragma optimize("y", off)
#pragma optimize("s", on)
extern "C" void* __cdecl malloc(unsigned int bytes);                                   // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }
class CCharString { public: void* PStringData; };
class C3DVector { public: float X, Y, Z; };
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o);                     // out of line (below)
};
}
namespace std {
struct __false_type { __false_type() { } };
template <class T> __forceinline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }
template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                                                  const __false_type&)
{                                                                                              // 0x006555CE
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{                                                                                              // 0x006555F9
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector {
public:
    T* _M_start;                    // +0x00
    T* _M_finish;                   // +0x04
    T* _M_end_of_storage;           // +0x08
    unsigned int size() const { return _M_finish - _M_start; }
    static __forceinline T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    void _M_clear();                                    // 0x006568CF
    __forceinline void _M_set(T* s, T* f, T* e) { _M_start = s; _M_finish = f; _M_end_of_storage = e; }
protected:
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fillLength, bool atEnd);
};
template <class T, class A> void vector<T, A>::_M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fillLength, bool atEnd)
{
    const unsigned int oldSize = size();
    const unsigned int length = oldSize + (max)(oldSize, fillLength);
    T* newStart = allocate(length);
    T* newFinish = __uninitialized_copy(_M_start, position, newStart, __false_type());
    if (fillLength == 1) {
        _Construct(newFinish, x);
        ++newFinish;
    }
    else
        newFinish = __uninitialized_fill_n(newFinish, fillLength, x, __false_type());
    if (!atEnd)
        newFinish = __uninitialized_copy(position, _M_finish, newFinish, __false_type());
    _M_clear();
    _M_set(newStart, newFinish, newStart + length);
}
template void vector<std::pair<C3DVector, CCharString> >::_M_insert_overflow(std::pair<C3DVector, CCharString>*, const std::pair<C3DVector, CCharString>&, const __false_type&, unsigned int, bool);
}