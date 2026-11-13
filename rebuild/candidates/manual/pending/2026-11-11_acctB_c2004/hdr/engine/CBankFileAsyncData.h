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
#if defined(FABLE_RETAIL_CBANKFILEASYNCDATA_PEEKDATA) && !defined(FABLE_RETAIL_C3DANIMATIONINFO_BUILD)
    // Retail opt-in (chunk 2004, row 0x009FED30 CGraphicDataBank::LoadGraphicFrame): Ego_r.pdb / FableWin.pdb
    // ?PeekData@CBankFileAsyncData@@QBEPBXXZ (out of line, 0x009D5580, landed) and the inline ?GetLength@CBankFileAsyncData@@QBEKXZ
    // (Length +0x10 read in place, 0x009FEE1F / 0x009FEFAD). No layout change.
    const void* PeekData() const;                                        // 0x009D5580
    unsigned long GetLength() const { return Length; }
#endif
#ifdef FABLE_RETAIL_C3DANIMATIONINFO_BUILD
// Retail opt-in (Codex pass19): original Ego_r/FableWin types and donor calls; native animation rows 00a24d70/00a26ad0. See NAMING_EVIDENCE.md.
    bool IsFinished()const;
    bool WaitUntilFinished();
    void* GetData();
    unsigned long GetLength()const{return Length;}
#endif
    virtual ~CBankFileAsyncData();
    virtual bool OnFileReadBegin(void**, EThreadedFilePriority);
    virtual void OnFileReadCompletion(bool);
    virtual void RelocateData(long,void*,void*,long);
    virtual bool CanRelocateData(long) const;
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
