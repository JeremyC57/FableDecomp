// std::__insertion_sort(CActiveScriptPair* first, CActiveScriptPair* last, CActiveScriptLess comp) -- plain insertion sort of a short (UID, active entity script) range
// Leaf: ??$__insertion_sort@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBCF20, 80 bytes (`ret 4`).
// TU: the scripting library's STLport 4.x instantiations (0x00CB6xxx-0x00CBDxxx); the game is built /Gr, so the
//   algorithm templates are __fastcall (first two pointer arguments in ecx/edx, callee pops the rest).
// Identity: STLport __insertion_sort over (UID, active entity script) = std::pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> > (0x10 bytes); callers
//   0x00CBD3F0, 0x00CBD40E, 0x00CBDF3D, 0x00CBDF57 (__final_insertion_sort). The manifest label "PriorityQueue_BuildHeap"
//   is propagated and wrong.
// Callees: __linear_insert 0x00CBC330.
// Shape: `if (first == last) return; for (i = first + 1; i != last; ++i) __linear_insert(first, i, *i, comp);` (val by value).
//   CCountedPointer is modelled with the release form the D rows need (decrement, re-read, test, then Data = Info = 0):
//   VC7.1 emits the same bytes for this row as with the short `if (Info && --Info->RefCount == 0)` form.
void __cdecl operator delete(void* block);                 // 0x00BFE9BC
class CCPPointerInfo { public:                                    // CCountedPointer count block
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
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
template <class K, class V> class CKeyPairCompareLess {    // empty, passed by value (4-byte slot)
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
typedef std::pair<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> > CActiveScriptPair;   // 16 bytes
typedef CKeyPairCompareLess<unsigned __int64, CCountedPointer<CActiveEntityScriptBase> > CActiveScriptLess;
namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00CBC330

template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first == __last) return;
    for (_RandomAccessIter __i = __first + 1; __i != __last; ++__i)
        __linear_insert(__first, __i, *__i, __comp);
}
template void __fastcall __insertion_sort<CActiveScriptPair*, CActiveScriptLess>(CActiveScriptPair*, CActiveScriptPair*, CActiveScriptLess);
}