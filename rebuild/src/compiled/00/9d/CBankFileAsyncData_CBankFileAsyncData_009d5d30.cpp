// CBankFileAsyncData::CBankFileAsyncData(void* buffer, unsigned long length, CBankFileAsync& bankFile) -- describe a caller-owned read buffer and add this read to its bank under the bank lock.
// Leaf: ??0CBankFileAsyncData@@AAE@PAXKAAVCBankFileAsync@@@Z
// Retail: 0x009D5D30, 179 bytes, thiscall ret 12.
// Retail-volatile: original Finished +0x44, BufferLocked +0x45 and AllowRestart +0x46 are LF_MODIFIER(T_BOOL,flags=2) in Ego_r c09d and FableWin bd67; completion 0x009D54C0/+44, 0x009D54CC/+46 and native write 0x009D5566/+45 write them. Aborted +0x47 is ordinary bool.
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CBANKFILEASYNC_DATA
#define FABLE_RETAIL_CCRITICALSECTIONLOCK
#include "engine/CBankFileAsyncData.h"
#include "engine/CBankFileAsync.h"
#include "engine/CCriticalSectionLock.h"

CBankFileAsyncData::CBankFileAsyncData(void* buffer,unsigned long length,CBankFileAsync& bankFile)
    : Data(buffer), Buffer(buffer), Length(length), SkipBytes(0), ThreadedFileUID(0), MemoryPool(0), MemoryHandle(-1),
      BankFile(bankFile), Finished(false), BufferLocked(false), AllowRestart(false), Aborted(false)
{
    CCriticalSectionLock lock(BankFile.CriticalSection,false);
    BankFile.AsyncDataInstances.insert(this);
    InternalAllocatedBufferSize=0;
}