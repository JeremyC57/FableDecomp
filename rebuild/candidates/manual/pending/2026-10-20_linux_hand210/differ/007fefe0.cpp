// CTCNamedUserActionPoints::GetFullNamedActionPoint(CThing& thing, CCharString name, long& index, long start) -- remember the user, then from `start` find the first action point whose name contains `name` and that is not in the free list (in use); returns its index
// Grade: DIFFER(220v216) -- retail keeps the loop index in ebx and also in the `start` slot (whose address is find's value AND the tag's), preloads "" for the name's text and lays the Names[i] null case out first; ours keeps the index in eax, opens no ebx/edi and orders the two c_str selections differently
// Leaf: GetFullNamedActionPoint
// Retail: Fable.exe (TLC, VC7.1) @ 0x007FEFE0, 216 bytes (`ret 10h`; PDB ?GetFullNamedActionPoint@CTCNamedUserActionPoints@@QAE_NAAVCThing@@VCCharString@@AAJJ@Z, the name by value).
// Callees: CBaseIntelligentPointer() @ 0x00A01B10 / SetPItem @ 0x00A01B90 / GetPItem @ 0x00A01B50 / ~ @ 0x00A01C10 (PThing assigned
//   from a converting temporary), strstr @ 0x00BFEB3A, std::find @ 0x0050E540 (landed `Find_0050e540`, `__fastcall`), ~CCharString
//   @ 0x0099EAE0 (the by-value parameter, destroyed by the callee). "" @ 0x0129AAF4.
// Tried: one grade (the `for (; start < n; ++start)` loop on the parameter, `&&` of strstr and find). Next: lane gap: the find
//   value and its iterator tag share the live `start` parameter slot (GOTCHAS 24l / chunk 156); the two c_str forms are a
//   `const char* s = ""; if (p) s = p->Text;` getter for the parameter and the null-first ternary for the element.
extern "C" char* __cdecl strstr(const char* s, const char* find);   // 0x00BFEB3A (CRT)
struct CCharStringData { const char* Text; };
extern const char g_EmptyText_0129aaf4[];               // 0x0129AAF4 ""
struct CCharString {
    CCharStringData* PStringData;
    ~CCharString();                                     // 0x0099EAE0
    const char* c_str() const { return PStringData == 0 ? g_EmptyText_0129aaf4 : PStringData->Text; }
};
struct CIteratorTag { CIteratorTag() { } };
long* __fastcall Find_0050e540(long* first, long* last, const long& value, const CIteratorTag& tag);   // 0x0050E540
class CThing;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CThing* item);                        // 0x00A01B90
    void* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C
public:
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
template <class T> struct vector {                      // STLport vector, 12 bytes
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    T& operator[](unsigned long n) const { return *(_M_start + n); }
};
struct CTCUserActionPointBase {
    virtual void v00();
    CThing*       ParentThing;                          // +0x04 (CTCBase)
    bool          ToBeRemoved;                          // +0x08
    bool          IsInGlobalUpdateTCs;                  // +0x09
    unsigned char _pad0a[2];
    vector<long>  FreeActionPointIndexes;               // +0x0c
    vector<long>  BlockedActionPointIndexes;            // +0x18
    long          NumberOfActionPoints;                 // +0x24
    long          FrameOfNextCheck;                     // +0x28
    CIntelligentPointer<CThing> PThing;                 // +0x2c
};
struct CTCNamedUserActionPoints : CTCUserActionPointBase {
    vector<CCharString> Names;                          // +0x34
    bool GetFullNamedActionPoint(CThing& thing, CCharString name, long& index, long start);
};

bool CTCNamedUserActionPoints::GetFullNamedActionPoint(CThing& thing, CCharString name, long& index, long start)
{
    PThing = CIntelligentPointer<CThing>(&thing);
    for (; start < NumberOfActionPoints; ++start) {
        if (strstr(Names[start].c_str(), name.c_str())
            && Find_0050e540(FreeActionPointIndexes._M_start, FreeActionPointIndexes._M_finish, start, CIteratorTag())
                   == FreeActionPointIndexes._M_finish) {
            index = start;
            return true;
        }
    }
    return false;
}
