// _global::__unguarded_linear_insert(CTestQuest* last, CTestQuest value, CTestQuestNameCompare comp) -- STLport insertion-sort step for the test-quest list: shift every element whose name sorts after `value` up one slot, then drop `value` into the hole
// Leaf: __unguarded_linear_insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00623E90, 201 bytes, `ret 20h`.
// Identity: the catalog label "Array_InsertionSort_TestQuest" is descriptive; the body is the STLport
//   __unguarded_linear_insert(last, T val, comp) { next = last; --next; while (comp(val, *next)) {
//       *last = *next; last = next; --next; } *last = val; }
// for CTestQuest (0x1c bytes) under CTestQuestNameCompare; its caller is the landed-shape __linear_insert
// @ 0x00624760 (and the unguarded insertion sort).
// Callees: CTestQuestNameCompare::operator() @ 0x006238A0, CCharString::operator= @ 0x0099EFB0 (x6, the
// inlined element assignment in the loop), CTestQuest::operator= @ 0x004A8F50 (the final `*last = val`, out
// of line), CCharString::~CCharString @ 0x0099EAE0 (x6, the by-value parameter).
// Shape: /Gr STLport algorithm TU: `last` in ecx, the 28-byte value and the empty comparator on the stack.
// CTestQuest's assignment is the IMPLICIT memberwise operator=: VC7.1 inlines it inside the loop (six
// CCharString::operator= calls with the ColourIndex dword moved between the second and third) and calls the
// out-of-line COMDAT for the last assignment; the callee destroys the by-value `val` (six string dtors in
// reverse member order, skipping ColourIndex). Layout from the landed CTestQuest sources (PDB names).
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

void __fastcall __unguarded_linear_insert(CTestQuest* last, CTestQuest val, CTestQuestNameCompare comp)
{
    CTestQuest* next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}