#ifndef FABLE_ENGINE_CBANKFILEASYNCDATA_H
#define FABLE_ENGINE_CBANKFILEASYNCDATA_H
#include "engine/CThreadedFileCallback.h"
#include "engine/CMovableResource.h"
#include "engine/CThreadedFileObjectDesc.h"
class CBankFileAsync;
class CMovableResourceMemoryPool;
// Retail opt-in (Codex pass 17): Ego_r complete type c0a1 is 0x48; FableWin bd6b has the same core plus debug-only trailing fields.
// Both give CThreadedFileCallback@0, CMovableResource@4, and BankFile as a reference.
class CBankFileAsyncData : public CThreadedFileCallback, public CMovableResource {
public:
#ifdef FABLE_RETAIL_C3DANIMATIONINFO_BUILD
// Retail opt-in (Codex pass19): original Ego_r/FableWin types and donor calls; native animation rows 00a24d70/00a26ad0. See NAMING_EVIDENCE.md.
    bool IsFinished()const;
    bool WaitUntilFinished();
    void* GetData();
    unsigned long GetLength()const{return Length;}
#endif
    virtual ~CBankFileAsyncData();
#ifdef FABLE_RETAIL_CBANKFILEASYNCDATA_SIZEOF
    // wf22 (row 0x00A24520): Ego_r.pdb ?Sizeof@CBankFileAsyncData@@QBEKXZ (retail 0x009D5620, landed). Declaration only.
    unsigned long Sizeof() const;                                        // 0x009D5620
#endif
#ifdef FABLE_RETAIL_CBANKFILEASYNCDATA_ABORT
    // chunkQ29 (2026-11-12): Ego_r.pdb ?Abort@CBankFileAsyncData@@QAE_NXZ (retail 0x009D6350). Declaration only.
    bool Abort();                                                        // 0x009D6350
#endif
#ifdef FABLE_RETAIL_CBANKFILEASYNCDATA_CHANGELOADINGPRIORITY
    // Retail opt-in (chunk F2, row 0x00BE06B0 CEngineStaticMapFileBlockManager::AddRequest): Ego_r.pdb
    // ?ChangeLoadingPriority@CBankFileAsyncData@@QAEXW4EThreadedFilePriority@@@Z (retail 0x009D5630, landed). Declaration only.
    void ChangeLoadingPriority(EThreadedFilePriority priority);          // 0x009D5630
#endif
    virtual bool OnFileReadBegin(void**, EThreadedFilePriority);
    virtual void OnFileReadCompletion(bool);
    virtual void RelocateData(long,void*,void*,long);
    virtual bool CanRelocateData(long) const;
#ifdef FABLE_RETAIL_CBANKFILEASYNCDATA_FREEBUFFER
    // Retail opt-in (chunk F2, row 0x009D5BF0 CBankFileAsync::HandleFailedAllocation, which resets a finished read and expands
    // FreeBuffer): Ego_r.pdb ?FreeBuffer@CBankFileAsyncData@@AAEXXZ (private; retail's out-of-line copy is 0x009D5410, landed). The
    // bank reads Finished / AllowRestart and calls the private FreeBuffer, so it is a friend (as CBankFileAsync befriends the data).
    friend class CBankFileAsync;
private:
    void FreeBuffer();                                                   // 0x009D5410
#endif
private:
    CBankFileAsyncData(void*,unsigned long,CBankFileAsync&);
    CBankFileAsyncData(unsigned long,unsigned long,CMovableResourceMemoryPool*,CBankFileAsync&);
    void* Data;
    void* Buffer;
    unsigned long Length;
    unsigned long SkipBytes;
    unsigned long ThreadedFileUID;
    CMovableResourceMemoryPool* MemoryPool;
    long MemoryHandle;
    unsigned long InternalAllocatedBufferSize;
    CBankFileAsync& BankFile;
    CThreadedFileObjectDesc LoadDesc;
    // Retail-volatile: Ego_r c09d / FableWin bd67 are LF_MODIFIER(T_BOOL, flags=2); +44/+45/+46 only. Completion 0x009D54C0 writes Finished and 0x009D54CC writes AllowRestart; native write 0x009D5566 writes BufferLocked.
    volatile bool Finished;
    volatile bool BufferLocked;
    volatile bool AllowRestart;
    bool Aborted;
};
FABLE_STATIC_ASSERT(sizeof(CBankFileAsyncData)==0x48);
#endif
