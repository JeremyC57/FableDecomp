// CBankFileAsync::OpenReadOnly(const CCharString& bankHandle, unsigned long flags) -- open the bank read-only through CBankFile, report progress, then take the asynchronous reader: the retail bank set's threaded file (and its DMA setting) when attached to one, else a new threaded file opened on the bank's own path; async reads enabled
// Leaf: ?OpenReadOnly@CBankFileAsync@@UAE_NABVCCharString@@K@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local SCountBlock (= CCPPointerInfo) members m_references->RefCount, m_pfnDestroy->DeleteFunc, m_pObject->Data are the header CCPPointerInfo's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D56C0, length 352 bytes (+ int3 padding), virtual, ret 8.
// Callees: CBankFile::OpenReadOnly @ 0x009D06F0 (qualified base call), CCharString ctor @ 0x0099EBF0 ("..." @ 0x0129B884) /
//          dtor @ 0x0099EAE0, NProgressDisplay::DisplayProgress(const CCharString&, float, bool, bool) @ 0x009E9F40
//          (__fastcall, (text, -1.0f, false, false)), CBankFileManager::FindContainedBank @ 0x009A7F80 on the manager
//          singleton @ 0x013CA79C, CCountedPointer<CThreadedFile>::operator= @ 0x009D6FD0 (catalog label is the
//          CDiskFileWin32 instantiation) / ~ @ 0x009A9C40, CCountedPointer<CRetailBank>::~ @ 0x009A9D60, operator new
//          @ 0x00BFEA1A, CThreadedFile ctor @ 0x0098DFD0 / Open(const CWideString&, bool) @ 0x0098E1E0,
//          CCountedPointer<CThreadedFile>::Reset @ 0x009A9C80, CBankFile::GetBankPathname @ 0x009CBF10 (CWideString by
//          hidden pointer), CWideString dtor @ 0x0099B510.
// Layout: retail CBankFileAsync as the landed ctor 0x009D5F80 / OpenFileReadOnly 0x009D5820 (CBankFile base 0x110:
//          RetailMode +0x8c; ThreadedFile +0x110, NonCached +0x148, DisableAsync +0x149); Ego_r.pdb CRetailBank: DiskFile
//          +4, ThreadedFile +0xc, UseDMA +0x14; FindContainedBank's 20-byte SBankEntryInfo.
// Shape: the progress text is a temporary homed in the dead `flags` argument slot; the contained bank's threaded file
//          comes through an inline by-value getter on a NAMED pointee local (`CRetailBank* retailBank = bank.Data;` -- the
//          temporary's two words are then loaded before either is stored, retail's `cmp eax,ebx` ahead of the stores),
//          assigned out of line and destroyed at once; the disk arm is the landed OpenFileReadOnly tail (GetBankPathname
//          temporary in the same dead slot).
// Port (NF177, q6927; coordinator call: a port onto the shared header, not an in-place respell): moved off the row-local CCharString /
// CWideString / CThreadedFile / CRetailBank / SBankEntryInfo / CBankFileManager / CBankFile_Retail / CBankFileAsync models onto
// fable_string.h, fable_threaded_file.h, fable_retail_banks.h, engine/CBankFile.h, engine/CBankFileAsync.h and
// engine/NProgressDisplay.h. The counted bank pointer is CCountedPointer<NBankFileManager::CRetailBank> (destructor 0x009A9D60, the
// self row's explicit instantiation), FindContainedBank takes both outputs by reference (Ego_r.pdb, as the landed 0x009A7F80 Leaf), the
// manager is NBankFileManager::GBankFileManager (both PDBs name its initializer / atexit destructor), the threaded-file getter is FableWin.pdb
// NBankFileManager::CRetailBank::GetThreadedFile (inline, QAE) and the path is Ego_r.pdb CBankFile::GetBankPathname (QBE).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTHREADEDFILE
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CBANKFILE_GETBANKPATHNAME
#define FABLE_RETAIL_CBANKFILEMANAGER
#define FABLE_RETAIL_CBANKFILEMANAGER_HEADERPATHS
#define FABLE_RETAIL_CBANKFILEMANAGER_FINDCONTAINEDBANK
#define FABLE_RETAIL_NBANKFILEMANAGER_CRETAILBANK_GETTHREADEDFILE
#include "fable_compiler.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "fable_threaded_file.h"
#include "fable_retail_banks.h"
#include "engine/CBankFile.h"
#include "engine/CBankFileAsync.h"
#include "engine/NProgressDisplay.h"

bool CBankFileAsync::OpenReadOnly(const CCharString& bankHandle, unsigned long flags)
{
    if (!CBankFile::OpenReadOnly(bankHandle, flags))
        return false;
    NProgressDisplay::DisplayProgress("Open Bank File Async", -1.0f, false, false);
    if (RetailMode) {
        NBankFileManager::CContainedBank contained_bank;
        contained_bank.DataVersion = 0;
        contained_bank.EntryCount = 0;
        contained_bank.InfoOffset = 0;
        contained_bank.InfoSize = 0;
        contained_bank.Alignment = 0;
        CCountedPointer<NBankFileManager::CRetailBank> bank;
        if (!NBankFileManager::GBankFileManager.FindContainedBank(bankHandle, contained_bank, bank))
            return false;
        NBankFileManager::CRetailBank* retailBank = bank.Data;
        ThreadedFile = retailBank->GetThreadedFile();
        NonCached = bank->UseDMA;
        DisableAsync = false;
        return true;
    }
    ThreadedFile.Reset(new CThreadedFile);
    ThreadedFile->Open(GetBankPathname(), NonCached);
    DisableAsync = false;
    return true;
}
