// _STL::__unguarded_insertion_sort_aux(CActiveScriptPair* first, CActiveScriptPair* last, CActiveScriptPair*, CActiveScriptLess comp) -- insertion-sort the tail of an introsorted (UID, active entity script) range without bound checks
// Leaf: ??$__unguarded_insertion_sort_aux@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBB270, 70 bytes (`ret 8`).
// TU: the scripting library's STLport 4.x instantiations (0x00CB6xxx-0x00CBDxxx); the game is built /Gr, so the
//   algorithm templates are __fastcall (first two pointer arguments in ecx/edx, callee pops the rest).
// Identity: STLport __unguarded_insertion_sort_aux over (UID, active entity script) = _STL::pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> > (0x10 bytes);
//   callers 0x00CBB857, 0x00CBD3FC, 0x00CBDF49 (final insertion sorts). The manifest label
//   "_Copy_impl<CNavigationPosition*,...>" is propagated and wrong.
// Callees: __unguarded_linear_insert 0x00CBA840.
// Shape: the by-value 16-byte temporary is copy-built in the argument slot (inline counted-pointer copy, `inc [ecx]`).
//   CCountedPointer is modelled with the release form the D rows need (decrement, re-read, test, then Data = Info = 0):
//   VC7.1 emits the same bytes for this row as with the short `if (Info && --Info->RefCount == 0)` form.
void __cdecl operator delete(void* block);                 // 0x00BFE9BC
struct CCPPointerInfo {                                    // CCountedPointer count block
    long  RefCount;                                        // +0x00
    void  (__fastcall* DeleteFunc)(void* data);            // +0x04
    void* Data;                                            // +0x08
};
template <class T> class CCountedPointer {
public:
    T*              Data;                                  // +0x00
    CCPPointerInfo* Info;                                  // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        T* data = other.Data;
        CCPPointerInfo* info = other.Info;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    operator delete(Info);
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
};
class CActiveEntityScriptBase;
namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
template <class K, class V> class CKeyPairCompareLess {    // empty, passed by value (4-byte slot)
public:
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};
typedef _STL::pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> > CActiveScriptPair;   // 16 bytes
typedef CKeyPairCompareLess<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> > CActiveScriptLess;
namespace _STL {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00CBA840

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}
template void __fastcall __unguarded_insertion_sort_aux<CActiveScriptPair*, CActiveScriptPair, CActiveScriptLess>(CActiveScriptPair*, CActiveScriptPair*, CActiveScriptPair*, CActiveScriptLess);
}