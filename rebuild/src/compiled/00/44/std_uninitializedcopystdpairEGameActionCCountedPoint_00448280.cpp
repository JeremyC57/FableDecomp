// std::__uninitialized_copy(P* first, P* last, P* result, const __false_type&) -- STLport: copy-construct each
//   pair<EGameAction, CCountedPointer<const CInputTypeBase> > into raw storage (placement new, so a null destination is skipped; the counted
//   pointer bumps Info->RefCount), return the end of the copy
// Leaf: ??$__uninitialized_copy@PAU?$pair@W4EGameAction@@V?$CCountedPointer@$$CBVCInputTypeBase@@@@@std@@PAU12@@std@@YIPAU?$pair@W4EGameAction@@V?$CCountedPointer@$$CBVCInputTypeBase@@@@@0@PAU10@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00448280, 56 bytes, __fastcall (first ecx, last edx), `ret 8`.
// Identity: the row was catalogued "std::_Uninit_copy<... pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> > ...>"
//   (Dinkumware spelling, wrong element). Its callers are the landed vector<pair<EGameAction, CCountedPointer<const CInputTypeBase> > >::_M_insert_overflow
//   0x00449130 (head and tail copies) only; Ego_r has CVectorMap<EGameAction, CCountedPointer<const CInputTypeBase> >. Family pass 2 (agent951 G055) agrees; retail has no
//   ICF, so each of the seven byte-identical bodies is its own instantiation.
// Model: the caller's own model (landed 0x00449130): CCountedPointer's copy constructor, STLport pair (a struct), `_Construct`
//   by placement new, `__uninitialized_copy` as a __fastcall template.
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
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
typedef pair<EGameAction, CCountedPointer<const CInputTypeBase> > _Pair;
template _Pair* __fastcall __uninitialized_copy<_Pair*, _Pair*>(_Pair*, _Pair*, _Pair*, const __false_type&);
}
