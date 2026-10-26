// std::__uninitialized_fill_n(P* first, unsigned int n, const P& x, const __false_type&) -- STLport: copy-construct n
//   copies of the pair<EGameAction, CCountedPointer<const CInputTypeBase> > x into raw storage (placement new; the counted pointer bumps
//   Info->RefCount), return first + n
// Leaf: ??$__uninitialized_fill_n@PAU?$pair@W4EGameAction@@V?$CCountedPointer@$$CBVCInputTypeBase@@@@@std@@IU12@@std@@YIPAU?$pair@W4EGameAction@@V?$CCountedPointer@$$CBVCInputTypeBase@@@@@0@PAU10@IABU10@ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004482C0, 52 bytes, __fastcall (first ecx, n edx; &x and the tag on the stack), `ret 8`.
// Identity: catalogued "std::_Uninit_fill_n<pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> >*, ...>" (source: generic pair<long, CCountedPointer<T> >) (invented / wrong element). The only caller is the landed
//   vector<pair<EGameAction, CCountedPointer<const CInputTypeBase> > >::_M_insert_overflow 0x00449130 (its fill_len != 1 branch); Ego_r has
//   CVectorMap<EGameAction, CCountedPointer<const CInputTypeBase> > (CGamePlayerInterface::Controls). Family pass 2 (agent951 G055) agrees.
// Model: the caller's own model (landed 0x00449130): STLport pair (a struct), `_Construct` by placement new.
extern "C" void* __cdecl malloc(unsigned int size);
inline void* operator new(unsigned int, void* p) { return p; }
class CCPPointerInfo {                                     // 12 bytes
public:
    unsigned long RefCount;                                // +0x00
    void (__fastcall* DeleteFunc)(void* data);             // +0x04
    void* Data;                                            // +0x08
};
template <class T> class CCountedPointer {                 // 8 bytes
public:
    T* Data;                                               // +0x00
    CCPPointerInfo* Info;                                  // +0x04
    CCountedPointer(const CCountedPointer<T>& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
};
enum EGameAction { };
class CInputTypeBase;
namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
typedef pair<EGameAction, CCountedPointer<const CInputTypeBase> > _Pair;
template _Pair* __fastcall __uninitialized_fill_n<_Pair*, unsigned int, _Pair>(_Pair*, unsigned int, const _Pair&, const __false_type&);
}
