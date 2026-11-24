// CCreateIndicesFromItemNames::operator()(const CCharString&) -- append (definition index of the named def, "") to ItemIndices
// Leaf: ??RCCreateIndicesFromItemNames@@QAEXABVCCharString@@@Z
// Retail 0x008ED440, 0x8F bytes, thiscall `ret 4`. Manifest label "std_vector_push_back<DefinitionIndex_CCharString>" is
//   descriptive; PDB: `??RCCreateIndicesFromItemNames@@QAEXABVCCharString@@@Z`. Only caller: for_each 0x008ED510
//   (loading branch of CTCContainer::OnSerialise).
// Callees: CGameDefinitionManager::Get @ 0x0044C6B0, CDefinitionManager::GetDefGlobalIndexFromName @ 0x009AD410,
//   CCharString(const char*, long = -1) @ 0x0099EBF0 ("" @ 0x0122D70E), copy ctor 0x0099EC30, dtor 0x0099EAE0 (twice:
//   the pair temporary, then the "" temporary), vector<pair<long, CCharString>>::_M_insert_overflow @ 0x0074FCE0.
// Shape: the index is a named local (fetched before the "" string is built); an EXPLICIT `CCharString("")` temporary
//   (its constructor's eax feeds the pair copy; the implicit "" conversion re-forms the address with `lea`) in the dead
//   `name` slot; the pair temporary is the 8-byte frame; STLport's pair copy ctor stores `first` before the string
//   copy call; the finish advances as `add [esi+4],8`.
// TU: CTCContainer (0x008ED040 RemoveItem, 0x008ED1F0 InsertItem landed, 0x008ED2B0 OnSerialise, the two functors at
//   0x008ED3B0 / 0x008ED440 and their std::for_each instantiations 0x008ED4D0 / 0x008ED510). Names from the FableWin PDB
//   name set: CCreateItemNamesFromIndices (ItemNames vector<CCharString>*), CCreateIndicesFromItemNames (ItemIndices
//   vector<pair<long, CCharString>>*), both with `operator()` non-const; Ego_r.pdb: CTCContainer::ContainerItems +0x0C,
//   CDefString::TablePos, CPersistContext::Mode +0x18.
// Shape notes shared by the TU: STLport 4.x vector (`_Vector_base` + `_STLP_alloc_proxy`), push_back with
//   `_M_insert_overflow(finish, x, __false_type(), 1, true)` as a same-TU noinline STAND-IN body (only the call is
//   graded; it reads `x` and never touches the tag, so the empty tag shares the dead-argument slot of the live
//   temporary, GOTCHAS b48) and STLport's MSVC `pair` copy constructor.
// NF254 (2026-11-22, CODEXP27 L4 / L5 family): CCharString and the whole namespace-std model (pair, allocator, _Vector_base, vector with a
//   53-byte same-TU STAND-IN _M_insert_overflow body under the STLport name) were row-local; they now come from the shared headers
//   (fable_string.h CCharString; stlport/fable_containers.h vector with the real STLport 4.6 _vector.c overflow body visible and kept out
//   of line, FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE, and its out-of-line __fastcall helpers FABLE_STLP_OUTOFLINE_UNINIT_COPY / _FILL_N,
//   the 0x0074FCE0 recipe), so the pick-any _M_insert_overflow copy this object defines is the owner's body, not a stand-in. The other
//   local models are as at HEAD.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_STLP_OUTOFLINE_UNINIT_COPY
#define FABLE_STLP_OUTOFLINE_UNINIT_FILL_N
#include "fable_string.h"
#include "stlport/fable_containers.h"

class CDefString {                              // 4 bytes, an index into the def string table
public:
    long TablePos;                              // +0x00
    CDefString() {}
};

class CDefStringTable {
public:
    CCharString GetString(long tablePos) const; // 0x009D49B0
};
extern CDefStringTable GDefStringTable;         // 0x013CA828 (Ego_r.pdb S_PUB32 ?GDefStringTable@@3VCDefStringTable@@A + Ego_d.pdb; NF202 q12261: was the made-up g_DefStringTable; unused here)

class CDefinitionManager {
public:
    CDefString GetDefNameFromGlobalIndex(long index) const;          // 0x009ACCC0
    long GetDefGlobalIndexFromName(const CCharString& name) const;   // 0x009AD410
};

class CGameDefinitionManager : public CDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();                 // 0x0044C6B0
};

typedef std::pair<long, CCharString> CContainerItem;

class CCreateItemNamesFromIndices {
public:
    std::vector<CCharString>* ItemNames;       // +0x00
    CCreateItemNamesFromIndices(std::vector<CCharString>& itemNames) : ItemNames(&itemNames) {}
    void operator()(const CContainerItem& item);
};

class CCreateIndicesFromItemNames {
public:
    std::vector<CContainerItem>* ItemIndices;  // +0x00
    CCreateIndicesFromItemNames(std::vector<CContainerItem>& itemIndices) : ItemIndices(&itemIndices) {}
    void operator()(const CCharString& name);
};

class CPersistContext {
public:
    unsigned char _pad00[0x18];
    long Mode;                                  // +0x18 (EMode)
    bool IsSaving() const { return Mode == 1 || Mode == 3; }
    template <class T> void TransferVector(const char* name, std::vector<T>& value);   // <CCharString> 0x0049B8D0
};

class CTCContainer {
public:
    unsigned char _base[0xc];                   // +0x00 CTCBase (vptr, parent thing, flags)
    std::vector<CContainerItem> ContainerItems;   // +0x0c
    void OnSerialise(CPersistContext& context, bool savedInGame);
};

void CCreateIndicesFromItemNames::operator()(const CCharString& name)
{
    long defIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name);
    ItemIndices->push_back(CContainerItem(defIndex, CCharString("")));
}