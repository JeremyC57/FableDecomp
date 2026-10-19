// NGameText::CDataBankGroupEntry::BeginLoading -- create asynchronous group resource.
// Retail009C80F0,296 bytes. Native resource bank164; resource lists168/1A4; PMF is not used by CCPPointerInfo.
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

#include "fable_string.h"
#include <new>
struct NativePointerInfo {
 unsigned long Count;void (__fastcall *Destroy)(void*);void* Object; // actual CCPPointerInfo; native callback9A42B0
};
#if defined(_MSC_VER)
FABLE_STATIC_ASSERT(sizeof(NativePointerInfo)==12);
#endif
template<class T>struct CountedPointer {
 T* Data;NativePointerInfo* Info;
 CountedPointer():Data(0),Info(0){}
 CountedPointer(const CountedPointer& p):Data(p.Data),Info(p.Info){if(Info)++Info->Count;}
 __forceinline ~CountedPointer(){if(Info){--Info->Count;if(Info->Count==0){Info->Destroy(Info->Object);operator delete(Info);}}Data=0;Info=0;}
 T* operator->()const{return Data;}
};
template<class T>struct OwnedIVPointer {
 T* Object;OwnedIVPointer():Object(0){}
 __forceinline ~OwnedIVPointer(){Reset();}
 __forceinline void Reset(){T* old=Object;if(old){if(!--old->IVRefCount)old->Delete();Object=0;}}
 __forceinline void Adopt(T* value){Reset();Object=value;}
 T* operator->()const{return Object;}
};
struct CBankFileAsyncData {};
struct CBankFileAsyncEntry {CountedPointer<CBankFileAsyncData> BeginReadDataAutoAlloc(unsigned long,unsigned long,long);}; //009D6100
extern long gDataBankReadPriority; //01396314 used as EThreadedFilePriority
struct CResourceBank {void* VTable;struct CResourceList ResourceList,LoadingQueue;void AddResource(CResource&);}; //009FC380
namespace NGameText {
struct CDataBank;struct CDataBankGroupEntry;
struct CGroupEntry:CResource {
 virtual bool UnloadResource();
 CArrayNative<long> Entries;CountedPointer<CBankFileAsyncData> PFileData;const CDataBankGroupEntry* Owner;
 CGroupEntry(CountedPointer<CBankFileAsyncData>&,const CDataBankGroupEntry*); //009C8060 native34 donor38, real array12 vs16
};
struct CDataBankGroupEntry {OwnedIVPointer<CGroupEntry> PGroup;void BeginLoading(unsigned long,CDataBank*);};
struct CDataBank {
 unsigned char BankAsyncState[0x164];struct CResourceBank ResourceBank;
 CountedPointer<CBankFileAsyncEntry> GetAsyncEntry(unsigned long); //009D58D0
 unsigned long GetEntryDataSize(unsigned long)const; //009CC240
};
void CDataBankGroupEntry::BeginLoading(unsigned long index,CDataBank* bank){
 if(!PGroup.Object){
  CountedPointer<CBankFileAsyncEntry> entry=bank->GetAsyncEntry(index);
  CountedPointer<CBankFileAsyncData> data=entry->BeginReadDataAutoAlloc(0,bank->GetEntryDataSize(index),gDataBankReadPriority);
  PGroup.Adopt(new CGroupEntry(data,this));
  bank->ResourceBank.AddResource(*PGroup.Object);
  bank->ResourceBank.ResourceList.MakeFreeSpace(0);
 }
 PGroup->Touch();
}
}