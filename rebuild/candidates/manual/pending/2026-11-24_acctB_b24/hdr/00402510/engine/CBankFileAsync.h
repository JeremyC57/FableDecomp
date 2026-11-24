#ifndef FABLE_ENGINE_CBANKFILEASYNC_H
#define FABLE_ENGINE_CBANKFILEASYNC_H
#if defined(FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS) || defined(FABLE_RETAIL_CBANKFILEASYNC_DATA) || defined(FABLE_RETAIL_NGAMETEXT_DATABANK_GROUPTEXT)
#include "engine/CBankFile.h"
#include "engine/CCountedPointer.h"
#ifdef FABLE_RETAIL_CBANKFILEASYNC_DATA
#include "stlport/fable_map.h"
// Win32 SDK _RTL_CRITICAL_SECTION: six 32-bit fields, sizeof 24.
struct _RTL_CRITICAL_SECTION { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
#endif
class CThreadedFile;class CMovableResourceMemoryPool;class CLoadingMemoryFailureHandler;class CBankFileAsyncData;
// Complete original member range on the established0x110 retail CBankFile base.
class CBankFileAsync : public CBankFile {
public:
    virtual ~CBankFileAsync();
    virtual bool OpenReadOnly(const CCharString&,unsigned long);
    virtual void OpenReadWrite(const CCharString&,unsigned long,unsigned long);
    virtual bool OpenFileReadOnly(const CWideString&,unsigned long);
    virtual void OpenFileReadWrite(const CWideString&,unsigned long,unsigned long);
    virtual void Close();
#ifdef FABLE_RETAIL_CBANKFILEASYNC_SETENABLEFAILUREHANDLING
    // b24c4 opt-in (row 0x00402510 GFMain): Ego_r.pdb ?SetEnableFailureHandling@CBankFileAsync@@SIX_N@Z, out of line
    // in retail (0x009D5240, `mov cl,[..] ; call`). Declaration only.
    static void __fastcall SetEnableFailureHandling(bool enable);        // 0x009D5240
#endif
#ifdef FABLE_RETAIL_CBANKFILEASYNC_DATA
    friend class CBankFileAsyncData;
#endif
#ifdef FABLE_RETAIL_NGAMETEXT_DATABANK_GROUPTEXT
    // Opt-in (row 0x009C9280 NGameText::CDataBank::GetTextEntryFromGroup): Ego_r.pdb ?AsyncUpdatePending@CBankFileAsync@@QAEXXZ,
    // out of line at 0x009D68B0 (landed GetPTextEntry 0x009C9070 calls it the same way). Declaration only.
    void AsyncUpdatePending();                                           // 0x009D68B0
#endif
#if defined(FABLE_RETAIL_CBANKFILEASYNC_ASYNCUPDATEPENDING) && !defined(FABLE_RETAIL_NGAMETEXT_DATABANK_GROUPTEXT)
    // Opt-in (NF53, round 7, rows 0x009F8230 / 0x00B3B7F0): the same Ego_r.pdb / FableWin.pdb ?AsyncUpdatePending@CBankFileAsync@@QAEXXZ
    // (Ego_r 0x00B2ABD0 / FableWin 0x02FF6B50; out of line at 0x009D68B0) for TUs that do not take the NGameText group-text opt-in.
    // Declaration only.
    void AsyncUpdatePending();                                           // 0x009D68B0
#endif
#ifdef FABLE_RETAIL_CBANKFILEASYNC_UPDATEPENDING_BODY
    // NF72 opt-in (round 7, q10905, row 0x009D68B0 AsyncUpdatePending): Ego_r.pdb 0x00B2A6E0 / FableWin.pdb 0x02FF6EC0
    // ?ProcessDisposalList@CBankFileAsync@@QAEXXZ (public, retail 0x009D6730) and Ego_r.pdb 0x00B29C60 / FableWin.pdb 0x02FF6F90
    // ?RestartAbortedReads@CBankFileAsync@@AAEXXZ (private, retail 0x009D5A90), the two calls AsyncUpdatePending makes.
    // Declarations only; pair with FABLE_RETAIL_CBANKFILEASYNC_ASYNCUPDATEPENDING for AsyncUpdatePending itself.
    void ProcessDisposalList();                                          // 0x009D6730
private:
    void RestartAbortedReads();                                          // 0x009D5A90
public:
#endif
#ifdef FABLE_RETAIL_CBANKFILEASYNC_HANDLEFAILEDALLOCATION
    // Retail opt-in (chunk F2, row 0x009D5BF0): Ego_r.pdb / FableWin.pdb ?HandleFailedAllocation@CBankFileAsync@@QAE_NXZ. Declaration only.
    bool HandleFailedAllocation();                                       // 0x009D5BF0
#endif
#ifdef FABLE_RETAIL_CBANKFILEASYNC_SETLOADINGMEMORY
    // Retail opt-in (wfE7, row 0x00B3CB30): Ego_r.pdb / FableWin.pdb ?SetLoadingMemoryPool@CBankFileAsync@@QAEXV?$CCountedPointer@VCMovableResourceMemoryPool@@@@@Z
    // (retail 0x00B3E820) and ?SetLoadingMemoryFailureHandler@CBankFileAsync@@QAEXV?$CCountedPointer@VCLoadingMemoryFailureHandler@@@@@Z
    // (0x009D66A0), the counted pointers passed by value. Declarations only.
    void SetLoadingMemoryPool(CCountedPointer<CMovableResourceMemoryPool> pool);                        // 0x00B3E820
    void SetLoadingMemoryFailureHandler(CCountedPointer<CLoadingMemoryFailureHandler> handler);        // 0x009D66A0
#endif
#ifdef FABLE_RETAIL_CBANKFILEASYNC_GETASYNCENTRY
    // NF68 opt-in (rows 0x00A42980 / 0x00A25BD0): the PDB name of retail 0x009D58D0 is Ego_r.pdb 0x00B29950 / FableWin.pdb 0x02FF69A0
    // ?GetAsyncEntry@CBankFileAsync@@QAE?AV?$CCountedPointer@VCBankFileAsyncEntry@@@@K@Z (the landed self row 0x009D58D0 spells it so);
    // neither PDB has a CreateEntry (the CREATEENTRY opt-in below). Declaration only.
    CCountedPointer<class CBankFileAsyncEntry> GetAsyncEntry(unsigned long index);   // 0x009D58D0
#endif
#ifdef FABLE_RETAIL_CBANKFILEASYNC_CREATEENTRY
    // agentC27 (row 0x009FDE80): Ego_r.pdb ?CreateEntry@CBankFileAsync@@QAE?AV?$CCountedPointer@VCBankFileAsyncEntry@@@@K@Z, retail 0x009D58D0.
    CCountedPointer<class CBankFileAsyncEntry> CreateEntry(unsigned long index);   // 0x009D58D0
#endif
private:
    CCountedPointer<CThreadedFile> ThreadedFile;
    CCountedPointer<CMovableResourceMemoryPool> LoadingMemoryPool;
    CCountedPointer<CLoadingMemoryFailureHandler> MemoryFailureHandler;
#ifdef FABLE_RETAIL_CBANKFILEASYNC_DATA
    // Retail opt-in (Codex pass 17): retail 0x009D5D30 / RestartReads 0x009D5A90, original STLport pointer sets.
    std::set<CBankFileAsyncData*> AsyncDataInstances;
    std::set<CBankFileAsyncData*> ReadsToRestart;
#else
    unsigned char AsyncDataInstances[12]; // std::set<CBankFileAsyncData*>
    unsigned char ReadsToRestart[12]; // same original set type
#endif
    CBankFileAsyncData* LastRestartingData;
    unsigned char DisposalList[4]; // std::list<CCountedPointer<CBankFileAsyncData> >
    bool NonCached,DisableAsync;
#ifdef FABLE_RETAIL_CBANKFILEASYNC_DATA
    _RTL_CRITICAL_SECTION CriticalSection;
#else
    unsigned char CriticalSection[24]; // SDK _RTL_CRITICAL_SECTION, retained opaque
#endif
};
FABLE_STATIC_ASSERT_AT(54, sizeof(CBankFileAsync)==0x164);
#endif
#endif
