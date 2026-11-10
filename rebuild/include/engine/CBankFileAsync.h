#ifndef FABLE_ENGINE_CBANKFILEASYNC_H
#define FABLE_ENGINE_CBANKFILEASYNC_H
#if defined(FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS) || defined(FABLE_RETAIL_CBANKFILEASYNC_DATA)
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
#ifdef FABLE_RETAIL_CBANKFILEASYNC_DATA
    friend class CBankFileAsyncData;
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
FABLE_STATIC_ASSERT(sizeof(CBankFileAsync)==0x164);
#endif
#endif
