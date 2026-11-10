// 009d5df0 CBankFileAsyncData::CBankFileAsyncData
// Leaf: ??0CBankFileAsyncData@@AAE@KKPAVCMovableResourceMemoryPool@@AAVCBankFileAsync@@@Z
// Evidence: Ego_r c097 private four-argument constructor; retail caller 009d616d and field stores agree.
// Original field qualifiers and their two-PDB evidence are in engine/CBankFileAsyncData.h.
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CBANKFILEASYNC_DATA
#define FABLE_RETAIL_CCRITICALSECTIONLOCK
#include "engine/CBankFileAsyncData.h"
#include "engine/CBankFileAsync.h"
#include "engine/CCriticalSectionLock.h"

CBankFileAsyncData::CBankFileAsyncData(unsigned long length,unsigned long skipBytes,CMovableResourceMemoryPool* memoryPool,CBankFileAsync& bankFile)
    : Data(0), Buffer(0), Length(length), SkipBytes(skipBytes), ThreadedFileUID(0), MemoryPool(memoryPool), MemoryHandle(-1),
      BankFile(bankFile), Finished(false), BufferLocked(false), AllowRestart(false), Aborted(false)
{
    CCriticalSectionLock lock(BankFile.CriticalSection,false);
    BankFile.AsyncDataInstances.insert(this);
    InternalAllocatedBufferSize=0;
}