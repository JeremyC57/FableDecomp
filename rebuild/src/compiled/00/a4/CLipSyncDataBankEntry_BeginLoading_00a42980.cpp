// CLipSyncDataBankEntry::BeginLoading(unsigned long index, CLipSyncDataBank& bank) -- when the entry has no data resource yet, create one
// for this entry and bank, start the asynchronous read of the whole bank entry at the lip-sync loading priority and queue the resource
// on the bank's resource bank
// Leaf: ?BeginLoading@CLipSyncDataBankEntry@@AAEXKAAVCLipSyncDataBank@@@Z
// Identity: Ego_r.pdb 0x00B544B0 / FableWin.pdb 0x0310FE80 ?BeginLoading@CLipSyncDataBankEntry@@AAEXKAAVCLipSyncDataBank@@@Z (private;
//   the landed caller CLipSyncDataBank::BeginLoadingEntry 0x00A42AC0 already binds this spelling).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A42980, 253 bytes.
// Callees: operator new 0x00BFEA1A, CResource::CResource 0x009FBEC0 (the inline CData constructor, FableWin.pdb
//   ??0CData@CLipSyncDataBankEntry@@QAE@AAV1@AAVCLipSyncDataBank@@@Z; vtable 0x0129D3A4), CBankFileAsync::GetAsyncEntry 0x009D58D0,
//   CBankFile::GetEntryDataSize 0x009CC240, CBankFileAsyncEntry::BeginReadDataAutoAlloc 0x009D6100, CCountedPointer<CBankFileAsyncData>
//   operator= 0x009A4440, CResourceBank::AddToLoadingQueue 0x009FC3D0 (nf44:83), operator delete 0x00BFE9BC (count blocks).
// Port (NF68): the local ResourceNative / IVNative / BankNative / ResourceBankNative / Counted models are gone: engine/CResource.h and
//   engine/CIVCountedPointeeBase.h (FABLE_RETAIL_DEF_CLASS_BASE), engine/CIVCountedPointer.h (Reset / Release), engine/CCountedPointer.h,
//   engine/CBankFile.h + engine/CBankFileAsync.h (GetAsyncEntry, GetEntryDataSize), engine/CResourceBank.h (AddToLoadingQueue) and the
//   overlay opt-in FABLE_RETAIL_CLIPSYNCDATABANK_CLASS (the retail CLipSyncDataBank : CBankFileAsync). Local models remain only for the
//   classes with no shared header: CLipSyncDataBankEntry (Ego_r.pdb: Data, Duration) with its CData (Ego_r.pdb: LipSyncData, FileData,
//   Owner, Bank) and CBankFileAsyncEntry (declaration of BeginReadDataAutoAlloc only).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CBANKFILEASYNC_DATA
#define FABLE_RETAIL_CBANKFILEASYNC_GETASYNCENTRY
#define FABLE_RETAIL_CBANKFILE_ENTRYDATASIZE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CLIPSYNCDATABANK_CLASS
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CBankFile.h"
#include "engine/CBankFileAsync.h"
#include "engine/CResource.h"
#include "engine/CResourceBank.h"
#include "engine/CIVCountedPointer.h"
#include "engine/EThreadedFilePriority.h"
#include "engine/CLipSyncDataBank.h"

class CBankFileAsyncData;
class CBankFileAsyncEntry {
public:
    CCountedPointer<CBankFileAsyncData> BeginReadDataAutoAlloc(unsigned long offset, unsigned long size, EThreadedFilePriority priority);   // 0x009D6100
};

// The read priority global of the lip-sync bank (0x01397700); no PDB name is known for it (descriptive, as landed).
extern EThreadedFilePriority LipSyncLoadingPriority;

class CLipSyncDataBankEntry {
public:
    class CData : public CResource {
    public:
        CData(CLipSyncDataBankEntry& owner, CLipSyncDataBank& bank) : Owner(&owner), Bank(&bank) {}
        virtual ~CData();
        virtual bool UnloadResource();
        virtual bool IsResourceLoaded() const;
        virtual void OnResourceLoaded();
        CCountedPointer<CLipSyncSampleData> LipSyncData;   // +0x1c
        CCountedPointer<CBankFileAsyncData> FileData;      // +0x24
        CLipSyncDataBankEntry*              Owner;         // +0x2c
        CLipSyncDataBank*                   Bank;          // +0x30
    };
private:
    friend class CLipSyncDataBank;
    void BeginLoading(unsigned long index, CLipSyncDataBank& bank);
    CIVCountedPointer<CData> Data;                           // +0x00
    float                    Duration;                       // +0x04
};
FABLE_STATIC_ASSERT(sizeof(CLipSyncDataBankEntry::CData) == 0x34);
FABLE_STATIC_ASSERT(sizeof(CLipSyncDataBankEntry) == 0x8);

void CLipSyncDataBankEntry::BeginLoading(unsigned long index, CLipSyncDataBank& bank)
{
    if (Data.IsNull()) {
        Data.Reset(new CData(*this, bank));
        CCountedPointer<CBankFileAsyncEntry> entry = bank.GetAsyncEntry(index);
        unsigned long size = bank.GetEntryDataSize(index);
        Data->FileData = entry->BeginReadDataAutoAlloc(0, size, LipSyncLoadingPriority);
        bank.ResourceBank->AddToLoadingQueue(*Data);
    }
}
