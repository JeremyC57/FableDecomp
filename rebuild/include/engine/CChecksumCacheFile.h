#ifndef FABLE_ENGINE_CCHECKSUMCACHEFILE_H
#define FABLE_ENGINE_CCHECKSUMCACHEFILE_H
#ifdef FABLE_RETAIL_CBANKFILE_UPDATE_PASS27

#include "fable_string.h"
#include "engine/CDateAndTime.h"
#include "stlport/fable_map.h"

class CChecksumCacheFile {
public:
    CChecksumCacheFile(long buffer_size);
    ~CChecksumCacheFile();
    void Open(const CWideString& path);
    void Close();
    unsigned long GetChecksum(const CWideString& path);
private:
    struct CEntry {
        CDateAndTime TimeStamp;
        unsigned long Checksum;
    };
    CWideString Filename;
    std::map<CWideString, CEntry> Entries;
    long BufferSize;
    bool NeedsSaving;
};

FABLE_STATIC_ASSERT(sizeof(CChecksumCacheFile) == 0x18);
#endif
#endif
