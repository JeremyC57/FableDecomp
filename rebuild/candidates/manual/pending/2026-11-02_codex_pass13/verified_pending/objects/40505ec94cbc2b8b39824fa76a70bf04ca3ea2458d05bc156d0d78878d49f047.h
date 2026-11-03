#ifndef FABLE_ENGINE_CCHECKSUMCACHEFILE_RENDER2D_H
#define FABLE_ENGINE_CCHECKSUMCACHEFILE_RENDER2D_H
#include "rebuild_abi.h"
#include "engine/CArray.h"
#include "engine/CDateAndTime.h"
#include "fable_string.h"
#include "stlport/fable_map.h"
// Original Ego_r and FableWin declarations: evidence/009da9f0_*_types.json and *_dependencies.json.
class CChecksumCacheFile {
private:struct CEntry {
public: CDateAndTime TimeStamp;
public: unsigned long Checksum;
};
private: CWideString Filename;
private: std::map<CWideString,CChecksumCacheFile::CEntry,std::less<CWideString>,std::allocator<std::pair<CWideString const ,CChecksumCacheFile::CEntry> > > Entries;
private: long BufferSize;
private: bool NeedsSaving;
public: CChecksumCacheFile(const CWideString &,long);
public: CChecksumCacheFile(long);
public: ~CChecksumCacheFile();
public: unsigned long GetChecksum(const CWideString &);
public: void Open(const CWideString &);
public: void Close();
public: bool IsOpen();
private: void Save();
};
#endif
