// std::__unguarded_linear_insert(CBindingPair* last, CBindingPair val, CBindingLess comp) -- shift every earlier (name, entity script binding) pair whose name sorts after val's up one slot and drop val into the gap
// Leaf: ??$__unguarded_linear_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBA9B0, 247 bytes, __fastcall (last in ecx), `ret 10h` (by-value 12-byte val +
//   the comparator slot).
// Identity (proven): STLport __unguarded_linear_insert over std::pair<CCharString, CCountedPointer<CEntityScriptBindingBase> >
//   with CCompareString (CCharString::FastCompare 0x009E6530); the landed __linear_insert 0x00CBC3C0 of the same pair declares
//   0x00CBA9B0 as its __unguarded_linear_insert callee, and the landed __unguarded_insertion_sort_aux 0x00CBB2D0 calls it too.
// Label: the catalog's "_global::SpeechFrame_InsertSortedWithComparison" is a propagated label (no CSpeechFrame here).
// Callees: CCharString::FastCompare 0x009E6530, CCharString::operator= 0x0099EFB0, ~CCharString 0x0099EAE0, operator delete
//   0x00BFE9BC (count block; its delete function via +4).
// Shape: the implicit pair operator= inlines here (CCharString::operator= call plus the inline CCountedPointer::operator=);
//   val's destructor releases the counted pointer inline, zeroes Data/Info, then calls ~CCharString.
// Model: the landed __linear_insert 0x00CBC3C0 pasted (same pair, same comparator).
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
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    _RandomAccessIter __next = __last;
    --__next;
    while (__comp(__val, *__next)) {
        *__last = *__next;
        __last = __next;
        --__next;
    }
    *__last = __val;
}
template void __fastcall __unguarded_linear_insert<CBindingPair*, CBindingPair, CBindingLess>(CBindingPair*, CBindingPair, CBindingLess);
}