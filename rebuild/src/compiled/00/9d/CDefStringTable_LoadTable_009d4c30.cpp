// CDefStringTable::LoadTable(bool readOnly) -- if the table file exists, open it, read the string table from a buffered file stream, and mark the table locked (read-only as asked)
// Leaf: LoadTable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D4C30, length 164 bytes (+ int3 padding), `ret 4`.
// Callees: PathExists(const CWideString&) @ 0x00999230 (__fastcall), CBaseClassNonCopyable ctor @ 0x0099A2F0 / dtor @ 0x0099A300,
// CWideString::CWideString() @ 0x0099AED0 / ~CWideString @ 0x0099B510, CDiskFileWin32::Open(const CWideString&,
// FileAccessType, ulong) @ 0x0099A6A0 (read access 1, flags 4), CDiskFileWin32::Close @ 0x0099A920,
// CFileDataInputStream::CFileDataInputStream(CAFile*, ulong) @ 0x00994700 (0x4000-byte buffer) and its dtor
// @ 0x00994780, CDefStringTable::ReadTable(CDataInputStream*) @ 0x009D4AB0.
// Layout: retail CDefStringTable = Ego_r.pdb with a 12-byte map and a 0x18-byte memory stream: TableFilename +0x30,
// Locked +0x34, ReadOnly +0x35 (PDB +0x38/+0x3c/+0x3d). The local CDiskFileWin32 follows engine/CDiskFileWin32.h
// (vptr 0x0122D06C, Opened +0x14, Pathname +0x18), modelled with its virtual Open/Close as in the landed
// CBankFile::GetLastModifiedTime (0x009CBC50) family: Opened is cleared in the ctor body, the dtor calls Close().
// Shape: two flag stores scheduled before the stream's destructor; Open's arguments push right to left.
// NF134 (2026-11-20, q6180 / q6181): ported onto the shared engine/CAFile.h, CDiskFileWin32.h (DEFAULTCTOR, inline dtor) and
//   CFileDataInputStream.h (template: landed 0x00B3C5D0): the stream binds ??0CFileDataInputStream@@QAE@AAVCAFile@@K@Z 0x00994700 (Ego_r.pdb
//   0x00AFC7D0 + FableWin.pdb 0x030836B0) and ??1CFileDataInputStream@@UAE@XZ 0x00994780 (both PDBs, virtual); the file is passed as CAFile&
//   (CDiskFileWin32 : CAFile, RTTI). Was the row-local CBaseClassNonCopyable / CWideString / CAFile / CDiskFileWin32 / CFileDataInputStream models.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CDISKFILEWIN32_DEFAULTCTOR
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CFILEDATAINPUTSTREAM
#include "fable_string.h"
#include "engine/CAFile.h"
#include "engine/CDiskFileWin32.h"
#include "engine/CFileDataInputStream.h"

class CDefStringTable {
public:
    unsigned char _pad[0x30];                      // StringMap (12-byte retail map), StringStream, counts, RandomID
    CWideString TableFilename;                     // +0x30
    bool Locked;                                   // +0x34
    bool ReadOnly;                                 // +0x35
    void ReadTable(CFileDataInputStream* stream);  // 0x009D4AB0
    void LoadTable(bool readOnly);
};

void CDefStringTable::LoadTable(bool readOnly)
{
    if (CAFile::PathExists(TableFilename)) {
        CDiskFileWin32 file;
        file.Open(TableFilename, FILE_ACCESS_READ, 4);
        CFileDataInputStream stream(file, 0x4000);
        ReadTable(&stream);
        Locked = true;
        ReadOnly = readOnly;
    }
}