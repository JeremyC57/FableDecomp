// CLipSyncDataBankEntry::BeginLoading(unsigned long,CLipSyncDataBank&). Leaf: BeginLoading
#include "engine/CLipSyncDataBank.h"
#include "engine/CBankFile.h"
#include "engine/CResource.h"
#include "engine/CResourceList.h"
#include "engine/CActiveFile.h"
#include "engine/CIVCountedPointeeBase.h"
struct CountInfo {unsigned long RefCount;void(__fastcall*DeleteFunc)(void*);void* Data;};
template<class T>struct Counted {
 T* Data;CountInfo* Info;
 Counted():Data(0),Info(0){}
 Counted(const Counted& b):Data(b.Data),Info(b.Info){if(Info)++Info->RefCount;}
 Counted& operator=(const Counted&); //FileData specialization009A4440
 ~Counted(){if(Info){--Info->RefCount;if(Info->RefCount==0){Info->DeleteFunc(Info->Data);delete Info;}}}
};
struct ResourceNative;
struct IVNative {
 virtual ~IVNative(); virtual void Delete()const; //42D930
 unsigned long IVRefCount;
 long Release()const {unsigned long r=--const_cast<IVNative*>(this)->IVRefCount;if(!r)Delete();return (long)r;}
 void ReleaseReference()const {if(--const_cast<IVNative*>(this)->IVRefCount==0)Delete();}
 long GetRefCount()const{return (long)IVRefCount;}
};
// Native VT12354A4, derived VT129D3A4. Hierarchy and all ten genuine slots.
struct ResourceNative:IVNative {
 ResourceNative(); //9FBEC0 PDBB6BD70
 virtual ~ResourceNative();
 virtual bool UnloadResource(); //slot2
 virtual unsigned long GetUnloadableMemory()const; //slot3 9FBEF0
 virtual bool IsResourceLoaded()const; //slot4
 virtual void OnResourceLoaded(); //slot5
 virtual bool CanAbortResourceLoad(long)const; //slot6 42D970
 virtual void OnResourceLoadAborted(); //slot7 42D980
 virtual unsigned long GetDebugMemoryInUse()const; //slot8 9FBF00
 virtual void ResetDebugMemoryInUse(); //slot9 9FBF10
 CResourceList* ResourceList;ResourceNative* PrevResource;ResourceNative* NextResource;unsigned long ResourceSize,LastUsedFrame;
};
struct AsyncData;struct LipSyncSampleData;
struct AsyncEntry {Counted<AsyncData> BeginReadDataAutoAlloc(unsigned long,unsigned long,long);}; //9D6100 enum EThreadedFilePriority ABI
struct ResourceBankNative {void AddResource(ResourceNative&);}; //9FC3D0 PDBB6C2B0
struct BankNative {
 unsigned char BaseAndEntries[0x170];ResourceBankNative* ResourceBank;CountInfo* ResourceBankInfo;
 Counted<AsyncEntry> CreateEntry(unsigned long); //9D58D0 descriptive identity: constructs CBankFileAsyncEntry, not propagated WorldMap label
 unsigned long GetEntryDataSize(unsigned long)const; //9CC240 PDBB19F40
};
extern long LipSyncLoadingPriority; //01397700 enum EThreadedFilePriority
struct LipSyncEntry {
 struct DataResource:ResourceNative {
  Counted<LipSyncSampleData> LipSyncData;Counted<AsyncData> FileData;LipSyncEntry* Owner;BankNative* Bank;
  DataResource(LipSyncEntry& owner,BankNative& bank):ResourceNative(),Owner(&owner),Bank(&bank){} //PDB03110030, nativeA43580
  virtual ~DataResource();virtual bool UnloadResource();virtual bool IsResourceLoaded()const;virtual void OnResourceLoaded();
 };
 DataResource* Data;float Duration;
 void BeginLoading(unsigned long index,BankNative& bank);
};
void LipSyncEntry::BeginLoading(unsigned long index,BankNative& bank){
 if(!Data){
  DataResource* fresh=new DataResource(*this,bank);
  if(Data){Data->ReleaseReference();Data=0;}
  Data=fresh;
  Counted<AsyncEntry> entry=bank.CreateEntry(index);
  unsigned long size=bank.GetEntryDataSize(index);
  DataResource* resource=Data;
  resource->FileData=entry.Data->BeginReadDataAutoAlloc(0,size,LipSyncLoadingPriority);
  bank.ResourceBank->AddResource(*Data);
 }
}
FABLE_STATIC_ASSERT(sizeof(ResourceNative)==0x1c);
FABLE_STATIC_ASSERT(sizeof(LipSyncEntry::DataResource)==0x34);
FABLE_STATIC_ASSERT(offsetof(BankNative,ResourceBank)==0x170);
