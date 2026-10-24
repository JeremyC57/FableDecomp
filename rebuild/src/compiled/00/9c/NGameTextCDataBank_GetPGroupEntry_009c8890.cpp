// NGameText::CDataBank::GetPGroupEntry -- load/materialise a group and retain it.
// Leaf: GetPGroupEntry
// Retail009C8890,247 bytes. Native Entries1E0 vs donor254; two native3C resource lists at168/1A4.
#define CDataBank CDataBankLegacy
#define CIVCountedPointer CIVCountedPointerLegacy
#include "fable_game_text_bank.h"
#undef CIVCountedPointer
#undef CDataBank
#define CResource CResourcePdb
#define CResourceList CResourceListPdb
#include "engine/CResource.h"
#include "engine/CResourceList.h"
#undef CResourceList
#undef CResource
#include "engine/CIVCountedPointeeBase.h"
struct CResourceList;
struct CResource {
 virtual ~CResource();virtual void Delete()const;
 virtual bool UnloadResource();virtual unsigned long GetUnloadableMemory()const;
 virtual bool IsResourceLoaded()const;virtual void OnResourceLoaded();
 virtual bool CanAbortResourceLoad(long)const;virtual void OnResourceLoadAborted();
 virtual unsigned long GetDebugMemoryInUse()const;virtual void ResetDebugMemoryInUse();
 unsigned long IVRefCount;struct CResourceList* ResourceList;CResource* PrevResource;CResource* NextResource;unsigned long ResourceSize,LastUsedFrame;
 void ListRemove(); //009FBFB0
 void ListInsert(CResource*,CResource*); //009FBF90
 void Touch();
};
struct CResourceList {
 void* VTable;CResource Head;unsigned long ResourceCount;long AllocatedMemory,MaximumMemory;unsigned long CurrentFrame,DebugStatsFrame,UnloadDelay,UnloadedThisFrame;
 bool MakeFreeSpace(unsigned long); //009FC4F0
};
__forceinline void CResource::Touch(){if(ResourceList&&LastUsedFrame!=ResourceList->CurrentFrame){ListRemove();ListInsert(ResourceList->Head.PrevResource,&ResourceList->Head);LastUsedFrame=ResourceList->CurrentFrame;}}
template<class T>struct CArrayNative {T* First;T* Last;T* End;T& operator[](unsigned long n)const{return First[n];}};
template<class T>struct CIVCountedPointer {
 T* Object;
 CIVCountedPointer(T* p):Object(p){if(Object)++const_cast<CResource*>(static_cast<const CResource*>(Object))->IVRefCount;}
 CIVCountedPointer(const CIVCountedPointer& p):Object(p.Object){if(Object)++const_cast<CResource*>(static_cast<const CResource*>(Object))->IVRefCount;}
 ~CIVCountedPointer(){if(Object&&!--const_cast<CResource*>(static_cast<const CResource*>(Object))->IVRefCount)Object->Delete();}
};
struct CBankFileAsyncData {bool IsFinished();}; //009D54E0
namespace NGameText {
struct CDataBank;struct CDataBankGroupEntry;
struct CGroupEntry:CResource {virtual bool UnloadResource();CArrayNative<long> Entries;CBankFileAsyncData* PFileData;void* FileDataInfo;CDataBankGroupEntry* Owner;};
struct CDataBankGroupEntry {CGroupEntry* PGroup;void BeginLoading(unsigned long,CDataBank*);/*009C80F0*/void BuildGroup();/*009C8700*/};
struct CDataBank {
 unsigned char BankAsyncState[0x168];CResourceList ResourceCache,OtherResourceCache;CArrayNative<CDataBankGroupEntry*> Entries;
 unsigned long GetEntryDataType(unsigned long)const; //009CC210
 void FlushAsyncUpdates()const; //009D68B0: invokes009D6730 then009D5A90
 void QueueGroupLoad(unsigned long)const; //009C8220 out-of-line BeginLoadingGroup; manifest text label incorrect
 CIVCountedPointer<const CGroupEntry> GetPGroupEntry(unsigned long,bool)const;
 __forceinline void BeginLoadingGroup(unsigned long n)const{if(GetEntryDataType(n)==1&&Entries[n])Entries[n]->BeginLoading(n,const_cast<CDataBank*>(this));}
};
CIVCountedPointer<const CGroupEntry> CDataBank::GetPGroupEntry(unsigned long n,bool allowPending)const {
 if(GetEntryDataType(n)==1){
 CDataBankGroupEntry* entry=Entries[n];
 if(entry){
 FlushAsyncUpdates();
 if(!(entry->PGroup&&(!entry->PGroup->PFileData||entry->PGroup->PFileData->IsFinished()))&&!allowPending){QueueGroupLoad(n);return CIVCountedPointer<const CGroupEntry>(0);}
 if(!entry->PGroup){BeginLoadingGroup(n);entry->BuildGroup();const_cast<CResourceList&>(ResourceCache).MakeFreeSpace(0);}
 else if(entry->PGroup->PFileData)entry->BuildGroup();
 entry->PGroup->Touch();
 return CIVCountedPointer<const CGroupEntry>(entry->PGroup);
 }}
 return CIVCountedPointer<const CGroupEntry>(0);
}
}