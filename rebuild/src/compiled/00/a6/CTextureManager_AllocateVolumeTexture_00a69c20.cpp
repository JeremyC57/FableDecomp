// 00a69c20 CTextureManager::AllocateVolumeTexture
// Leaf: ?AllocateVolumeTexture@CTextureManager@@QAE_NAAVCManagedTexture@@KKKJABVCPixelFormat@@PAVCManagedTextureOwner@@@Z
// Flags:
#include "fable_compiler.h"
#include "rebuild_abi.h"
#define CTextureManager CTextureManagerFields
#define CPreallocTexturePool CPreallocTexturePoolFields
#include "engine/CTextureManager.h"
#undef CPreallocTexturePool
#undef CTextureManager
#define CManagedTextureData CManagedTextureDataFields
#include "engine/CManagedTextureData.h"
#undef CManagedTextureData
#define CResource CResourceFields
#include "engine/CResource.h"
#undef CResource
#define CPixelFormat CPixelFormatFields
#include "engine/CPixelFormat.h"
#undef CPixelFormat
class CPixelFormat:public CPixelFormatFields{};
#include "engine/CBaseClass.h"
#include "engine/CBaseClassNonCopyable.h"
#define CIVCountedPointeeBase CIVCountedPointeeBaseFields
#include "engine/CIVCountedPointeeBase.h"
#undef CIVCountedPointeeBase
// Layout: original polymorphic base projects the supplied flattened count layout.
class CIVCountedPointeeBase{public:virtual~CIVCountedPointeeBase();virtual void Delete()const;private:mutable unsigned long IVRefCount;public:void AddRef()const{++IVRefCount;}FABLE_FORCEINLINE long Release()const{return --IVRefCount;}};
class CResourceList;class CResource:public CIVCountedPointeeBase{private:CResourceList*ResourceList;CResource*PrevResource,*NextResource;unsigned long ResourceSize,LastUsedFrame;};
class CManagedTextureOwner;class CTexture;class CVolumeTexture;
#include "engine/CCountedPointer.h"
// Layout: original managed-data base and typed used fields restored; unread tail retained physically.
class CManagedTextureData:public CResource{friend class CTextureManager;private:unsigned short Width,Height;CCountedPointer<CTexture>Texture;CCountedPointer<CVolumeTexture>VolumeTexture;CManagedTextureOwner*Owner;private:unsigned char unread[24];public:bool CreateTexture(unsigned long,unsigned long,long,const CPixelFormat&);bool CreateVolumeTexture(unsigned long,unsigned long,unsigned long,long,const CPixelFormat&);};
template<class T>class CIVCountedPointer{private:CIVCountedPointeeBase*Object;public:FABLE_FORCEINLINE void Release(){CIVCountedPointeeBase*p=Object;if(p){if(p->Release()==0)p->Delete();Object=0;}}FABLE_FORCEINLINE void Reset(T*data){Release();Object=CastToBase(data);}public:T*Get()const{return static_cast<T*>(Object);}private:CIVCountedPointeeBase*CastToBase(T*data){return data;}};
// Original Data is private; native manager direct access requires manager friendship.
class CManagedTexture:public CBaseClass{friend class CTextureManager;private:CIVCountedPointer<CManagedTextureData>Data;};
class CFailedAllocationHandler{public:virtual~CFailedAllocationHandler();virtual bool HandleFailedAllocation(unsigned long)=0;};
// Layout: resource-list state is unread; retail preallocation pool has12-byte vector storage, unlike donor16. Only its receiver is used.
class CResourceList:public CFailedAllocationHandler{unsigned char unread[56];public:virtual bool HandleFailedAllocation(unsigned long);};class CPreallocTexturePool:private CResourceList{unsigned char unread[24];public:CManagedTextureData*Allocate();};
class CTextureManager:public CBaseClassNonCopyable{private:bool Initialised;unsigned long PoolCount,PreallocSizes[16];CPreallocTexturePool PreallocPools[16];unsigned long FailedAllocations[16];public:static unsigned long __fastcall CalculateTextureSize(unsigned long,unsigned long,bool,const CPixelFormat&);
 bool AllocateTexture(CManagedTexture&,unsigned long,unsigned long,long,const CPixelFormat&,CManagedTextureOwner*);bool AllocateVolumeTexture(CManagedTexture&,unsigned long,unsigned long,unsigned long,long,const CPixelFormat&,CManagedTextureOwner*);
private:long GetPoolIndex(unsigned long);};
FABLE_STATIC_ASSERT(sizeof(CManagedTextureData)==sizeof(CManagedTextureDataFields));FABLE_STATIC_ASSERT(sizeof(CResource)==sizeof(CResourceFields));FABLE_STATIC_ASSERT(sizeof(CPreallocTexturePool)==84);
bool CTextureManager::AllocateVolumeTexture(CManagedTexture&out,unsigned long width,unsigned long height,unsigned long depth,long mipLevels,const CPixelFormat&format,CManagedTextureOwner*owner){long index=GetPoolIndex(CalculateTextureSize(width,height,false,format)*depth);if(index==-1)return false;CManagedTextureData*texture=PreallocPools[index].Allocate();if(texture&&texture->CreateVolumeTexture(width,height,depth,mipLevels,format)){texture->Owner=owner;out.Data.Reset(texture);out.Data.Get()->AddRef();return true;}++FailedAllocations[index];return false;}