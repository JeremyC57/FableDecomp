// std::uninitialized_fill_n(CCountedPointer<C3DClothPrimitive>* first, unsigned int n, const CCountedPointer<C3DClothPrimitive>& x) -- copy-construct x into n raw slots, return the end
// Leaf: ??$uninitialized_fill_n@PAV?$CCountedPointer@VC3DClothPrimitive@@@@IV1@@std@@YIPAV?$CCountedPointer@VC3DClothPrimitive@@@@PAV1@IABV1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00a959e0, 46 bytes, /Gr `__fastcall` (first ecx, n edx, x on the stack), `ret 4`.
// Identity (proven): its caller is vector<CCountedPointer<C3DClothPrimitive> >::_M_fill_insert 0x00A97E40 (bound), whose copies go through __uninitialized_copy<CCountedPointer<C3DClothPrimitive>*> 0x00A95750 (bound).
// Body: STLport 4.6 `return __uninitialized_fill_n(first, n, x, __false_type());` with the inner loop inline -- placement new's null
//   test, CCountedPointer's copy constructor (Data, Info, `if (Info) ++Info->RefCount`).
class C3DClothPrimitive;
class CCPPointerInfo {                                     // 12 bytes
public:
    unsigned long RefCount;                                // +0x00
    void (__fastcall* DeleteFunc)(void* data);             // +0x04
    void* Data;                                            // +0x08
};
template <class T> class CCountedPointer {                 // 8 bytes
public:
    T*              Data;                                  // +0x00
    CCPPointerInfo* Info;                                  // +0x04
    CCountedPointer(const CCountedPointer<T>& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
};
inline void* operator new(unsigned int, void* __p) { return __p; }
namespace std {
struct __false_type { };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new ((void*)__p) _T1(__val); }
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}
template CCountedPointer<C3DClothPrimitive>* __fastcall uninitialized_fill_n(CCountedPointer<C3DClothPrimitive>*, unsigned int, const CCountedPointer<C3DClothPrimitive>&);
}