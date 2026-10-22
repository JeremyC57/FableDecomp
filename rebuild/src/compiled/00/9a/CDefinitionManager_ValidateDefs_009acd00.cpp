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