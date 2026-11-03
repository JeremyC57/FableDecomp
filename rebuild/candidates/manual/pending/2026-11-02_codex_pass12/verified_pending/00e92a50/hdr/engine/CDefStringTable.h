#ifdef FABLE_RETAIL_GFMAIN
#ifndef FABLE_ENGINE_CDEFSTRINGTABLE_H
#define FABLE_ENGINE_CDEFSTRINGTABLE_H
#include "rebuild_abi.h"
#include "engine/CCharString.h"
// Original Ego_r TPI0xb662; native ctor009d4db0 and GetString009d49b0.
// Original composite members not used here retain their full native storage.
class CDefStringTable {
public:
#ifdef FABLE_RETAIL_GFMAIN
 void SetTableFilename(const CWideString&);
 void LoadTable(bool);
#endif
    CCharString GetString(long) const;
#ifdef FABLE_RETAIL_FACTIONS
    unsigned long GetStringCRC(long)const;
#endif
private:
    unsigned char StringMap[12]; // std::map<CCharString,long,...>
    unsigned char StringStream[24]; // CMemoryDataOutputStream
    unsigned long StringCount,SharedCount,RandomID;
    unsigned char TableFilename[4]; // CWideString
    bool Locked,ReadOnly;
};
FABLE_STATIC_ASSERT(sizeof(CDefStringTable)==0x38);
extern CDefStringTable GDefStringTable;
#endif

#else
#ifndef FABLE_ENGINE_CDEFSTRINGTABLE_H
#define FABLE_ENGINE_CDEFSTRINGTABLE_H
#include "rebuild_abi.h"
#include "engine/CCharString.h"
// Original Ego_r TPI0xb662; native ctor009d4db0 and GetString009d49b0.
// Original composite members not used here retain their full native storage.
class CDefStringTable {
public:
    CCharString GetString(long) const;
#ifdef FABLE_RETAIL_FACTIONS
    unsigned long GetStringCRC(long)const;
#endif
private:
    unsigned char StringMap[12]; // std::map<CCharString,long,...>
    unsigned char StringStream[24]; // CMemoryDataOutputStream
    unsigned long StringCount,SharedCount,RandomID;
    unsigned char TableFilename[4]; // CWideString
    bool Locked,ReadOnly;
};
FABLE_STATIC_ASSERT(sizeof(CDefStringTable)==0x38);
extern CDefStringTable GDefStringTable;
#endif

#endif
