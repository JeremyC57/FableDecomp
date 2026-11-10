#ifndef FABLE_ENGINE_CBANKFILEASYNC_H
#define FABLE_ENGINE_CBANKFILEASYNC_H
#ifdef FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#include "engine/CBankFile.h"
#include "engine/CCountedPointer.h"
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
private:
    CCountedPointer<CThreadedFile> ThreadedFile;
    CCountedPointer<CMovableResourceMemoryPool> LoadingMemoryPool;
    CCountedPointer<CLoadingMemoryFailureHandler> MemoryFailureHandler;
    unsigned char AsyncDataInstances[12]; // std::set<CBankFileAsyncData*>
    unsigned char ReadsToRestart[12]; // same original set type
    CBankFileAsyncData* LastRestartingData;
    unsigned char DisposalList[4]; // std::list<CCountedPointer<CBankFileAsyncData> >
    bool NonCached,DisableAsync;
    unsigned char CriticalSection[24]; // SDK _RTL_CRITICAL_SECTION, retained opaque
};
FABLE_STATIC_ASSERT(sizeof(CBankFileAsync)==0x164);
#endif
#endif
