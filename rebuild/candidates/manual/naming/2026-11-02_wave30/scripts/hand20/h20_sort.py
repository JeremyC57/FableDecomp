# h20_sort.py: naming wave 20, family sort -- the std::sort chain over the CScriptManager binding table (chunk 686's __introsort_loop
# 0x00CBDAD0 names its callees by their real STLport 4.6 instantiations; the landed callers / siblings bind invented stand-ins):
#   00cbdf70 sort<pair<CCharString,CCountedPointer<CEntityScriptBindingBase> >*, CCompareString<...> > (was Sort_SElement12_00cbdf70, binding
#            IntrosortLoop_00cbdad0 / FinalInsertionSort_00cbd420);
#   00cbd420 __final_insertion_sort<...> (was FinalInsertionSort_SElement12_00cbd420 binding InsertionSort_00cbcf70 / UnguardedInsertionSortAux_00cbb2d0);
#   00cbb2d0 __unguarded_insertion_sort_aux<...> (was a non-template spelling over an invented pair<CCharString, CCountedPointer<CCountedObject> > /
#            SNamedCountedEntryLess, binding __unguarded_linear_insert 0x00CBA9B0 under the same invented types);
#   00cb9c50 the pair's copy constructor ??0?$pair@VCCharString@@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@std@@QAE@ABU01@@Z
#            (FableWin.pdb; named by the six landed chunk-686 / chunk-685 rows; was the descriptive SScriptBindingPair).
# All byte-identical only (recheck); the element / comparator model is the landed 0x00CBDAD0 text.
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
_X = set()
_xp = os.path.join(he.W, 'runs', 'excl_sort.txt')
if os.path.exists(_xp): _X = set(l.split()[0] for l in open(_xp) if l.strip())

MODEL = '''void __cdecl operator delete(void* block);                 // 0x00BFE9BC
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
%s};
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
'''
PAIR_CTOR = '    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }\n'

SORT = '''// std::sort(std::pair<CCharString, CCountedPointer<CEntityScriptBindingBase> >* first, std::pair<CCharString, CCountedPointer<CEntityScriptBindingBase> >* last, CCompareString<CCountedPointer<CEntityScriptBindingBase> > comp) -- introsort loop to depth 2*lg(n), then the final insertion sort
// Leaf: ??$sort@PAU?$pair@VCCharString@@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@std@@V?$CCompareString@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@@@std@@YIXPAU?$pair@VCCharString@@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@0@0V?$CCompareString@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBDF70, oracle length 82 bytes.
// Identity (naming wave 20, chunk 686): STLport 4.6 sort (stl/_algo.c) over the 12-byte pair<CCharString, CCountedPointer<CEntityScriptBindingBase> >
//   with CCompareString -- the caller CVectorMap<CCharString, CCountedPointer<CEntityScriptBindingBase>, CCompareString<...> >::Sort 0x00CB8930
//   (landed) binds this spelling; the callees are the landed __introsort_loop 0x00CBDAD0 (chunk 686, the same instantiation) and
//   __final_insertion_sort 0x00CBD420. Was the descriptive Sort_SElement12_00cbdf70 over SElement12 / SCompare stand-ins (one of 15 bodies
//   of this shape); the comparator is the empty CCompareString passed by value in a 4-byte slot.
// Retail bytes:  push ebx ; push edi ; mov ebx,edx ; mov edi,ecx ; cmp edi,ebx ; je E ; (n = (last - first) / 12) ; xor ecx,ecx ; cmp eax,1 ; je K ; L: sar eax,1 ; inc ecx ; cmp eax,1 ; jne L ; K: push [comp] ; push 0 ; mov edx,ebx ; mov ecx,edi ; call __introsort_loop ; push [comp] ; mov edx,ebx ; mov ecx,edi ; call __final_insertion_sort ; E: pop edi ; pop ebx ; ret 4
// /Gr TU (__fastcall: first in ecx, last in edx, comp on the stack); `__lg` is the shift-count loop, its result int (the iterator difference).
''' + MODEL % '' + '''template <class _Size> inline _Size __lg(_Size __n)
{
    _Size __k;
    for (__k = 0; __n != 1; __n >>= 1)
        ++__k;
    return __k;
}
template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp);   // 0x00CBDAD0
template <class _RandomAccessIter, class _Compare>
void __fastcall __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);                          // 0x00CBD420
template <class _RandomAccessIter, class _Compare>
void __fastcall sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__first != __last) {
        __introsort_loop(__first, __last, (typename iterator_traits<_RandomAccessIter>::value_type*)0, __lg(__last - __first) * 2, __comp);   // _STLP_VALUE_TYPE
        __final_insertion_sort(__first, __last, __comp);
    }
}
template void __fastcall sort<CScriptBindingPair*, CScriptBindingLess>(CScriptBindingPair*, CScriptBindingPair*, CScriptBindingLess);
}
'''

FINAL = '''// std::__final_insertion_sort(std::pair<CCharString, CCountedPointer<CEntityScriptBindingBase> >* first, std::pair<CCharString, CCountedPointer<CEntityScriptBindingBase> >* last, CCompareString<CCountedPointer<CEntityScriptBindingBase> > comp) -- insertion sort of the first 16, unguarded insertion sort of the rest
// Leaf: ??$__final_insertion_sort@PAU?$pair@VCCharString@@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@std@@V?$CCompareString@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@@@std@@YIXPAU?$pair@VCCharString@@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@0@0V?$CCompareString@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBD420, oracle length 80 bytes.
// Identity (naming wave 20, chunk 686): STLport 4.6 __final_insertion_sort (stl/_algo.c) of the sort<> 0x00CBDF70 over the 12-byte
//   pair<CCharString, CCountedPointer<CEntityScriptBindingBase> > with CCompareString; callees the landed __insertion_sort 0x00CBCF70 (the
//   same instantiation, its own leaf) and __unguarded_insertion_sort_aux 0x00CBB2D0. Was the descriptive FinalInsertionSort_SElement12_00cbd420
//   over SElement12 / SCompare stand-ins (one of 25 bodies of this shape).
// Retail bytes:  push esi ; mov esi,edx ; sub edx,ecx ; (divide by 12) ; cmp eax,10h ; jle S ; push ebx ; mov ebx,[comp] ; push edi ; lea edi,[ecx+0xc0] ; push ebx ; mov edx,edi ; call __insertion_sort ; push ebx ; push 0 ; mov edx,esi ; mov ecx,edi ; call __unguarded_insertion_sort_aux ; pop edi ; pop ebx ; pop esi ; ret 4 ; S: push [comp] ; mov edx,esi ; call __insertion_sort ; pop esi ; ret 4
// /Gr TU (__fastcall: first in ecx, last in edx, comp on the stack).
''' + MODEL % '' + '''template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);                                 // 0x00CBCF70
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp);          // 0x00CBB2D0
template <class _RandomAccessIter, class _Compare>
inline void __fastcall __unguarded_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    __unguarded_insertion_sort_aux(__first, __last, (typename iterator_traits<_RandomAccessIter>::value_type*)0, __comp);   // _STLP_VALUE_TYPE
}
template <class _RandomAccessIter, class _Compare>
void __fastcall __final_insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__last - __first > __stl_threshold) {
        __insertion_sort(__first, __first + __stl_threshold, __comp);
        __unguarded_insertion_sort(__first + __stl_threshold, __last, __comp);
    }
    else
        __insertion_sort(__first, __last, __comp);
}
template void __fastcall __final_insertion_sort<CScriptBindingPair*, CScriptBindingLess>(CScriptBindingPair*, CScriptBindingPair*, CScriptBindingLess);
}
'''

AUX = '''// std::__unguarded_insertion_sort_aux(std::pair<CCharString, CCountedPointer<CEntityScriptBindingBase> >* first, std::pair<CCharString, CCountedPointer<CEntityScriptBindingBase> >* last, std::pair<CCharString, CCountedPointer<CEntityScriptBindingBase> >*, CCompareString<CCountedPointer<CEntityScriptBindingBase> > comp) -- unguarded insertion sort of the (name, binding) pairs
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
''' + MODEL % PAIR_CTOR + '''template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00CBA9B0
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}
template void __fastcall __unguarded_insertion_sort_aux<CScriptBindingPair*, CScriptBindingPair, CScriptBindingLess>(CScriptBindingPair*, CScriptBindingPair*, CScriptBindingPair*, CScriptBindingLess);
}
'''

PAIR = '''// std::pair<CCharString, CCountedPointer<CEntityScriptBindingBase> >::pair(const std::pair<CCharString, CCountedPointer<CEntityScriptBindingBase> >& other) -- copy constructor of the (binding name, entity script binding) record: out-of-line string copy, add-ref copy of the counted pointer
// Leaf: ??0?$pair@VCCharString@@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@std@@QAE@ABU01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB9C50, length 39 bytes.
// Identity (naming wave 20, chunk 686): FableWin.pdb ??0?$pair@VCCharString@@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@std@@QAE@ABU01@@Z;
//   the landed __introsort_loop 0x00CBDAD0 (chunk 686), __median 0x00CBA620, swap 0x00CBA8E0, __partial_sort 0x00CBD0F0, __linear_insert
//   0x00CBC3C0 and __insertion_sort 0x00CBCF70 of the CScriptManager binding table all call it under this spelling. Was the descriptive
//   SScriptBindingPair (the manifest "CScriptGameResourceObjectScriptedThingBase" is propagated).
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30 (on +0, `other` pushed).
// Shape: no vtable store anywhere, so the record is not polymorphic: the first member is a CCharString copied out of line, the second
//   a counted pointer {Data +4, Info +8} copied by its inline copy constructor with the count block add-ref'd in place (`inc dword ptr [eax]`).
class CCPPointerInfo { public:
    long m_count;                                     // +0x00 shared reference count
};
class CEntityScriptBindingBase;
class CCharString { public:
    char* m_pData;
    CCharString(const CCharString& other);            // 0x0099EC30
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->m_count;
    }
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                        // +0x00 binding name
    _T2 second;                                       // +0x04 Data / +0x08 Info
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
template struct pair<CCharString, CCountedPointer<CEntityScriptBindingBase> >;
}
'''
for a, txt in (('00cbdf70', SORT), ('00cbd420', FINAL), ('00cbb2d0', AUX), ('00cb9c50', PAIR)):
    if a in _X: continue
    he.put(a, 'src', txt)
# the tests are self-contained models: cosmetic respells of the stand-in names they echo
if '00cbb2d0' not in _X:
    he.resub('00cbb2d0', 'tst', r'\bCCountedObject\b', 'CEntityScriptBindingBase')
    he.resub('00cbb2d0', 'tst', r'\bSNamedCountedEntryLess\b', 'CScriptBindingLess')
    he.resub('00cbb2d0', 'tst', r'\bSNamedCountedEntry\b', 'CScriptBindingPair')
if '00cb9c50' not in _X:
    he.resub('00cb9c50', 'tst', r'\bSScriptBindingPair\b', 'CScriptBindingPair')
