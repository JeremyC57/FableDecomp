// std::__introsort_loop(CTestQuest* __first, CTestQuest* __last, CTestQuest*, int __depth_limit, CTestQuestNameCompare __comp) -- STLport introsort recursion over the test quests: median-of-three partition by name until 16 elements remain, partial (heap) sort when the depth budget runs out
// Leaf: ??$__introsort_loop@PAVCTestQuest@@V1@HVCTestQuestNameCompare@@@std@@YIXPAVCTestQuest@@00HVCTestQuestNameCompare@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00625440, 364 bytes, __fastcall (first ecx, last edx), `ret 0Ch` (the _Tp* tag,
//   the depth limit, the comparator).
// Identity (proven): the landed std::sort<CTestQuest*, CTestQuestNameCompare> 0x006257C0 calls it with `__lg(n) * 2`
//   (bound spelling used here), and it calls itself; Ego_r.pdb sorts CTCInventoryQuests' vector<CTestQuest> with
//   CTestQuestNameCompare. The catalog label "introsort_TestQuest" is descriptive.
// Callees: CTestQuestNameCompare::operator() const 0x006238A0 (the inlined __median's compares), CCharString(const
//   CCharString&) 0x0099EC30 (the pivot's implicit copy, six strings), __unguarded_partition 0x00623C80 (pivot by value),
//   __partial_sort 0x006251C0, itself.
// Shape: the landed 0x006FD1E0 model: `while (last - first > __stl_threshold)`; `--depth_limit` in its own argument
//   slot; __median inlined; the pivot CTestQuest is copy-constructed straight into the by-value argument area.
extern "C" int __cdecl _stricmp(const char* a, const char* b);                   // 0x00BFEBA8
extern "C" int __cdecl strcmp(const char* a, const char* b);
#pragma intrinsic(strcmp)
template <class T> class CBasicString { public:
    T* PData;
    bool operator==(const T* text) const;                   // 0x004115A0
};
class CCharStringData { public:
    CBasicString<char> Data;
};
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
    CCharString& operator=(const CCharString& other);       // 0x0099EFB0
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
    const char* GetAsCharArray() const { return PStringData == 0 ? EmptyText : PStringData->Data.PData; }
    static char EmptyText[];                                // 0x0129AAF4
};
class CTestQuest { public:                                  // 0x1C bytes (Ego_r.pdb)
    CCharString QuestName;                                  // +0x00
    CCharString HSPName;                                    // +0x04
    long        ColourIndex;                                // +0x08
    CCharString DisplayName;                                // +0x0C
    CCharString IniFile;                                    // +0x10
    CCharString OutiFile;                                   // +0x14
    CCharString QuestCardDefName;                           // +0x18
};
class CTestQuestNameCompare { public:                       // empty comparator (Ego_r.pdb)
    bool operator()(const CTestQuest& a, const CTestQuest& b) const;   // 0x006238A0
};
// The comparator's body (0x006238A0) compiled in this TU under auto_inline(off), as retail's sort TU holds it: knowing
// that the call never reads the empty comparator, VC7.1 passes the argument slot itself to the inlined __median
// instead of a 1-byte copy. It orders by display name, or the quest name when that is empty, case-insensitively.
#pragma auto_inline(off)
__declspec(noinline) bool CTestQuestNameCompare::operator()(const CTestQuest& a, const CTestQuest& b) const
{
    CCharString nameA(a.DisplayName);
    if (nameA == "")
        nameA = a.QuestName;
    CCharString nameB(b.DisplayName);
    if (nameB == "")
        nameB = b.QuestName;
    return _stricmp(nameA.GetAsCharArray(), nameB.GetAsCharArray()) < 0;
}
#pragma auto_inline(on)

namespace std {

const int __stl_threshold = 16;

template <class _Tp, class _Compare>
inline const _Tp& __median(const _Tp& __a, const _Tp& __b, const _Tp& __c, _Compare __comp)
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
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot,
                                                   _Compare __comp);                           // 0x00623C80
template <class _RandomAccessIterator, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIterator __first, _RandomAccessIterator __middle,
                               _RandomAccessIterator __last, _Tp*, _Compare __comp);           // 0x006251C0
template <class _RandomAccessIterator, class _Compare>
inline void partial_sort(_RandomAccessIterator __first, _RandomAccessIterator __middle, _RandomAccessIterator __last,
                         _Compare __comp)
{
    __partial_sort(__first, __middle, __last, (CTestQuest*)0, __comp);
}

template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Size __depth_limit,
                                 _Compare __comp)
{
    while (__last - __first > __stl_threshold) {
        if (__depth_limit == 0) {
            partial_sort(__first, __last, __last, __comp);
            return;
        }
        --__depth_limit;
        _RandomAccessIter __cut =
            __unguarded_partition(__first, __last,
                                  _Tp(__median(*__first, *(__first + (__last - __first) / 2), *(__last - 1), __comp)),
                                  __comp);
        __introsort_loop(__cut, __last, (_Tp*)0, __depth_limit, __comp);
        __last = __cut;
    }
}

template void __fastcall __introsort_loop<CTestQuest*, CTestQuest, int, CTestQuestNameCompare>(CTestQuest*, CTestQuest*,
                                                                                            CTestQuest*, int,
                                                                                            CTestQuestNameCompare);
}