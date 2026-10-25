// _STL::__make_heap(CBindingPair* first, CBindingPair* last, CBindingLess comp, CBindingPair*, int*) -- heapify the (name, entity script binding) range by sifting every parent down from the last one
// Leaf: ??$__make_heap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBBC00, 117 bytes (`ret 0Ch`).
// TU: the scripting library's STLport 4.x instantiations (0x00CB6xxx-0x00CBDxxx); the game is built /Gr, so the
//   algorithm templates are __fastcall (first two pointer arguments in ecx/edx, callee pops the rest).
// Identity: STLport __make_heap over (name, entity script binding) = _STL::pair<CCharString, CCountedPointer<CEntityScriptBindingBase> > (0x0C bytes, CCompareString -> CCharString::FastCompare 0x009E6530) (`imul 2AAAAAABh ; sar 1` = /12);
//   callers 0x00CBBF79, __partial_sort 0x00CBD101. The manifest label "EntityScriptBinding_BuildHeapify" is descriptive.
// Callees: __adjust_heap 0x00CBB4D0, CCharString copy constructor 0x0099EC30.
// Shape: as 0x00CBAAB0 for the 12-byte element.
void __cdecl operator delete(void* block);                 // 0x00BFE9BC
class CCharString {
public:
    void* m_pStringData;
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
    CCharString& operator=(const CCharString& other);      // 0x0099EFB0
    bool FastCompare(const CCharString& other) const;      // 0x009E6530
};
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
class CEntityScriptBindingBase;
namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
template <class V> class CCompareString {                  // empty, passed by value (4-byte slot)
public:
    bool operator()(const _STL::pair<CCharString, V>& a, const _STL::pair<CCharString, V>& b) const { return a.first.FastCompare(b.first); }
};
typedef _STL::pair<CCharString, CCountedPointer<CEntityScriptBindingBase> > CBindingPair;   // 12 bytes
typedef CCompareString<CCountedPointer<CEntityScriptBindingBase> > CBindingLess;
namespace _STL {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);   // 0x00CBB4D0

template <class _RandomAccessIterator, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Tp*, _Distance*)
{
    if (__last - __first < 2) return;
    _Distance __len = __last - __first;
    _Distance __parent = (__len - 2) / 2;
    for (;;) {
        __adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)), __comp);
        if (__parent == 0) return;
        __parent--;
    }
}
template void __fastcall __make_heap<CBindingPair*, CBindingLess, CBindingPair, int>(CBindingPair*, CBindingPair*, CBindingLess, CBindingPair*, int*);
}