// CBankFileAsync::OpenFileReadOnly(const CWideString& filename, unsigned long flags) -- open the bank through the base CBankFile, then create the threaded (asynchronous) reader on the same path and re-enable async reads
// Retail: Fable.exe (TLC, VC7.1) @ 0x009D5820, 122 bytes (+6 int3), __fastcall, ret 8 (virtual override).
// Leaf: ?OpenFileReadOnly@CBankFileAsync@@UAE_NABVCWideString@@K@Z
// Callees: CBankFile::OpenFileReadOnly 0x009D1090 (qualified base call), operator new 0x00BFEA1A,
// CThreadedFile::CThreadedFile 0x0098DFD0, CCountedPointer<CThreadedFile>::Reset 0x009A9C80 (landed),
// CBankFile::GetBankPathname() 0x009CBF10 (CWideString by hidden pointer, landed), CThreadedFile::Open(const
// CWideString&, bool) 0x0098E1E0 (landed), CWideString::~CWideString 0x0099B510.
// Layout: Ego_r.pdb CBankFileAsync at the retail offsets (0x74 lower than the PDB, the retail CBankFile base
// being smaller): ThreadedFile (CCountedPointer: object, count block) +0x110, NonCached +0x148,
// DisableAsync +0x149.
// Shape: the base call fails straight out; `new CThreadedFile` (0x1C bytes, null-checked) goes into the
// counted pointer through its out-of-line Reset; the file pointer is fetched before the Open arguments are
// evaluated -- NonCached is pushed first, then GetBankPathname builds its temporary in the dead `filename`
// argument slot -- and the temporary dies right after the call.
// Naming (NF129, 2026-11-20, q6179): the local stand-in CCountedPointer_CThreadedFile (and its SCountBlock) replaced by the shared
//   engine/CCountedPointer.h CCountedPointer<CThreadedFile> (Ego_r.pdb CBankFileAsync::ThreadedFile), so the Reset call binds
//   FableWin.pdb ?Reset@?$CCountedPointer@VCThreadedFile@@@@QAEXPAVCThreadedFile@@@Z (0x009A9C80), as the landed 0x009D56C0 does.
// Port (NF177, with 0x009D56C0 for q6927): moved off the row-local CWideString / CThreadedFile / CBankFile_Retail / CBankFileAsync models
// onto fable_string.h, fable_threaded_file.h, engine/CBankFile.h and engine/CBankFileAsync.h, so the base call binds the 0x009D1090
// self-row symbol ?OpenFileReadOnly@CBankFile@@UAE_NABVCWideString@@K@Z and the path getter Ego_r.pdb / FableWin.pdb
// ?GetBankPathname@CBankFile@@QBE?AVCWideString@@XZ (0x009CBF10; its only other referencer 0x009D56C0 moves in the same hand-back).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTHREADEDFILE
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CBANKFILE_GETBANKPATHNAME
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "fable_threaded_file.h"
#include "engine/CBankFile.h"
#include "engine/CBankFileAsync.h"

bool CBankFileAsync::OpenFileReadOnly(const CWideString& filename, unsigned long flags)
{
    if (!CBankFile::OpenFileReadOnly(filename, flags))
        return false;
    ThreadedFile.Reset(new CThreadedFile);
    ThreadedFile->Open(GetBankPathname(), NonCached);
    DisableAsync = false;
    return true;
}
