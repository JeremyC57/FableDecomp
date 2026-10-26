// std::__unguarded_partition(CTestQuest* first, CTestQuest* last, CTestQuest pivot, CTestQuestNameCompare comp) -- STLport quicksort partition for the test-quest sort: walk both ends towards the pivot by quest name, swap the out-of-place pair, return the split point
// Leaf: ??$__unguarded_partition@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00623C80, 474 bytes, `ret 20h`. /Gr algorithm TU written as explicit __fastcall:
//   first in ecx (-> esi), last in edx (-> edi), the 28-byte pivot and the empty comparator on the stack.
// Callers: __introsort_loop over CTestQuest (the pivot copy built in the argument area). Callees:
//   CTestQuestNameCompare::operator() 0x006238A0 (landed, thiscall on the comparator slot), CCharString copy ctor
//   0x0099EC30 (x6) / operator= 0x0099EFB0 (x12) / ~CCharString 0x0099EAE0 (x6 for the swap temporary, x6 for the
//   by-value pivot): the inlined iter_swap with CTestQuest's implicit copy, assignment and destructor.
// Label: catalog "std_Partition_TestQuest" is descriptive; this is STLport 4.6 `__unguarded_partition`.
// Shape: STLport 4.6 _algo.c verbatim; iter_swap -> __iter_swap(a, b, (T*)0) { T tmp = *a; *a = *b; *b = tmp; } inline.
class CCharString { public:
    void* m_pData;
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
};

class CTestQuest {
public:
    CCharString QuestName;                         // +0x00
    CCharString HSPName;                           // +0x04
    long        ColourIndex;                       // +0x08
    CCharString DisplayName;                       // +0x0c
    CCharString IniFile;                           // +0x10
    CCharString OutiFile;                          // +0x14
    CCharString QuestCardDefName;                  // +0x18
};

class CTestQuestNameCompare {
public:
    bool operator()(const CTestQuest& a, const CTestQuest& b) const;   // 0x006238A0
};

namespace std {
template <class _Tp>
inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }

template <class _ForwardIter1, class _ForwardIter2, class _Tp>
inline void __iter_swap(_ForwardIter1 __a, _ForwardIter2 __b, _Tp*)
{
    _Tp __tmp = *__a;
    *__a = *__b;
    *__b = __tmp;
}

template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2)
{
    __iter_swap(__i1, __i2, __value_type(__i1));
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot, _Compare __comp)
{
    for (;;) {
        while (__comp(*__first, __pivot))
            ++__first;
        --__last;
        while (__comp(__pivot, *__last))
            --__last;
        if (!(__first < __last))
            return __first;
        iter_swap(__first, __last);
        ++__first;
    }
}
}

template CTestQuest* __fastcall std::__unguarded_partition<CTestQuest*, CTestQuest, CTestQuestNameCompare>(CTestQuest*, CTestQuest*, CTestQuest, CTestQuestNameCompare);