// CTCHeroOpinionDeedLog::OnSerialise(CPersistContext& context, bool in_game) -- save or restore the hero's deed log: the next handle, the two crime switches, the crime village and the opinion-deed map (flattened into a vector of records to save, re-inserted record by record on load)
// Flags: none
// Leaf: ?OnSerialise@CTCHeroOpinionDeedLog@@UAEXAAVCPersistContext@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C7370, 342 bytes, `ret 8` (8-byte aligned frame); CTCHeroOpinionDeedLog vtable
//   0x0125EA48 slot 1 (RTTI .?AVCTCHeroOpinionDeedLog@@; the CTCBase OnSerialise slot, Ego_r.pdb ?OnSerialise@CTCHeroOpinionDeedLog@@
//   UAEXAAVCPersistContext@@_N@Z 0x006FEAA0). Every transfer name is the PDB member it addresses. The manifest's
//   "CDeedManager::TransferState" is an autoname.
// Callees: CPersistContext::Transfer<long> @ 0x00410BE0 ("NextHandle", default 0), Transfer<bool> @ 0x004045C0 (as the landed CTCWife
//   ::OnSerialise 0x006D2BD0; "WeaponOutCrimeEnabled" default true, "GuardsIgnoreCrimes" default false), Transfer<CIntelligentPointer
//   <CThing> > @ 0x006C9C60 ("VillageForCrimes"), vector<record>::reserve @ 0x006C8560, STLport __copy(first, last, back_insert_iterator,
//   const input_iterator_tag&, int*) @ 0x006C9B70 (__fastcall: the back_insert_iterator returned through ecx, the tag in edx; reached
//   through the inline std::copy), the record-vector Transfer @ 0x006C9EE0 ("OpinionDeedMap"), the map's insert_unique @ 0x006C8400,
//   free @ 0x00BFEA14.
// Layout (Ego_r.pdb CTCHeroOpinionDeedLog, retail STLport sizes): OpinionDeedMap +0x0C (header +0x0C, node count +0x10),
//   OpinionDeedsSearchingForWitnesses +0x18, NextHandle +0x1C, VillageForCrimes +0x20, WeaponOutDeedHandle +0x28,
//   WeaponOutCrimeEnabled +0x2C, GuardsIgnoreCrimes +0x2D; CPersistContext Mode +0x18 (IsSaving: 1 text save, 3 binary save).
// Shape: a save (mode 1 or 3) flattens the map into a reserved local vector before the transfer; a load transfers into an empty local vector
// and inserts each 8-byte record as a pair into the map.
extern "C" void __cdecl free(void* block);                       // 0x00BFEA14
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
class CThing;
struct COpinionDeedRecord { long Key; long Value; };
struct CRecordVector {
    COpinionDeedRecord* first; COpinionDeedRecord* last; COpinionDeedRecord* end;
    CRecordVector() : first(0), last(0), end(0) {}
    ~CRecordVector() { if (first) free(first); }
    void reserve(unsigned long count);                             // 0x006C8560
};
struct SDeedNode { long colour; SDeedNode* parent; SDeedNode* left; SDeedNode* right; COpinionDeedRecord value; };
struct SPairIB { SDeedNode* node; bool inserted; };
struct CDeedTree {
    SDeedNode* header; unsigned long count; unsigned char comp[4];
    unsigned long size() const { return count; }
    SPairIB InsertUnique(const COpinionDeedRecord& value);          // 0x006C8400
};
namespace std {
struct input_iterator_tag { };
struct bidirectional_iterator_tag : input_iterator_tag { };
struct _Rb_tree_iterator {                                             // STLport _Rb_tree_iterator (by value, copy constructed)
    SDeedNode* _M_node;
    _Rb_tree_iterator(SDeedNode* node) : _M_node(node) { }
    _Rb_tree_iterator(const _Rb_tree_iterator& other) : _M_node(other._M_node) { }
};
struct back_insert_iterator {                                           // STLport back_insert_iterator<vector<COpinionDeedRecord> >
    CRecordVector* container;
    explicit back_insert_iterator(CRecordVector& x) : container(&x) { }
};
// STLport __copy(first, last, result, const input_iterator_tag&, _Distance*) for the map iterators (out of line, 0x006C9B70)
back_insert_iterator __fastcall __copy(_Rb_tree_iterator first, _Rb_tree_iterator last, back_insert_iterator result,
                                     const input_iterator_tag& tag, int* distance);
// STLport 4.6 copy (stl/_algobase.h): inline, forwarding to __copy with the iterator category and distance type
inline back_insert_iterator copy(_Rb_tree_iterator first, _Rb_tree_iterator last, back_insert_iterator result)
{
    return __copy(first, last, result, bidirectional_iterator_tag(), (int*)0);
}
}
class CPersistContext { public:
    unsigned char _pad[0x18];
    long Mode;                                                     // +0x18
    bool IsSaving() const { return Mode == 1 || Mode == 3; }   // engine/CPersistContext.h (MODE_SAVE_TEXT / MODE_SAVE_BINARY)
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);  // 0x00410BE0 <long>, 0x004045C0 <bool> (agentNF19: Ego_r/FableWin ??$Transfer@J@.. / ??$Transfer@_N@..)
    void Transfer(const char* name, CIntelligentPointer<CThing>& value);                    // 0x006C9C60
    void Transfer(const char* name, CRecordVector& value);                                  // 0x006C9EE0
};
class CTCHeroOpinionDeedLog : public CTCBase {                     // Ego_r.pdb members, retail (STLport) offsets
public:
    CDeedTree   OpinionDeedMap;                                    // +0x0c map<long, COpinionDeed*>
    void*       OpinionDeedsSearchingForWitnesses;                 // +0x18 list<CWitnessesSearchingDeed> (STLport node pointer)
    long        NextHandle;                                        // +0x1c
    CIntelligentPointer<CThing> VillageForCrimes;                  // +0x20
    long        WeaponOutDeedHandle;                               // +0x28
    bool        WeaponOutCrimeEnabled;                             // +0x2c
    bool        GuardsIgnoreCrimes;                                // +0x2d
    virtual void OnSerialise(CPersistContext& context, bool in_game);
};
void CTCHeroOpinionDeedLog::OnSerialise(CPersistContext& context, bool in_game)
{
    context.Transfer("NextHandle", NextHandle, 0L);
    context.Transfer("WeaponOutCrimeEnabled", WeaponOutCrimeEnabled, true);
    context.Transfer("GuardsIgnoreCrimes", GuardsIgnoreCrimes, false);
    context.Transfer("VillageForCrimes", VillageForCrimes);
    if (context.IsSaving()) {
        CRecordVector records;
        records.reserve(OpinionDeedMap.size());
        std::copy(std::_Rb_tree_iterator(OpinionDeedMap.header->left), std::_Rb_tree_iterator(OpinionDeedMap.header),
                  std::back_insert_iterator(records));
        context.Transfer("OpinionDeedMap", records);
    }
    else {
        CRecordVector records;
        context.Transfer("OpinionDeedMap", records);
        COpinionDeedRecord* last = records.last;
        for (COpinionDeedRecord* it = records.first; it != last; ++it) {
            COpinionDeedRecord value = *it;
            OpinionDeedMap.InsertUnique(value);
        }
    }
}