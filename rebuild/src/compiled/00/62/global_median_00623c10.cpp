// _global::__median(const CTestQuest& a, const CTestQuest& b, const CTestQuest& c, CTestQuestNameCompare comp) -- STLport median-of-three for the test-quest sort: returns the element whose display name sorts between the other two
// Leaf: __median
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00623C10, 112 bytes (+ int3 padding), `ret 8`.
// Identity: the catalog label "CTestQuestNameCompare_Median" is descriptive; the body is the STLport
//   __median(a, b, c, comp) instantiated for CTestQuest (0x1c bytes) under CTestQuestNameCompare, the
// pivot picker of the test-quest introsort (same skeleton as the landed __median @ 0x00748760).
// Callees: CTestQuestNameCompare::operator()(const CTestQuest&, const CTestQuest&) @ 0x006238A0 (landed,
// out of line, `ret 8`), five call sites.
// Shape: the STLport algorithm TUs are /Gr: a in ecx -> edi, b in edx -> esi, c on the stack -> ebx and the
// empty comparator passed BY VALUE in the second stack slot, so every compare runs with ecx = &comp (the
// argument slot). The result is a reference (pointer in eax). `c` is loaded after the first compare, while
// the branch on its result is pending, because both arms push it.
// Layout from the landed CTestQuest sources (Ego_r.pdb member names).
struct CCharString {
    void* m_pData;
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

const CTestQuest& __fastcall __median(const CTestQuest& a, const CTestQuest& b, const CTestQuest& c,
                                      CTestQuestNameCompare comp)
{
    if (comp(a, b)) {
        if (comp(b, c))
            return b;
        else if (comp(a, c))
            return c;
        else
            return a;
    }
    else if (comp(a, c))
        return a;
    else if (comp(b, c))
        return c;
    else
        return b;
}