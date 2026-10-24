// _global::__insertion_sort(CTestQuest* first, CTestQuest* last, CTestQuestNameCompare comp) -- STLport insertion sort over the test-quest list: insert every element after the first into the sorted prefix
// Leaf: __insertion_sort
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00624C90, 124 bytes (+ int3 padding), `ret 4`.
// Identity: the catalog label `_Insertion_sort1<CTestQuest*,CTestQuestNameCompare,CTestQuest>` is right in
// substance; the STLport spelling is
//   __insertion_sort(first, last, comp) { if (first == last) return;
//       for (i = first + 1; i != last; ++i) __linear_insert(first, i, T(*i), comp); }
// Callees: __linear_insert(first, last, T value, comp) @ 0x00624760 (out of line, `ret 20h`),
// CCharString::CCharString(const CCharString&) @ 0x0099EC30 (x6).
// Shape: /Gr STLport algorithm TU: first in ecx -> ebp, last in edx -> ebx, the empty comparator on the stack and
// pushed by value first. The by-value `*i` is CTestQuest's IMPLICIT copy constructor inlined into the argument
// area inside the loop (`sub esp,1Ch ; mov edi,esp`, six CCharString copies, ColourIndex moved between the second
// and third). Layout from the landed CTestQuest sources (PDB names).
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

struct CTestQuestNameCompare {
    bool operator()(const CTestQuest& a, const CTestQuest& b) const;   // 0x006238A0
};

void __fastcall __linear_insert(CTestQuest* first, CTestQuest* last, CTestQuest val,
                                CTestQuestNameCompare comp);   // 0x00624760

void __fastcall __insertion_sort(CTestQuest* first, CTestQuest* last, CTestQuestNameCompare comp)
{
    if (first == last)
        return;
    for (CTestQuest* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}