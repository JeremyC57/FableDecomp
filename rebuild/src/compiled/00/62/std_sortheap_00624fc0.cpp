// std::sort_heap(CTestQuest* first, CTestQuest* last, CTestQuestNameCompare comp) -- SGI sort_heap over the test-quest list: pop the root to the back until one element remains
// Leaf: sort_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00624FC0, length 199 bytes (+ int3 padding), `ret 4`.
// Identity: the catalog label `_Sort_heap<pair<unsigned long,C3DHelperPoint>*,...>` is propagated over the
// byte-identical skeleton; the only non-string callee is the landed SGI `__pop_heap<CTestQuest*, ...,
// CTestQuestNameCompare>` @ 0x00624800 (one of its four front-end callers is 0x00625054, inside this body),
// and the stride is 0x1C (the 0x92492493 reciprocal + `add edx,ecx ; sar edx,4` divide by 28).
// Callees: __pop_heap @ 0x00624800, CCharString::CCharString(const CCharString&) @ 0x0099EC30 (x6).
// Shape: /Gr SGI algorithm TU -- `while (last - first > 1) pop_heap(first, last--, comp)` with pop_heap and
// __pop_heap_aux inline. The value copy `CTestQuest(*(last - 1))` is CTestQuest's IMPLICIT copy constructor,
// inlined here (the __pop_heap TU calls an out-of-line copy at 0x004A89D0): six CCharString copy constructors
// in member order with the ColourIndex dword moved between HSPName and DisplayName. Layout from the landed
// CTestQuest sources (PDB member names).
struct CCharString {
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

struct CTestQuestNameCompare { };

void __fastcall __pop_heap(CTestQuest* first, CTestQuest* last, CTestQuest* result, CTestQuest value,
                           CTestQuestNameCompare comp, long*);   // 0x00624800

inline void __pop_heap_aux(CTestQuest* first, CTestQuest* last, CTestQuest*, CTestQuestNameCompare comp)
{
    __pop_heap(first, last - 1, last - 1, CTestQuest(*(last - 1)), comp, (long*)0);
}

inline void pop_heap(CTestQuest* first, CTestQuest* last, CTestQuestNameCompare comp)
{
    __pop_heap_aux(first, last, (CTestQuest*)0, comp);
}

void __fastcall sort_heap(CTestQuest* first, CTestQuest* last, CTestQuestNameCompare comp)
{
    while (last - first > 1)
        pop_heap(first, last--, comp);
}