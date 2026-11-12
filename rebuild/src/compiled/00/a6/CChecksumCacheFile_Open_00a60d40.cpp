// CChecksumCacheFile::Open(const CWideString& filename) -- remember the cache file name, drop every cached entry, clear
//   NeedsSaving, then (when the file exists) read a version-4 cache: an entry count, then per entry the NUL-terminated name
//   (widened), its CDateAndTime stamp and CRC, stored into Entries[name].
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A60D40, 623 bytes, __fastcall, ret 4; PDB ?Open@CChecksumCacheFile@@QAEXABVCWideString@@@Z
//   (Ego_r 0x00C00760, FableWin same mangled name).
// Callees: CWideString::operator= 0x0099B7D0 / ~CWideString 0x0099B510 / CWideString() 0x0099AED0; Entries: inline STLport clear
//   (_M_erase 0x00A60A00), operator[] 0x00A617E0; CAFile::PathExists 0x00999230; inline CDiskFileWin32 ctor/dtor
//   (CBaseClassNonCopyable 0x0099A2F0 / 0x0099A300, Open 0x0099A6A0, Close 0x0099A920); CFileDataInputStream 0x00994700 /
//   ~ 0x00994780; inline CDataInputStream::Read (ReadWithSrcChunkOverflow 0x00993CA0); ReadNullTerminatedString 0x00994C80
//   (virtual, called direct); ReadDateAndTime 0x00995770; CCharString::ToWideString 0x0099E480 / ~CCharString 0x0099EAE0;
//   CDateAndTime() 0x009E9AB0.
// Shape: the version mismatch is an early `return` (a second exit through the inline ~CDiskFileWin32), which is what makes
//   VC7.1 keep the CDiskFileWin32 vptr 0x0122D06C in esi (re-loaded after the loop's rep movsd) and lay the CCharString temp
//   above the file. Layout: Filename +0, Entries +4 (retail STLport map, 12 bytes), BufferSize +0x10, NeedsSaving +0x14.
// Flags: none (header opt-ins FABLE_RETAIL_CDATAINPUTSTREAM_READDATEANDTIME, FABLE_RETAIL_CDATEANDTIME_CTOR).
// Leaf: ?Open@CChecksumCacheFile@@QAEXABVCWideString@@@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CDISKFILEWIN32_DEFAULTCTOR
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM_READDATEANDTIME
#define FABLE_RETAIL_CFILEDATAINPUTSTREAM
#define FABLE_RETAIL_CDATEANDTIME_CTOR
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/CDateAndTime.h"
#include "engine/CAFile.h"
#include "engine/CDiskFileWin32.h"
#include "engine/CFileDataInputStream.h"

class CChecksumCacheFile {
public:
    struct CEntry {
        CDateAndTime  TimeStamp;     // +0x00
        unsigned long Checksum;      // +0x1c
    };
private:
    CWideString Filename;                          // +0x00
    std::map<CWideString, CEntry> Entries;         // +0x04 (retail 12 bytes)
    long BufferSize;                               // +0x10
    bool NeedsSaving;                              // +0x14
public:
    void Open(const CWideString& filename);
};

void CChecksumCacheFile::Open(const CWideString& filename)
{
    Filename = filename;
    Entries.clear();
    NeedsSaving = false;
    if (!CAFile::PathExists(filename))
        return;
    CDiskFileWin32 file;
    file.Open(filename, FILE_ACCESS_READ, 2);
    CFileDataInputStream stream(file, 0x4000);
    if (stream.ReadSLONG() != 4)
        return;
    long count = stream.ReadSLONG();
    for (long i = 0; i < count; ++i) {
        CWideString name = stream.ReadNullTerminatedString().ToWideString();
        CEntry entry;
        entry.TimeStamp = stream.ReadDateAndTime();
        entry.Checksum = stream.ReadULONG();
        Entries[name] = entry;
    }
}