// _global::__make_heap(CTestQuest* first, CTestQuest* last, CTestQuestNameCompare comp, CTestQuest*, int*) -- STLport make_heap core for the test-quest sort: sift every parent down from the last one to the root
// Leaf: __make_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00624880, 170 bytes (+ int3 padding), `ret 0Ch`.
// Identity: the catalog label "std_AdjustHeap_TestQuest" names its callee; the body is the STLport
//   __make_heap(first, last, comp, T*, Distance*) { if (last - first < 2) return; len = last - first;
//       parent = (len - 2) / 2; for (;;) { __adjust_heap(first, parent, len, T(*(first + parent)), comp);
//       if (parent == 0) return; parent--; } }
// for CTestQuest (0x1c bytes) under CTestQuestNameCompare.
// Callees: __adjust_heap(first, holeIndex, len, T value, comp) @ 0x006243D0 (__fastcall, landed-shape callee
// of __pop_heap 0x00624800), CCharString::CCharString(const CCharString&) @ 0x0099EC30 (x6).
// Shape: /Gr STLport algorithm TU: first in ecx (spilled to the `push ecx` frame word), last in edx, the comparator
// and the two type-tag pointers on the stack. The element count is the reciprocal multiply by 0x92492493
// (divide by 28), computed once. The by-value copy of `*(first + parent)` is CTestQuest's IMPLICIT copy
// constructor, inlined into the argument area (`sub esp,1Ch ; mov esi,esp`, six CCharString copies with the
// ColourIndex dword between the second and third) inside the loop, the source pointer strength-reduced to
// `first + parent*28 + 8`. Layout from the landed CTestQuest sources (PDB names).
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
};

class CTestQuestNameCompare { public:
    bool operator()(const CTestQuest& a, const CTestQuest& b) const;   // 0x006238A0
};

void __fastcall __adjust_heap(CTestQuest* first, int holeIndex, int len, CTestQuest val,
                              CTestQuestNameCompare comp);   // 0x006243D0

void __fastcall __make_heap(CTestQuest* first, CTestQuest* last, CTestQuestNameCompare comp, CTestQuest*, int*)
{
    if (last - first < 2)
        return;
    int len = last - first;
    int parent = (len - 2) / 2;
    for (;;) {
        __adjust_heap(first, parent, len, CTestQuest(*(first + parent)), comp);
        if (parent == 0)
            return;
        parent--;
    }
}