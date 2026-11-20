// CBankFile::ReadRecoveryFile(unsigned long validation) -- reload the entry list from the bank's recovery file: false when the file
//   is missing, its "BREC" signature does not match or its validation word differs from `validation`; otherwise read the two
//   entry-list words and hand the stream to ReadEntryList
// Leaf: ?ReadRecoveryFile@CBankFile_Retail@@QAE_NK@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x009D0380, 679 bytes, ret 4; PDB ?ReadRecoveryFile@CBankFile@@AAE_NK@Z. Flags: /GS (cookie
//   0x0139C8A8, check 0x00BFE9F9; char signature[5] from "BREC" 0x0129B59C, as the landed WriteRecoveryFile 0x009CDF50 writes it).
// Callees: CBankFile::GetRecoveryFilePath 0x009CBFE0 (PDB AAE?AVCWideString@@XZ), CAFile::PathExists 0x00999230 (static
//   __fastcall, PDB ?PathExists@CAFile@@SI_NABVCWideString@@@Z), CBaseClassNonCopyable ctor 0x0099A2F0 / dtor 0x0099A300,
//   CDiskFileWin32 (vtable 0x0122D06C) Open 0x0099A6A0 / Close 0x0099A920, CWideString default ctor 0x0099AED0 / dtor 0x0099B510,
//   CFileDataInputStream ctor 0x00994700 / dtor 0x00994780, CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (inline Read,
//   as the landed 0x00B16F60), CBankFile::ReadEntryList 0x009CFBC0 (PDB AAE_NAAVCFileDataInputStream@@KK@Z, result dropped).
// Layout: CDiskFileWin32 / CFileDataInputStream as Ego_r.pdb (engine/CDiskFileWin32.h, engine/CFileDataInputStream.h: 0x24
//   bytes) restated locally because the inline Read and the inline file ctor/dtor need bodies; CDataInputStream member names
//   from engine/CDataInputStream.h. No CBankFile member is touched.
// Shape: the body is one `if (PathExists)` with the failure return after it (retail places that block last); the signature and
//   validation checks sit in their own blocks so the two trailing words reuse the signature's frame slot and the dead
//   validation argument slot, as retail.
// NF134 (2026-11-20, q6180 / q6181): ported onto the shared engine/CAFile.h, CDiskFileWin32.h (DEFAULTCTOR, inline dtor), CDataInputStream.h
//   (inline Read) and CFileDataInputStream.h (template: landed 0x00B3C5D0 / 0x009A8840): the stream binds ??0CFileDataInputStream@@QAE@AAVCAFile@@K@Z
//   0x00994700 (Ego_r.pdb 0x00AFC7D0 + FableWin.pdb 0x030836B0) and ??1CFileDataInputStream@@UAE@XZ 0x00994780 (both PDBs, virtual), the file is
//   passed as CAFile& (CDiskFileWin32 : CAFile, RTTI). Was the row-local CWideString / CAFile / CBaseClassNonCopyable / CDiskFileWin32 /
//   CDataInputStream / CFileDataInputStream models and a (CAFile*) cast.
#include "fable_compiler.h"
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
extern "C" int __cdecl memcmp(const void*, const void*, unsigned int);
#pragma intrinsic(memcmp)
struct CBankFile_Retail {
    virtual void s0();
    CWideString GetRecoveryFilePath();                 // 0x009CBFE0
    bool ReadEntryList(CFileDataInputStream& stream, unsigned long a, unsigned long b);   // 0x009CFBC0
    bool ReadRecoveryFile(unsigned long validation);
};

bool CBankFile_Retail::ReadRecoveryFile(unsigned long validation)
{
    CWideString path = GetRecoveryFilePath();
    if (CAFile::PathExists(path)) {
        CDiskFileWin32 file;
        file.Open(path, FILE_ACCESS_READ, 4);
        CFileDataInputStream stream(file, 0x4000);
        unsigned long header;
        {
            char signature[] = "BREC";
            stream.Read(&header, 4);
            if (memcmp(&header, signature, 4) != 0)
                return false;
        }
        {
            unsigned long value;
            stream.Read(&value, 4);
            if (value != validation)
                return false;
        }
        {
            unsigned long first;
            stream.Read(&first, 4);
            unsigned long second;
            stream.Read(&second, 4);
            ReadEntryList(stream, first, second);
        }
        return true;
    }
    return false;
}