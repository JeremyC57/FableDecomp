// _global::__linear_insert(CTestQuest* first, CTestQuest* last, CTestQuest value, CTestQuestNameCompare comp) -- STLport insertion-sort step for the test-quest list: a value that sorts before the first element shifts the run up one slot and goes first, anything else goes through the unguarded insert
// Leaf: __linear_insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00624760, 143 bytes (+ int3 padding), `ret 20h`.
// Identity: the catalog label "std_InsertionSort_Step_TestQuest" is descriptive; the body is the STLport
//   __linear_insert(first, last, T val, comp) { if (comp(val, *first)) { copy_backward(first, last, last + 1);
//       *first = val; } else __unguarded_linear_insert(last, val, comp); }
// for CTestQuest (0x1c bytes); its caller is the __insertion_sort @ 0x00624C90.
// Callees: CTestQuestNameCompare::operator() @ 0x006238A0; __copy_backward(first, last, result,
// const random_access_iterator_tag&, int*) @ 0x00623390 (catalogued under the propagated
// `_Copy_backward<CSymbolMap::CSymbolInfo*>`; `ret 0Ch`); CTestQuest::operator= @ 0x004A8F50;
// CTestQuest::CTestQuest(const CTestQuest&) @ 0x004A89D0; __unguarded_linear_insert @ 0x00623E90;
// CCharString::~CCharString @ 0x0099EAE0 (x6, the by-value parameter).
// Shape: /Gr STLport algorithm TU: first in ecx -> esi, last in edx -> edi, the value and the empty comparator on
// the stack. copy_backward's empty iterator tag is a const-reference temporary homed over the comparator slot
// (`lea ecx,[esp+2Ch]`), which needs the real __copy_backward body defined out of line in the TU (the 0x00ADBE70
// lesson). The implicit CTestQuest copy and assignment stay out of line here (no loop): `*first = val` calls
// operator=, and the by-value argument of the unguarded insert is copy-constructed in the argument area
// (`sub esp,1Ch ; mov ecx,esp`) after the comparator copy is pushed. Layout from the landed CTestQuest
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
};

class CTestQuestNameCompare { public:
    bool operator()(const CTestQuest& a, const CTestQuest& b) const;   // 0x006238A0
};

struct random_access_iterator_tag {
    random_access_iterator_tag() { }
};

// The real __copy_backward body (0x00623390), out of line in the TU.
#pragma auto_inline(off)
__declspec(noinline) CTestQuest* __fastcall __copy_backward(CTestQuest* first, CTestQuest* last, CTestQuest* result,
                                                            const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}
#pragma auto_inline(on)

void __fastcall __unguarded_linear_insert(CTestQuest* last, CTestQuest val, CTestQuestNameCompare comp);   // 0x00623E90

inline CTestQuest* copy_backward(CTestQuest* first, CTestQuest* last, CTestQuest* result)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}

void __fastcall __linear_insert(CTestQuest* first, CTestQuest* last, CTestQuest val, CTestQuestNameCompare comp)
{
    if (comp(val, *first)) {
        copy_backward(first, last, last + 1);
        *first = val;
    }
    else
        __unguarded_linear_insert(last, val, comp);
}