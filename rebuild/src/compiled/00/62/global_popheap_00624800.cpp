// _global::__pop_heap(CTestQuest* first, CTestQuest* last, CTestQuest* result, CTestQuest value, CTestQuestNameCompare comp, long*) -- SGI pop_heap core for the test-quest list: *result = *first, then sift `value` down from the root
// Leaf: __pop_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00624800, length 126 bytes (+ int3 padding).
// Identity: the catalog label "~CAIStateGroup_JackDragon" is propagated over the byte-identical skeleton and
// wrong; this is the SGI
//   __pop_heap(first, last, result, T value, Compare comp, Distance*) { *result = *first;
//       __adjust_heap(first, Distance(0), Distance(last - first), value, comp); }
// instantiated for CTestQuest (0x1c bytes) with CTestQuestNameCompare -- the manifest names the callee
// `_Adjust_heap<CTestQuest*,int,CTestQuest,CTestQuestNameCompare>` @ 0x006243D0; callers 0x0062494E,
// 0x00624D4E, 0x00625054, 0x00625270 are the pop_heap/sort_heap front ends.
// Callees: CTestQuest::operator= @ 0x004A8F50 and CTestQuest::CTestQuest(const CTestQuest&) @ 0x004A89D0
// (both landed, out of line), __adjust_heap @ 0x006243D0, CCharString::~CCharString @ 0x0099EAE0 (x6).
//
// Shape: the SGI algorithm TUs are /Gr (free templates are __fastcall: first in ecx, last in edx, `ret 28h`
// for result + the 28-byte value + comp + Distance*); the element count is the reciprocal multiply by
// 0x92492493 (divide by 28). The value copy for __adjust_heap is built in the argument area by the
// out-of-line copy constructor (`sub esp,1Ch ; mov ecx,esp`); the by-value `value` parameter is destroyed
// by this callee at the end: CTestQuest's implicit destructor runs the six CCharString destructors in
// reverse member order (+0x18 .. +0x00, skipping ColourIndex at +8). Layout from the landed CTestQuest
// sources (PDB names).
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
    CTestQuest(const CTestQuest& other);           // 0x004A89D0
    CTestQuest& operator=(const CTestQuest& other);   // 0x004A8F50
};

class CTestQuestNameCompare { public: };

void __fastcall __adjust_heap(CTestQuest* first, long holeIndex, long len, CTestQuest value,
                              CTestQuestNameCompare comp);   // 0x006243D0

void __fastcall __pop_heap(CTestQuest* first, CTestQuest* last, CTestQuest* result, CTestQuest value,
                           CTestQuestNameCompare comp, long*)
{
    *result = *first;
    __adjust_heap(first, 0, long(last - first), value, comp);
}