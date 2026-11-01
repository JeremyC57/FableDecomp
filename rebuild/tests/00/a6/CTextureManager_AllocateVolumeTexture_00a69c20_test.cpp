#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
#define CHECK(x) do{if(!(x)){fprintf(stderr,"CHECK line %d\n",__LINE__);abort();}}while(0)
class CBaseClass{public:virtual~CBaseClass(){}};
class CBaseClassNonCopyable:public CBaseClass{protected:CBaseClassNonCopyable(){}virtual~CBaseClassNonCopyable(){}};
class CPixelFormat{};
// Layout: original polymorphic base projects the supplied flattened count layout.
class CIVCountedPointeeBase{public:virtual~CIVCountedPointeeBase();virtual void Delete()const;public:mutable unsigned long IVRefCount;public:void AddRef()const{++IVRefCount;}__forceinline long Release()const{return --IVRefCount;}};
class CResourceList;class CResource:public CIVCountedPointeeBase{private:CResourceList*ResourceList;CResource*PrevResource,*NextResource;unsigned long ResourceSize,LastUsedFrame;};
class CManagedTextureOwner;class CTexture;class CVolumeTexture;

// Layout: original managed-data base and typed used fields restored; unread tail retained physically.
class CManagedTextureData:public CResource{friend class CTextureManager;public:unsigned short Width,Height;unsigned char test_unused_textures[16];CManagedTextureOwner*Owner;private:unsigned char unread[24];public:bool CreateTexture(unsigned long,unsigned long,long,const CPixelFormat&);bool CreateVolumeTexture(unsigned long,unsigned long,unsigned long,long,const CPixelFormat&);};
template<class T>class CIVCountedPointer{public:CIVCountedPointeeBase*Object;public:__forceinline void Release(){CIVCountedPointeeBase*p=Object;if(p){if(p->Release()==0)p->Delete();Object=0;}}__forceinline void Reset(T*data){Release();Object=CastToBase(data);}public:T*Get()const{return static_cast<T*>(Object);}private:CIVCountedPointeeBase*CastToBase(T*data){return data;}};
// Original Data is private; native manager direct access requires manager friendship.
class CManagedTexture:public CBaseClass{friend class CTextureManager;public:CIVCountedPointer<CManagedTextureData>Data;};
class CFailedAllocationHandler{public:virtual~CFailedAllocationHandler();virtual bool HandleFailedAllocation(unsigned long)=0;};
// Layout: resource-list state is unread; retail preallocation pool has12-byte vector storage, unlike donor16. Only its receiver is used.
class CResourceList:public CFailedAllocationHandler{unsigned char unread[56];public:virtual bool HandleFailedAllocation(unsigned long);};class CPreallocTexturePool:private CResourceList{unsigned char unread[24];public:CManagedTextureData*Allocate();};
class CTextureManager:public CBaseClassNonCopyable{public:bool Initialised;unsigned long PoolCount,PreallocSizes[16];CPreallocTexturePool PreallocPools[16];unsigned long FailedAllocations[16];public:static unsigned long __fastcall CalculateTextureSize(unsigned long,unsigned long,bool,const CPixelFormat&);
 bool AllocateTexture(CManagedTexture&,unsigned long,unsigned long,long,const CPixelFormat&,CManagedTextureOwner*);bool AllocateVolumeTexture(CManagedTexture&,unsigned long,unsigned long,unsigned long,long,const CPixelFormat&,CManagedTextureOwner*);
private:long GetPoolIndex(unsigned long);};

bool CTextureManager::AllocateVolumeTexture(CManagedTexture&out,unsigned long width,unsigned long height,unsigned long depth,long mipLevels,const CPixelFormat&format,CManagedTextureOwner*owner){long index=GetPoolIndex(CalculateTextureSize(width,height,false,format)*depth);if(index==-1)return false;CManagedTextureData*texture=PreallocPools[index].Allocate();if(texture&&texture->CreateVolumeTexture(width,height,depth,mipLevels,format)){texture->Owner=owner;out.Data.Reset(texture);out.Data.Get()->AddRef();return true;}++FailedAllocations[index];return false;}

class CManagedTextureOwner{};
CIVCountedPointeeBase::~CIVCountedPointeeBase(){}
CFailedAllocationHandler::~CFailedAllocationHandler(){}
bool CResourceList::HandleFailedAllocation(unsigned long){CHECK(false);return false;}
static CTextureManager*expected_manager;static CManagedTextureData*next_texture;static const CIVCountedPointeeBase*deleted;static const CPixelFormat*expected_format;
static int pool_index,alloc_calls,create_calls,size_calls,index_calls,delete_calls;static bool create_result;static unsigned long expected_w,expected_h,expected_d;static long expected_mip;
void CIVCountedPointeeBase::Delete()const{CHECK(IVRefCount==0);deleted=this;++delete_calls;}
unsigned long __fastcall CTextureManager::CalculateTextureSize(unsigned long w,unsigned long h,bool compressed,const CPixelFormat&f){CHECK(w==expected_w&&h==expected_h&&!compressed&&&f==expected_format);++size_calls;return w*h+16;}
long CTextureManager::GetPoolIndex(unsigned long bytes){CHECK(this==expected_manager&&bytes==(expected_w*expected_h+16)*expected_d);++index_calls;return pool_index;}
CManagedTextureData*CPreallocTexturePool::Allocate(){CHECK(this==&expected_manager->PreallocPools[pool_index]);++alloc_calls;return next_texture;}
bool CManagedTextureData::CreateVolumeTexture(unsigned long w,unsigned long h,unsigned long d,long levels,const CPixelFormat&format){CHECK(this==next_texture&&w==expected_w&&h==expected_h&&d==expected_d&&levels==expected_mip&&&format==expected_format);++create_calls;return create_result;}
#ifdef _MSC_VER
typedef char pool_size[sizeof(CPreallocTexturePool)==84?1:-1];
typedef char data_owner_offset[offsetof(CManagedTextureData,Owner)==0x30?1:-1];
typedef char pools_offset[offsetof(CTextureManager,PreallocPools)==0x4c?1:-1];
typedef char failure_offset[offsetof(CTextureManager,FailedAllocations)==0x58c?1:-1];
#endif
int main(){int cases=0;for(int pi=-1;pi<16;++pi)for(int allocation=0;allocation<2;++allocation)for(int creation=0;creation<2;++creation)for(int old_state=0;old_state<4;++old_state)for(int dimensions=0;dimensions<4;++dimensions){
 CTextureManager manager;CManagedTexture out;CManagedTextureData fresh,old;CManagedTextureOwner owner,prior_owner;CPixelFormat format;
 fresh.IVRefCount=2;old.IVRefCount=(old_state==1)?1:3;fresh.Owner=&prior_owner;old.Owner=&prior_owner;out.Data.Object=(old_state==0)?0:(old_state==3)?static_cast<CIVCountedPointeeBase*>(&fresh):static_cast<CIVCountedPointeeBase*>(&old);
 CIVCountedPointeeBase*previous=out.Data.Object;unsigned long old_count=old.IVRefCount;
 for(int k=0;k<16;++k)manager.FailedAllocations[k]=100+k;
 expected_manager=&manager;next_texture=allocation?&fresh:0;pool_index=pi;create_result=creation!=0;expected_w=dimensions+1;expected_h=dimensions*2+1;expected_d=dimensions;expected_mip=dimensions-1;expected_format=&format;
 alloc_calls=create_calls=size_calls=index_calls=delete_calls=0;deleted=0;
 bool result=manager.AllocateVolumeTexture(out,expected_w,expected_h,expected_d,expected_mip,format,&owner);
 bool success=pi>=0&&allocation&&creation;CHECK(result==success&&size_calls==1&&index_calls==1);CHECK(alloc_calls==(pi>=0?1:0));CHECK(create_calls==(pi>=0&&allocation?1:0));
 for(int k=0;k<16;++k)CHECK(manager.FailedAllocations[k]==100UL+k+((pi==k&&!success)?1:0));
 if(success){CHECK(out.Data.Object==&fresh&&fresh.Owner==&owner);CHECK(fresh.IVRefCount==(old_state==3?2UL:3UL));if(old_state==1||old_state==2){CHECK(old.IVRefCount==old_count-1);CHECK(delete_calls==(old_state==1?1:0));CHECK(deleted==(old_state==1?&old:0));}else CHECK(delete_calls==0);}
 else{CHECK(out.Data.Object==previous&&fresh.Owner==&prior_owner&&fresh.IVRefCount==2&&old.IVRefCount==old_count&&delete_calls==0);}
 ++cases;
 }CHECK(cases==1088);puts("00a69c20_TEST PASS");return 0;}