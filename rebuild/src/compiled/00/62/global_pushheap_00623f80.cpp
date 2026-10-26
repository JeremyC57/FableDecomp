// _global::__push_heap(CTestQuest* first, int holeIndex, int topIndex, CTestQuest value, CTestQuestNameCompare comp) -- STLport heap sift-up for the test-quest sort: move parents that sort before `value` down into the hole, then store `value`
// Leaf: __push_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00623F80, 234 bytes, `ret 24h`.
// Identity: the catalog label `_Push_heap<CTestQuest*,int,CTestQuest,CTestQuestNameCompare>` is right in
// substance; the STLport spelling is
//   __push_heap(first, holeIndex, topIndex, T val, comp) { parent = (holeIndex - 1) / 2;
//       while (holeIndex > topIndex && comp(*(first + parent), val)) { *(first + holeIndex) = *(first + parent);
//           holeIndex = parent; parent = (holeIndex - 1) / 2; } *(first + holeIndex) = val; }
// It is the tail of the landed __adjust_heap @ 0x006243D0 (that body calls it).
// Callees: CTestQuestNameCompare::operator() @ 0x006238A0, CCharString::operator= @ 0x0099EFB0 (x6, inlined
// element assignment in the loop), CTestQuest::operator= @ 0x004A8F50 (the final store, out of line),
// CCharString::~CCharString @ 0x0099EAE0 (x6, the by-value parameter).
// Shape: /Gr STLport algorithm TU: first in ecx -> ebp, holeIndex in edx -> esi, topIndex, the 28-byte value
// and the empty comparator on the stack. The signed `/ 2` is `cdq ; sub eax,edx ; sar 1`. The implicit
// memberwise CTestQuest::operator= is inlined in the loop and called out of line for the last store; the
// by-value `val` is destroyed by the callee. Layout from the landed CTestQuest sources (PDB names).
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

class CTestQuestNameCompare { public:
    bool operator()(const CTestQuest& a, const CTestQuest& b) const;   // 0x006238A0
};

void __fastcall __push_heap(CTestQuest* first, int holeIndex, int topIndex, CTestQuest val, CTestQuestNameCompare comp)
{
    int parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(*(first + parent), val)) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = val;
}