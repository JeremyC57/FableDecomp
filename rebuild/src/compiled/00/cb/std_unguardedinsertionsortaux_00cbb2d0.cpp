// std::__unguarded_insertion_sort_aux(std::pair<CCharString, CCountedPointer<CEntityScriptBindingBase> >* first, std::pair<CCharString, CCountedPointer<CEntityScriptBindingBase> >* last, std::pair<CCharString, CCountedPointer<CEntityScriptBindingBase> >*, CCompareString<CCountedPointer<CEntityScriptBindingBase> > comp) -- unguarded insertion sort of the (name, binding) pairs
// Leaf: ??$__unguarded_insertion_sort_aux@PAU?$pair@VCCharString@@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@std@@U12@V?$CCompareString@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@@@std@@YIXPAU?$pair@VCCharString@@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@0@0PAU10@V?$CCompareString@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBB2D0, length 69 bytes.
// Identity (naming wave 20, chunk 686): STLport 4.6 __unguarded_insertion_sort_aux (stl/_algo.c) of the sort<> 0x00CBDF70 over the 12-byte
//   pair<CCharString, CCountedPointer<CEntityScriptBindingBase> > with CCompareString: called by the landed __final_insertion_sort 0x00CBD420
//   (the same instantiation) and from the partition area 0x00CBB917; its callee is the landed __unguarded_linear_insert 0x00CBA9B0 (its own
//   leaf names this pair and comparator). Was a non-template spelling over an invented pair<CCharString, CCountedPointer<CCountedObject> >
//   with an SNamedCountedEntryLess comparator (the catalog "_Destroy_range<std::allocator<CSpeechFrame>>" is propagated).
// Convention: LTCG register form -- first in ecx, last in edx; the unused value-type pointer and the 4-byte empty comparator on the stack (`ret 8`).
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30, std::__unguarded_linear_insert @ 0x00CBA9B0.
// Shape: the value is built in the argument slot by the memberwise pair copy constructor (inlined here); the counted pointer copy
//   constructor copies both words and adds a reference on the block under a null test (`inc dword ptr [eax]`).
void __cdecl operator delete(void* block);                 // 0x00BFE9BC
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
    bool operator<(const CCharString& other) const;        // 0x009E6530 ??MCCharString@@QBE_NABV0@@Z (FableWin.pdb)
};
class CCPPointerInfo { public:
    long  RefCount;
    void  (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
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
};
class CEntityScriptBindingBase;
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}
template <class T> class CCompareString {
public:
    bool operator()(const std::pair<CCharString, T>& a, const std::pair<CCharString, T>& b) const { return a.first < b.first; }
};
typedef std::pair<CCharString, CCountedPointer<CEntityScriptBindingBase> > CScriptBindingPair;
typedef CCompareString<CCountedPointer<CEntityScriptBindingBase> > CScriptBindingLess;
namespace std {
template <class _Iterator> struct iterator_traits;
template <class _Tp> struct iterator_traits<_Tp*> {
    typedef _Tp value_type;
};
const int __stl_threshold = 16;
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00CBA9B0
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}
template void __fastcall __unguarded_insertion_sort_aux<CScriptBindingPair*, CScriptBindingPair, CScriptBindingLess>(CScriptBindingPair*, CScriptBindingPair*, CScriptBindingPair*, CScriptBindingLess);
}
