// Behaviour test for 009acd00: src/009acd00.cpp copied verbatim, callees stubbed below so calls and arguments are observable.
#include <cstdio>
#include <cstring>
static int fails = 0;
static char g_msg[128];
#define CHECK(c) do { if (!(c)) { ++fails; _snprintf(g_msg, sizeof(g_msg), "FAIL %d: %s", __LINE__, #c); puts(g_msg); } } while (0)
// ---- source (verbatim) ----
// CDefinitionManager::ValidateDefs(const CValidateDefinitionInitBase& init) const -- walk every def class and every instantiated def of it (index 0, the class default, skipped) and ask each non-template def to validate itself under its name
// Leaf: ?ValidateDefs@CDefinitionManager@@QBEXABVCValidateDefinitionInitBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009ACD00, length 162 bytes (ret 4).
// Callees: CDefStringTable::GetString(long) 0x009D49B0 on the global def string table @ 0x013CA828 (CCharString by hidden
// pointer), the def's virtual slot 23 (+0x5C) Validate(const CValidateDefinitionInitBase&, const CCharString&), ~CCharString 0x0099EAE0.
// Layout (retail = PDB less 0xC before DefClassInfo, less 0x18 before InstantiatedDefs: the narrower retail vectors):
// DefClassInfo +0x78 (CVectorMap of 0x18-byte {class CRC, CDefClassInfo} pairs; the class's def index CArray at pair +0xC),
// InstantiatedDefs +0xA4 (CArray<CInstantiatedDefInfo*>: NameIndex +4, PDef +0xC); CDefClassBase Template = bit 1 of +0x24.
// Shape: the class info is a named reference to the pair's second (so the def-index array pointer is its own induction
// variable in edi, `add edi,18h`, and the pair pointer lives in the frame); both loops re-read their end/size every pass
// (unsigned compares); the name is a block-scoped CCharString.
#include "fable_string.h"

struct CValidateDefinitionInitBase;

class CDefClassBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22();
    virtual void Validate(const CValidateDefinitionInitBase& init, const CCharString& name) const;   // slot 23 (+0x5c)
    unsigned char _pad04[0x24 - 4];
    unsigned char Setup : 1;                    // +0x24 bit 0
    unsigned char Template : 1;                 // +0x24 bit 1
};

struct CDefStringTable {
    CCharString GetString(long index);          // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;        // 0x013CA828

struct CInstantiatedDefInfo {                   // CDefinitionManager::CInstantiatedDefInfo
    void*          _pad00;
    long           NameIndex;                   // +0x04
    void*          _pad08;
    CDefClassBase* PDef;                        // +0x0c
};

struct CDefIndexArray {                         // CArray<unsigned long> (std::vector)
    unsigned long* _M_start;
    unsigned long* _M_finish;
    unsigned long* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    const unsigned long& operator[](unsigned long i) const { return _M_start[i]; }
};

struct CDefClassInfo {                          // CDefinitionManager::CDefClassInfo
    CCharString    Name;                        // +0x00
    void*          AllocFunc;                   // +0x04
    CDefIndexArray GlobalIndices;               // +0x08
};
struct CDefClassInfoPair {                      // pair<const unsigned long, CDefClassInfo>, 0x18 bytes
    unsigned long  first;                       // +0x00
    CDefClassInfo  second;                      // +0x04
};

class CDefinitionManager {
public:
    unsigned char          _pad000[0x78];
    CDefClassInfoPair*     DefClassInfo_first;  // +0x78 CVectorMap<unsigned long, CDefClassInfo>
    CDefClassInfoPair*     DefClassInfo_last;   // +0x7c
    unsigned char          _pad080[0xa4 - 0x80];
    CInstantiatedDefInfo** InstantiatedDefs;    // +0xa4 CArray<CInstantiatedDefInfo*> begin
    const CDefClassInfoPair* DefClassInfoBegin() const { return DefClassInfo_first; }
    const CDefClassInfoPair* DefClassInfoEnd() const { return DefClassInfo_last; }
    void ValidateDefs(const CValidateDefinitionInitBase& init) const;
};

void CDefinitionManager::ValidateDefs(const CValidateDefinitionInitBase& init) const
{
    for (const CDefClassInfoPair* it = DefClassInfoBegin(); it != DefClassInfoEnd(); ++it) {
        const CDefClassInfo& classInfo = it->second;
        for (unsigned long i = 1; i < classInfo.GlobalIndices.size(); ++i) {
            CInstantiatedDefInfo* info = InstantiatedDefs[classInfo.GlobalIndices[i]];
            if (!info->PDef->Template) {
                CCharString name = g_DefStringTable.GetString(info->NameIndex);
                info->PDef->Validate(init, name);
            }
        }
    }
}

// ---- stubs and driver ----
void CDefClassBase::v00() { } void CDefClassBase::v01() { } void CDefClassBase::v02() { } void CDefClassBase::v03() { }
void CDefClassBase::v04() { } void CDefClassBase::v05() { } void CDefClassBase::v06() { } void CDefClassBase::v07() { }
void CDefClassBase::v08() { } void CDefClassBase::v09() { } void CDefClassBase::v10() { } void CDefClassBase::v11() { }
void CDefClassBase::v12() { } void CDefClassBase::v13() { } void CDefClassBase::v14() { } void CDefClassBase::v15() { }
void CDefClassBase::v16() { } void CDefClassBase::v17() { } void CDefClassBase::v18() { } void CDefClassBase::v19() { }
void CDefClassBase::v20() { } void CDefClassBase::v21() { } void CDefClassBase::v22() { }
static int g_gets = 0, g_validates = 0, g_namesDestroyed = 0;
static const CCharString* g_nameAddr = 0;
static long g_getIndex[8];
static const CDefClassBase* g_validated[8];
static const CValidateDefinitionInitBase* g_validInit[8];
CCharString::CCharString() { storage_ = 0; }
CCharString::~CCharString() { if (this == g_nameAddr) { ++g_namesDestroyed; g_nameAddr = 0; } }
CDefStringTable g_DefStringTable;
CCharString CDefStringTable::GetString(long index)
{
    CHECK(this == &g_DefStringTable);
    if (g_gets < 8) g_getIndex[g_gets] = index;
    ++g_gets;
    return CCharString();
}
void CDefClassBase::Validate(const CValidateDefinitionInitBase& init, const CCharString& name) const
{
    CHECK(g_nameAddr == 0);   // the previous name was destroyed before this call
    g_nameAddr = &name;
    if (g_validates < 8) { g_validated[g_validates] = this; g_validInit[g_validates] = &init; }
    ++g_validates;
}
struct CValidateDefinitionInitBase { long dummy; };
int main()
{
    static CDefClassBase defs[4];
    defs[0].Template = 0; defs[1].Template = 0; defs[2].Template = 1; defs[3].Template = 0;
    static CInstantiatedDefInfo infos[4];
    static CInstantiatedDefInfo* table[4];
    for (int k = 0; k < 4; ++k) { infos[k].NameIndex = 100 + k; infos[k].PDef = &defs[k]; table[k] = &infos[k]; }
    static unsigned long idx0[4] = { 0, 1, 2, 3 };   // index 0 skipped; 2 is a template
    static unsigned long idx1[1] = { 1 };            // only the class default: nothing
    static unsigned long idx2[2] = { 3, 0 };         // def 0 under index 1
    static CDefClassInfoPair pairs[3];
    pairs[0].second.GlobalIndices._M_start = idx0; pairs[0].second.GlobalIndices._M_finish = idx0 + 4;
    pairs[1].second.GlobalIndices._M_start = idx1; pairs[1].second.GlobalIndices._M_finish = idx1 + 1;
    pairs[2].second.GlobalIndices._M_start = idx2; pairs[2].second.GlobalIndices._M_finish = idx2 + 2;
    static CDefinitionManager mgr;
    mgr.InstantiatedDefs = table;
    CValidateDefinitionInitBase init;
    // 1) empty map: nothing
    mgr.DefClassInfo_first = pairs; mgr.DefClassInfo_last = pairs;
    mgr.ValidateDefs(init);
    CHECK(g_gets == 0 && g_validates == 0);
    // 2) three classes
    mgr.DefClassInfo_last = pairs + 3;
    mgr.ValidateDefs(init);
    CHECK(g_gets == 3 && g_validates == 3);
    CHECK(g_getIndex[0] == 101 && g_getIndex[1] == 103 && g_getIndex[2] == 100);
    CHECK(g_validated[0] == &defs[1] && g_validated[1] == &defs[3] && g_validated[2] == &defs[0]);
    CHECK(g_validInit[0] == &init && g_validInit[2] == &init);
    CHECK(g_namesDestroyed == 3 && g_nameAddr == 0);   // each name is destroyed after its Validate
    if (fails == 0) printf("009acd00_TEST PASS\n");
    return fails;
}