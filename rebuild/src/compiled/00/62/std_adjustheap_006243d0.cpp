// std::__adjust_heap(CTestQuest* first, int holeIndex, int len, CTestQuest val, CTestQuestNameCompare comp) -- STLport heap sift-down for the test-quest sort: move the preferred child up into the hole until the bottom, then sift `val` back up through __push_heap
// Leaf: ??$__adjust_heap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006243D0, 283 bytes, `ret 24h`. /Gr algorithm TU written as explicit __fastcall:
//   first in ecx (-> ebp), holeIndex in edx (-> edi), len, the 28-byte value and the empty comparator on the stack.
// Callers: __pop_heap 0x00624800 (landed), __make_heap 0x00624880 (landed). Callees: CTestQuestNameCompare::operator()
//   0x006238A0 (landed, thiscall on the comparator slot), CCharString::operator= 0x0099EFB0 (x6: the implicit
//   memberwise CTestQuest::operator= inlined in the loop), CTestQuest::operator= 0x004A8F50 (the `secondChild == len`
//   store, out of line), CTestQuest copy ctor 0x004A89D0 (the by-value value for __push_heap built in the argument area),
//   __push_heap 0x00623F80 (landed), CCharString::~CCharString 0x0099EAE0 (x6: the by-value val, destroyed by the callee).
// Label: catalog `_Adjust_heap<CTestQuest*,int,CTestQuest,CTestQuestNameCompare>` is the Dinkumware spelling; this is
//   STLport 4.6 `__adjust_heap`.
// Shape: STLport 4.6 _heap.c verbatim; topIndex lives in the `push ecx` slot; the implicit operator= is inlined in the
//   loop and called for the tail store, as in the landed __push_heap 0x00623F80.
class CCharString { public:
    void* m_pData;
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);  // 0x0099EFB0
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
    CTestQuest(const CTestQuest& other);           // 0x004A89D0
};

class CTestQuestNameCompare {
public:
    bool operator()(const CTestQuest& a, const CTestQuest& b) const;   // 0x006238A0
};

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val, _Compare __comp);   // 0x00623F80

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp)
{
    _Distance __topIndex = __holeIndex;
    _Distance __secondChild = 2 * __holeIndex + 2;
    while (__secondChild < __len) {
        if (__comp(*(__first + __secondChild), *(__first + (__secondChild - 1))))
            __secondChild--;
        *(__first + __holeIndex) = *(__first + __secondChild);
        __holeIndex = __secondChild;
        __secondChild = 2 * (__secondChild + 1);
    }
    if (__secondChild == __len) {
        *(__first + __holeIndex) = *(__first + (__secondChild - 1));
        __holeIndex = __secondChild - 1;
    }
    __push_heap(__first, __holeIndex, __topIndex, __val, __comp);
}
}

template void __fastcall std::__adjust_heap<CTestQuest*, int, CTestQuest, CTestQuestNameCompare>(CTestQuest*, int, int, CTestQuest, CTestQuestNameCompare);