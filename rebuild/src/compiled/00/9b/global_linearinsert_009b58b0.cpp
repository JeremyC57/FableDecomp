// _global::__linear_insert(CWideString* first, CWideString* last, CWideString value, less<CWideString> comp) -- SGI insertion-sort step over wide strings: if `value` sorts before the first element, shift [first, last) up one slot and put it first, otherwise sink it from `last` with __unguarded_linear_insert
// Leaf: __linear_insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B58B0, length 98 bytes (`ret 8`: the 4-byte CWideString value and the empty comparator
// on the stack; first/last in ecx/edx -- the SGI algorithm TUs are /Gr, as the landed 0x00A70FF0 / 0x009D2FC0).
// Label correction: the catalog's "CTCOpinionSource::PushNewSourceDef" is propagated and wrong (the body sits among the STL sort
// helpers 0x009B5160..0x009B5990 and has no direct callers).
// Callees: CWideString::operator< @ 0x0099AF70, copy_backward(first, last, result) @ 0x00497460 (__fastcall; catalog
// "VectorInsert_CopyWideStringsReverse"), CWideString::operator= @ 0x0099B7D0, CWideString::CWideString(const CWideString&)
// @ 0x0099B720, __unguarded_linear_insert(last, value, comp) @ 0x009B1870 (catalog "CWideString_BackupAndRestore"),
// CWideString::~CWideString @ 0x0099B510 (the by-value parameter, destroyed on both paths).
// Shape: the landed 0x00A70FF0 template with a 4-byte class element: the else branch pushes the comparator and rebuilds the value in
// the argument area with the copy constructor.
class CWideString { public:
    void* PStringData;
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    bool operator<(const CWideString& other) const;     // 0x0099AF70
};
struct CWideStringLess {                                // std::less<CWideString>
    bool operator()(const CWideString& a, const CWideString& b) const { return a < b; }
};
CWideString* __fastcall copy_backward(CWideString* first, CWideString* last, CWideString* result);   // 0x00497460
void __fastcall __unguarded_linear_insert(CWideString* last, CWideString value, CWideStringLess comp);   // 0x009B1870

void __fastcall __linear_insert(CWideString* first, CWideString* last, CWideString value, CWideStringLess comp)
{
    if (comp(value, *first)) {
        copy_backward(first, last, last + 1);
        *first = value;
    }
    else
        __unguarded_linear_insert(last, value, comp);
}