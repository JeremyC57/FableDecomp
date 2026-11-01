// std::__introsort_loop(std::pair<CCharString, CCountedPointer<CEntityScriptBindingBase> >* first, std::pair<CCharString, CCountedPointer<CEntityScriptBindingBase> >* last, std::pair<CCharString, CCountedPointer<CEntityScriptBindingBase> >*, int depth_limit, CCompareString<CCountedPointer<CEntityScriptBindingBase> > comp) -- STLport 4.6 introsort loop over the (name, binding) pairs of an entity script binding table: median-of-three partition while the run is longer than 16, recursing right and looping left, partial_sort when the depth budget runs out
// Leaf: ??$__introsort_loop@PAU?$pair@VCCharString@@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@std@@U12@HV?$CCompareString@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@@@std@@YIXPAU?$pair@VCCharString@@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@0@00HV?$CCompareString@V?$CCountedPointer@VCEntityScriptBindingBase@@@@@@@Z
// Identity (proven): STLport 4.6 __introsort_loop (stl/_algo.c) over the 12-byte pair<CCharString, CCountedPointer<CEntityScriptBindingBase> >
// (the / 12 is the 0x2AAAAAAB imul; 5 inline_recursion levels, threshold 16). Callees: the pair's CCharString::operator< 0x009E6530
// (reloc_bindings `??MCCharString@@QBE_NABV0@@Z`) through CCompareString, CCharString copy ctor 0x0099EC30, the pair copy ctor 0x00CB9C50
// (manifest stand-in "CScriptGameResourceObjectScriptedThingBase"), __median 0x00CBA620 (landed std_median_00cba620.cpp: the same
// pair/comparator), __unguarded_partition 0x00CBB860 (its swap is bound to pair<CCharString, CCountedPointer<CEntityScriptBindingBase> >),
// __partial_sort 0x00CBD0F0, and itself. Caller 0x00CBDDBD is the recursion; 0x00CBDFAD is the sort<> body (landed as
// global_SortSElement1200cbdf70, which binds this row as an invented IntrosortLoop_00cbdad0 -> NAME_FIXES).
// Model: the landed std_introsortloop_00cbd700.cpp (CActiveScriptPair instantiation) with this TU's element and comparator.
#pragma inline_recursion(on)
void __cdecl operator delete(void* block);                 // 0x00BFE9BC
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);                 // 0x0099EC30
    ~CCharString();                                        // 0x0099EAE0
    bool operator<(const CCharString& other) const;        // 0x009E6530
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
template <class _Tp, class _Compare>
const _Tp& __fastcall __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp)
{
    if (__comp(__a, __b))
        if (__comp(__b, __c))
            return __b;
        else if (__comp(__a, __c))
            return __c;
        else
            return __a;
    else if (__comp(__a, __c))
        return __a;
    else if (__comp(__b, __c))
        return __c;
    else
        return __b;
}
template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp);
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*, _Compare __comp);
template <class _RandomAccessIter, class _Compare>
inline void __fastcall partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Compare __comp)
{
    __partial_sort(__first, __middle, __last, (typename iterator_traits<_RandomAccessIter>::value_type*)0, __comp);   // _STLP_VALUE_TYPE
}
template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit, _Compare __comp)
{
    while (__last - __first > __stl_threshold) {
        if (__depth_limit == 0) {
            partial_sort(__first, __last, __last, __comp);
            return;
        }
        --__depth_limit;
        _RandomAccessIter __cut =
            __unguarded_partition(__first, __last,
                                  _Tp(__median(*__first,
                                               *(__first + (__last - __first) / 2),
                                               *(__last - 1), __comp)),
                                  __comp);
        __introsort_loop(__cut, __last, (_Tp*)0, __depth_limit, __comp);
        __last = __cut;
    }
}
template void __fastcall __introsort_loop<CScriptBindingPair*, CScriptBindingPair, int, CScriptBindingLess>(CScriptBindingPair*, CScriptBindingPair*, CScriptBindingPair*, int, CScriptBindingLess);
}