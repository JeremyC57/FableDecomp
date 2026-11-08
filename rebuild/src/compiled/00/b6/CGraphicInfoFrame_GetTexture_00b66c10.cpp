// 00b66c10 CGraphicInfoFrame::GetTexture
// Leaf: ?GetTexture@CGraphicInfoFrame@@QBE_NPAPBVCTexture@@KK@Z
// Flags:
#include "fable_compiler.h"
#include <new>
#include "rebuild_abi.h"
#define CIVCountedPointeeBase CIVCountedPointeeBaseFields
#include "engine/CIVCountedPointeeBase.h"
#undef CIVCountedPointeeBase
#define CResource CResourceFields
#include "engine/CResource.h"
#undef CResource
#define CGraphicFrame CGraphicFrameFields
#include "engine/CGraphicFrame.h"
#undef CGraphicFrame
// Layout: typed projections of the shared flattened headers. Original Ego_r types
// give the base and members; native loads establish all accessed offsets.
class CIVCountedPointeeBase{private:mutable unsigned long IVRefCount;public:virtual ~CIVCountedPointeeBase();virtual void Delete()const;long Release()const{return --IVRefCount;}};
template<class T>class CIVCountedPointer{private:CIVCountedPointeeBase*Object;public:T*Get()const{return static_cast<T*>(Object);}T*operator->()const{return Get();}bool IsNull()const{return Object==0;}void Release(){if(Object){CIVCountedPointeeBase*p=Object;if(p->Release()==0)p->Delete();Object=0;}}void Reset(T*p){Release();Object=p;}};
class CCPPointerInfo;template<class T>class CCountedPointer{public:T*Data;CCPPointerInfo*Info;T*Get()const{return Data;}};
class CResourceList;
class CResource:public CIVCountedPointeeBase{
 friend class CResourceList;
private:CResourceList*ResourceList;CResource*PrevResource;CResource*NextResource;unsigned long ResourceSize,LastUsedFrame;void ListRemove();void ListInsert(CResource*,CResource*);
public:void TouchResource();
};
class CFailedAllocationHandler{public:virtual~CFailedAllocationHandler();};
class CResourceList:public CFailedAllocationHandler{friend class CResource;private:CResource Head;unsigned long ResourceCount;long AllocatedMemory,MaximumMemory;unsigned long CurrentFrame,DebugStatsFrame,UnloadDelay,UnloadedThisFrame;public:unsigned long GetCurrentFrame()const{return CurrentFrame;}};
FABLE_FORCEINLINE void CResource::TouchResource(){if(ResourceList&&LastUsedFrame!=ResourceList->GetCurrentFrame()){ListRemove();ListInsert(ResourceList->Head.PrevResource,&ResourceList->Head);LastUsedFrame=ResourceList->GetCurrentFrame();}}
class CManagedTextureData:public CResource{};
class CBaseClass{public:virtual~CBaseClass();};
class CManagedTexture:public CBaseClass{private:CIVCountedPointer<CManagedTextureData>Data;public:FABLE_FORCEINLINE void TouchTexture(){if(!Data.IsNull())Data->TouchResource();}};
class CManagedTextureOwner:public CResource{};
class CTexture;
// Layout: accessed prefix of original27-byte CGraphicFrameData; trailing packed
// shorts and bitfields are not read or instantiated by this function.
class CGraphicFrameData{private:unsigned long ReferenceCount;void*Data;CCountedPointer<CTexture>PTexture;public:CTexture*GetTexture()const{return PTexture.Get();}};
class CGraphicFrame{private:CGraphicFrameData*FrameData;public:bool IsInitialised()const{return FrameData!=0;}CTexture*GetTexture()const{return FrameData->GetTexture();}};
class CBaseClassNonCopyable{protected:virtual~CBaseClassNonCopyable();};
class CResourceBank:public CBaseClassNonCopyable{private:CResourceList ResourceList,LoadingQueue;public:unsigned long GetCurrentFrame()const{return ResourceList.GetCurrentFrame();}};
// Layout: flattened unread native CBankFileAsync prefix164; original second
// CResourceBank base164 plus ResourceList4 plus CurrentFrame2c gives194.
class CBankFileAsync{public:virtual~CBankFileAsync();private:unsigned char unobservedBankState[0x160];};
class CGraphicDataBank:public CBankFileAsync,public CResourceBank{};
class CGraphicInfoFrame;
class CGraphicInfo{friend class CGraphicInfoFrame;private:unsigned char unobservedHeaderAndTextureType[0x20];CGraphicDataBank*Bank;long Index;CGraphicInfoFrame*Frames;CGraphicDataBank*GetBank()const{return Bank;}};
class CBankFileAsyncData;
class CGraphicInfoFrame:public CBaseClass{
public:class CFrameData;
private:unsigned short FrameNum;signed char SmallMipmapLevel;mutable CIVCountedPointer<CFrameData>Data;CGraphicInfo*GraphicInfo;
bool InternalGetFrameForcedLoading(unsigned long)const;CFrameData*GetData()const{return Data.Get();}CGraphicDataBank*GetBank()const{return GraphicInfo->GetBank();}
public:bool GetTexture(const CTexture**,unsigned long,unsigned long)const;
};
class CGraphicInfoFrame::CFrameData:public CManagedTextureOwner{
public:CGraphicFrame Frame;CManagedTexture ManagedTexture,LoadingManagedTexture;CCountedPointer<CBankFileAsyncData>FileData;signed char CurrentMipmapLevel,LoadingMipmapLevel,TouchedMipmapLevel;unsigned long TouchedFrame;const CGraphicInfoFrame*GraphicInfoFrame;
CFrameData(const CGraphicInfoFrame*);void BeginLoading(unsigned long,unsigned long);
};
FABLE_STATIC_ASSERT(sizeof(CResource)==sizeof(CResourceFields));
FABLE_STATIC_ASSERT(sizeof(CGraphicFrame)==sizeof(CGraphicFrameFields));
FABLE_STATIC_ASSERT(sizeof(CGraphicInfoFrame::CFrameData)==0x44);

bool CGraphicInfoFrame::GetTexture(const CTexture**texture,unsigned long mipLevel,unsigned long flags)const{
 if(!Data.IsNull()){
  if(Data->Frame.IsInitialised()){
   if(!(flags&8)){
    unsigned long bankFrame=GetBank()->GetCurrentFrame();
    if(Data->TouchedFrame!=bankFrame || mipLevel<=(unsigned long)Data->TouchedMipmapLevel){
     Data->TouchedFrame=bankFrame;
     Data->ManagedTexture.TouchTexture();
     if(mipLevel<=(unsigned long)Data->CurrentMipmapLevel)Data->TouchResource();
     if((unsigned long)Data->TouchedMipmapLevel>mipLevel)Data->TouchedMipmapLevel=(signed char)mipLevel;
    }
   }
   if(mipLevel<(unsigned long)Data->CurrentMipmapLevel){
    Data->BeginLoading(mipLevel,flags);
    if((flags&16)&&Data->FileData.Get())goto load;
    *texture=Data->Frame.GetTexture();return true;
   }
   *texture=Data->Frame.GetTexture();return true;
  }
load:Data->BeginLoading(mipLevel,flags);
 }else{
  if(flags&4)return false;
  Data.Reset(new CFrameData(this));
  Data->BeginLoading(mipLevel,flags);
 }
 if((flags&1)&&InternalGetFrameForcedLoading(flags)){*texture=Data->Frame.GetTexture();return true;}
 return false;
}