// std::__copy(_E* first, _E* last, _E* result, const random_access_iterator_tag&, int*) -- (_E = pair<EGameAction, CCountedPointer<const CInputTypeBase> >) assign [first, last) onto result
// Leaf: ??$__copy@PAU?$pair@W4EGameAction@@V?$CCountedPointer@$$CBVCInputTypeBase@@@@@std@@PAU12@H@std@@YIPAU?$pair@W4EGameAction@@V?$CCountedPointer@$$CBVCInputTypeBase@@@@@0@PAU10@00ABUrandom_access_iterator_tag@0@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004487C0, 145 bytes, `__fastcall` (first ecx, last edx), `ret 0Ch`.
// Identity (proven): CGamePlayerInterface::SetControlScheme 0x00446EF0 erases the CVectorMap at +0x7A8 with it: Ego_r.pdb CGamePlayerInterface::Controls (+1960 = 0x7A8) is CVectorMap<EGameAction, CCountedPointer<const CInputTypeBase> >; the other callers are that map's sort helpers in the same module.
// Callees: the count block's delete function (`call [eax+4]` with the data in ecx), operator delete 0x00BFE9BC.
// Shape (family M029, representative 0x004487C0): STLport `for (_Distance __n = __last - __first; __n > 0; --__n) { *__result =
//   *__first; ++__first; ++__result; } return __result;` -- 12-byte elements (the 0x2AAAAAAB divide); CCountedPointer::operator=
//   reads the source Info then Data into locals before the self test.
void __cdecl operator delete(void* p);
class CCPPointerInfo {
public:
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void*);
    void* Data;
};
template <class T> class CCountedPointer {
public:
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info) ++info->RefCount;
        }
        return *this;
    }
    T* Data;
    CCPPointerInfo* Info;
};
class CInputTypeBase;
enum EGameAction { GAME_ACTION_NULL = 0 };
namespace std {
struct random_access_iterator_tag { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
typedef pair<EGameAction, CCountedPointer<const CInputTypeBase> > _E;
template _E* __fastcall __copy<_E*, _E*, int>(_E*, _E*, _E*, const random_access_iterator_tag&, int*);
}