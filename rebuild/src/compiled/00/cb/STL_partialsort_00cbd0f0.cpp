// std::__partial_sort(CBindingPair* first, CBindingPair* middle, CBindingPair* last, CBindingPair*, CBindingLess comp) -- partial sort of (name, entity script binding) pairs by name: heapify [first, middle), pop every smaller later pair into the heap, then sort the heap
// Leaf: ??$__partial_sort@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBD0F0, 120 bytes (`ret 0Ch`).
// TU: the scripting library's STLport 4.x instantiations (0x00CB6xxx-0x00CBDxxx); the game is built /Gr, so the
//   algorithm templates are __fastcall (first two pointer arguments in ecx/edx, callee pops the rest).
// Identity: STLport __partial_sort over (name, entity script binding) = std::pair<CCharString, CCountedPointer<CEntityScriptBindingBase> > (0x0C bytes, CCompareString -> CCharString::FastCompare 0x009E6530); the 6 callers
//   0x00CBD49C, 0x00CBDDE4..0x00CBDEB3 are the binding-map sort. The manifest label "CGameCameraManager_InitModes" is
//   propagated and wrong.
// Callees: __make_heap 0x00CBBC00, __pop_heap 0x00CBBAD0, sort_heap 0x00CBC580, CCharString::FastCompare 0x009E6530,
//   CCharString copy constructor 0x0099EC30 (the by-value `_Tp(*__i)`).
// Shape: as 0x00CB7140 for the 12-byte element.
void __cdecl operator delete(void* block);                 // 0x00BFE9BC
class CCharString {
public:
    void* m_pStringData;
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
    CCharString& operator=(const CCharString& other);      // 0x0099EFB0
    bool FastCompare(const CCharString& other) const;      // 0x009E6530
};
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
class CEntityScriptBindingBase;
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
template <class V> class CCompareString {                  // empty, passed by value (4-byte slot)
public:
    bool operator()(const std::pair<CCharString, V>& a, const std::pair<CCharString, V>& b) const { return a.first.FastCompare(b.first); }
};
typedef std::pair<CCharString, CCountedPointer<CEntityScriptBindingBase> > CBindingPair;   // 12 bytes
typedef CCompareString<CCountedPointer<CEntityScriptBindingBase> > CBindingLess;
namespace std {
template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*);   // 0x00CBBC00
template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
void __fastcall __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result, _Tp __val, _Compare __comp, _Distance*);   // 0x00CBBAD0
template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp);   // 0x00CBC580

template <class _RandomAccessIterator, class _Compare>
inline void make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, (CBindingPair*)0, (int*)0);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, (int*)0);
    sort_heap(__first, __middle, __comp);
}
template void __fastcall __partial_sort<CBindingPair*, CBindingPair, CBindingLess>(CBindingPair*, CBindingPair*, CBindingPair*, CBindingPair*, CBindingLess);
}