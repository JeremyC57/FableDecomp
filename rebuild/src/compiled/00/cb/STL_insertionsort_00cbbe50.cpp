// std::__insertion_sort(SKeyedNameRecord* first, SKeyedNameRecord* last, CKeyPairCompareLess comp) -- STLport insertion sort over 24-byte {unsigned __int64 key, long, long, CCharString} records: every element after the first goes through the out-of-line __linear_insert(first, i, *i, comp)
// Leaf: __insertion_sort
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBBE50, 86 bytes, `ret 4` (__fastcall: first in ecx, last in edx, the
// 4-byte comparator on the stack -- the SGI/STLport algorithm TUs are /Gr).
// Callees: 0x00CBB920 std::__linear_insert(first, last, value, comp) (manifest "NavigationPosition_InsertInOrder" is
// propagated; only this body calls it, `ret 1Ch` = the 0x18-byte value + the comparator), 0x0099EC30
// CCharString::CCharString(const CCharString&).
// Element: the callee orders on a 64-bit unsigned key (`cmp hi ; ja/jb ; cmp lo ; jae`), assigns two more dwords and
// then a CCharString (operator= @ 0x0099EFB0) at +0x10; 0x18 bytes with 8-byte alignment (4 trailing pad bytes).
// Label: catalogued as `_Insertion_sort1<NGraphicAppearance::CMeshInfo*, CSortPrimaryMeshesFirst>` (propagated).
// Shape: the record has an explicit memberwise copy constructor, so the by-value `*i` is built in the argument area
// (`sub esp,18h ; mov eax,esp`): four dword moves, then the string copy constructor on `[eax+10h]`. It must be ONE flat
// record: a pair<u64, struct{long, long, CCharString}> makes VC7.1 strength-reduce `&i->second.m_name` into a second
// induction register (`lea edi,[esi+10h]` hoisted, `[edi-8]`/`[edi-4]` loads) and reload the comparator every pass.
class CCharString { public:
    void* m_pData;                        // shared string block
    CCharString(const CCharString& other);   // 0x0099EC30
};

struct SKeyedNameRecord {                 // 0x18 bytes, 8-byte alignment
    unsigned __int64 m_key;               // +0x00 sort key
    long m_value0;                        // +0x08
    long m_value1;                        // +0x0c
    CCharString m_name;                   // +0x10
    SKeyedNameRecord(const SKeyedNameRecord& o) : m_key(o.m_key), m_value0(o.m_value0), m_value1(o.m_value1), m_name(o.m_name) { }
};

struct CKeyPairCompareLess {              // 4-byte comparator functor passed by value
    void* m_context;
};

void __fastcall __linear_insert(SKeyedNameRecord* first, SKeyedNameRecord* last, SKeyedNameRecord value, CKeyPairCompareLess comp);   // 0x00CBB920

void __fastcall __insertion_sort(SKeyedNameRecord* first, SKeyedNameRecord* last, CKeyPairCompareLess comp)
{
    if (first == last)
        return;
    for (SKeyedNameRecord* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}