#ifndef FABLE_ENGINE_CDEFSTRINGTABLE_H
#define FABLE_ENGINE_CDEFSTRINGTABLE_H
// CDefStringTable (new, chunk 767): no RTTI (no virtuals; Ego_r.pdb ??1CDefStringTable@@QAE@XZ). The global table is g_DefStringTable
// 0x013CA828 (landed ctor 0x009D4E90 and its atexit stub 0x0122A780). Ego_r.pdb struct_layouts_egor.tsv (0x40): StringMap (map) +0,
// StringStream (CMemoryDataOutputStream), StringCount, SharedCount, RandomID, TableFilename (CWideString), Locked, ReadOnly; with
// retail STLport sizes TableFilename sits at +0x30 (SetTableFilename 0x009D4A40 assigns [ecx+30h]). Only what rows use is modelled.
// Name set (Ego_r.pdb donor order = retail order): GetString 0x009D49B0, GetStringAsCharArray 0x009D49F0, GetStringCRC 0x009D4A20,
// SetTableFilename 0x009D4A40, WriteTable 0x009D4A50, ReadTable 0x009D4AB0.
#include "rebuild_abi.h"
#include "fable_string.h"
class CDefStringTable {
public:
    void SetTableFilename(const CWideString& filename);   // 0x009D4A40
protected:
    unsigned char StringData[0x30];                        // +0x00 StringMap, StringStream, StringCount, SharedCount, RandomID (opaque)
    CWideString   TableFilename;                           // +0x30
    bool          Locked;                                  // +0x34
    bool          ReadOnly;                                // +0x35
};
#endif // FABLE_ENGINE_CDEFSTRINGTABLE_H
