// CBankFileManager::OpenRetailBank(const CWideString& pathname, bool startupPriority) -- open a retail .big bank: check the BIGB magic and the file version, register a CRetailBank holding the disk file and path, read its contained-bank table, then attach a threaded file on the path
// Leaf: ?OpenRetailBank@CBankFileManager@@QAEXABVCWideString@@_N@Z
// Flags: none
// Retail: Fable.exe @ 0x009A8840, 1565 bytes, thiscall `ret 8`; Ego_r.pdb (0x00AF6520) / FableWin.pdb
// ?OpenRetailBank@CBankFileManager@@QAEXABVCWideString@@_N@Z. Replaces the byte-baked oracle and its FableOpenRetail* thunks.
// Body: when the path exists, a counted CDiskFileWin32 (inline default ctor) is opened for read and wrapped in a 16 KB
// CFileDataInputStream. A "BIGB" magic (memcmp intrinsic: one dword compare against the literal at 0x0129B07C) and a version
// equal to FileVersion (+0x34) admit the file: a new CRetailBank goes on RetailBanks (+0x10) and gets the path (as a
// CCharString) and the disk file. The header block (offset, size) is read into a CArray<char> and parsed through a
// CMemoryDataInputStream: a count, then per contained bank its name and five words into ContainedBanks[name]. Last, the
// bank's threaded file is Reset to a new CThreadedFile, UseDMA cleared, and the file opened (no caching flag false).
// startupPriority is unused (retail pushes a constant 0 to CThreadedFile::Open).
// Inline split: every CCountedPointer member is the plain in-class body of engine/CCountedPointer.h. VC7.1's per-function
// inline budget then reproduces retail's mix by itself: the (T*) ctors (0x009A9B60 / 0x009A9D10), operator= (0x009A9BF0),
// Reset (0x009A9C80) and the version-mismatch exit's ~CCountedPointer<CDiskFileWin32> (0x009A9BB0) are called, the other
// two exits' releases are expanded. Local stand-in models with declared-only members (2026-10-23, DIFFER 1581v1565) needed a
// mid-function  for that exit and still differed in register choice.
// Stream reads are the __forceinline CDataInputStream::Read / ReadULONG (the 64-bit position range test before the chunk copy).
// Layout: fable_retail_banks.h retail branch (FABLE_RETAIL_CBANKFILEMANAGER), engine/CDiskFileWin32.h default ctor
// (FABLE_RETAIL_CDISKFILEWIN32_DEFAULTCTOR), fable_threaded_file.h retail CThreadedFile (FABLE_RETAIL_CTHREADEDFILE).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CDISKFILEWIN32_DEFAULTCTOR
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CFILEDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CTHREADEDFILE
#define FABLE_RETAIL_CBANKFILEMANAGER
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CAFile.h"
#include "engine/CDiskFileWin32.h"
#include "engine/CFileDataInputStream.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CCountedPointer.h"
#include "engine/CArray.h"
#include "fable_threaded_file.h"
#include "fable_retail_banks.h"
#include <string.h>

using NBankFileManager::CRetailBank;
using NBankFileManager::CContainedBank;

void CBankFileManager::OpenRetailBank(const CWideString& pathname, bool startupPriority)
{
    if (CAFile::PathExists(pathname)) {
        CCountedPointer<CDiskFileWin32> disk_file(new CDiskFileWin32);
        disk_file->Open(pathname, FILE_ACCESS_READ, 2);
        CFileDataInputStream stream(*disk_file, 0x4000);
        char magic[4];
        stream.Read(magic, 4);
        if (memcmp(magic, "BIGB", 4) == 0) {
            if (stream.ReadULONG() != FileVersion)
                return;
            CCountedPointer<CRetailBank> bank(new CRetailBank);
            RetailBanks.push_back(bank);
            bank->FilePath = pathname.ToCharString();
            bank->DiskFile = disk_file;
            unsigned long header_offset = stream.ReadULONG();
            unsigned long header_size = stream.ReadULONG();
            if (header_size > 0) {
                CArray<char> header(header_size);
                stream.SetPosition(header_offset);
                stream.Read(header.GetAsCArray(), header_size);
                CMemoryDataInputStream header_stream(header.GetAsCArray(), header_size);
                unsigned long no_banks = header_stream.ReadULONG();
                for (unsigned long i = 0; i < no_banks; ++i) {
                    CCharString name = header_stream.ReadNullTerminatedString();
                    CContainedBank contained;
                    contained.DataVersion = header_stream.ReadULONG();
                    contained.EntryCount = header_stream.ReadULONG();
                    contained.InfoOffset = header_stream.ReadULONG();
                    contained.InfoSize = header_stream.ReadULONG();
                    contained.Alignment = header_stream.ReadULONG();
                    bank->ContainedBanks[name] = contained;
                }
            }
            bank->ThreadedFile.Reset(new CThreadedFile);
            bank->UseDMA = false;
            bank->ThreadedFile->Open(pathname, false);
        }
    }
}