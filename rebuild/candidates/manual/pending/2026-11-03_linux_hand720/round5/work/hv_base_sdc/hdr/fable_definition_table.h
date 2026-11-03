#pragma once

#include <stddef.h>

#include "fable_string.h"

class CDefStringTableRecovered
{
public:
    void SetTablePath(const CWideString& pathname);

    fable_u8 unknown00_[0x30];
    CWideString tablePath_;
};

FABLE_STATIC_ASSERT(
    offsetof(CDefStringTableRecovered, tablePath_) == 0x30);

#ifdef FABLE_RETAIL_CDEFSTRINGTABLE
// Retail CDefStringTable (Ego_r TPI CDefStringTable member names; retail sizeof 0x38: the STLport map is 12 bytes and
// CMemoryDataOutputStream 24, so TableFilename sits at +0x30 as in the default's tablePath_). GetString 0x009D49B0
// (FableWin ?GetString@CDefStringTable@@QBE?AVCCharString@@J@Z 0x02FCCF40); GDefStringTable 0x013CA828.
// Members this model does not use stay opaque storage at their retail offsets.
class CDefStringTable
{
public:
    void LoadTable(bool readOnly);                                       // ?LoadTable@CDefStringTable@@QAEX_N@Z
    CCharString GetString(long pos) const;                               // 0x009D49B0
private:
    unsigned char StringMap[12];                                         // +0x00 std::map<CCharString,long>
    unsigned char StringStream[24];                                      // +0x0C CMemoryDataOutputStream
    unsigned long StringCount;                                           // +0x24
    unsigned long SharedCount;                                           // +0x28
    unsigned long RandomID;                                              // +0x2C
    CWideString TableFilename;                                           // +0x30
    bool Locked;                                                         // +0x34
    bool ReadOnly;                                                       // +0x35
};
FABLE_STATIC_ASSERT(sizeof(CDefStringTable) == 0x38);
extern CDefStringTable GDefStringTable;                                  // 0x013CA828 ?GDefStringTable@@3VCDefStringTable@@A
#else
class CDefStringTable
{
public:
    void LoadTable(bool readOnly);

    fable_u8 unknown00_[0x30];
    CWideString tablePath_;
    bool loaded_;
    bool readOnly_;
};

FABLE_STATIC_ASSERT(offsetof(CDefStringTable, tablePath_) == 0x30);
FABLE_STATIC_ASSERT(offsetof(CDefStringTable, loaded_) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CDefStringTable, readOnly_) == 0x35);
#endif // FABLE_RETAIL_CDEFSTRINGTABLE
