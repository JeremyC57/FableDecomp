// std::uninitialized_fill_n(CCountedPointer<CParticleSimulation>* first, unsigned int n, const CCountedPointer<CParticleSimulation>& x) -- copy-construct x into n raw slots, return the end
// Leaf: ??$uninitialized_fill_n@PAV?$CCountedPointer@VCParticleSimulation@@@@IV1@@std@@YIPAV?$CCountedPointer@VCParticleSimulation@@@@PAV1@IABV1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00b82dc0, 46 bytes, /Gr `__fastcall` (first ecx, n edx, x on the stack), `ret 4`.
// Identity (probable): its caller, the fill insert 0x00B83B00 (unbound), copies through __uninitialized_copy<CCountedPointer<CParticleSimulation>*> 0x00B82CC0 (bound).
// Body: STLport 4.6 `return __uninitialized_fill_n(first, n, x, __false_type());` with the inner loop inline -- placement new's null
//   test, CCountedPointer's copy constructor (Data, Info, `if (Info) ++Info->RefCount`).
class CParticleSimulation;
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
template CCountedPointer<CParticleSimulation>* __fastcall uninitialized_fill_n(CCountedPointer<CParticleSimulation>*, unsigned int, const CCountedPointer<CParticleSimulation>&);
}