// Complete LoadBinary logic with isolated dependency fixtures. Native layout is checked separately.
// Host adaptation: sizeof results in target expressions use unsigned int, preserving VC71's 32-bit size_t.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _MSC_VER
typedef __int64 FableSigned64;
#else
typedef long long FableSigned64;
#endif
static void Check(bool ok){if(!ok){fprintf(stderr,"00a8ad40 test failure\n");exit(1);}}
struct Event {int id;unsigned int a,b;};
static Event events[128];static int event_count;
static void Log(int id,unsigned int a=0,unsigned int b=0){Check(event_count<128);events[event_count].id=id;events[event_count].a=a;events[event_count].b=b;++event_count;}
static unsigned int count_material,count_primitive,count_bone,count_name,static_count,anim_count;
static int allocation_failure,primitive_failure,allocation_calls,primitive_calls,material_calls;
static bool stats_success,cloth,locked_flag;
static unsigned char raw_names[64];
class CMemoryDataInputStream {
    int words,shorts;
public:
    CMemoryDataInputStream():words(0),shorts(0){}
    unsigned int ReadULONG(){unsigned int a[4]={count_material,count_primitive,count_bone,count_name};Check(words<4);Log(2,words,a[words]);return a[words++];}
    bool ReadEBOOL(){Log(3,cloth);return cloth;}
    unsigned short ReadUWORD(){unsigned int v=shorts++?anim_count:static_count;Check(shorts<=2);Log(4,shorts,v);return (unsigned short)v;}
    void Read(void* p,unsigned int n){Log(10,n);if(n)memset(p,0x35,n);}
    void ReadCompressed(void* p,unsigned int n);
};
class CMovableResource {};class CGraphicDataBank {};
static unsigned char primitive_storage[4096];
class CMovableResourceMemoryPool {
public:
    bool AllocateResource(long* out,CMovableResource*,unsigned long n,unsigned long flags){Check(locked_flag&&flags==0);++allocation_calls;Log(5,allocation_calls,(unsigned int)n);if(allocation_calls==allocation_failure)return false;*out=allocation_calls;return true;}
    void* GetPointerFromHandle(long handle){Check(locked_flag);Log(13,(unsigned int)handle);return primitive_storage;}
    void Lock(long handle){Check(locked_flag);Log(14,(unsigned int)handle);}
    void Unlock(long handle){Check(locked_flag);Log(16,(unsigned int)handle);}
};
class C3DMeshMaterial {unsigned char storage[48];public:void LoadBinary(CMemoryDataInputStream&,const CGraphicDataBank*){Check(locked_flag);++material_calls;Log(12,material_calls);}};
class C3DPrimitive2 {unsigned char storage[160];public:bool LoadBinary(CMemoryDataInputStream&,unsigned long n,unsigned char** p,CMovableResourceMemoryPool*){Check(locked_flag&&n==0);++primitive_calls;Log(15,primitive_calls,(unsigned int)(*p-primitive_storage));*p+=16;return primitive_calls!=primitive_failure;}};
struct C3DBone2 {unsigned char storage[60];};struct C3DKeyframe {unsigned char storage[48];};struct CMatrix4x4 {unsigned char storage[64];};struct CMatrix3x4 {unsigned char storage[48];};
class CScopedLockFlagBool {bool& Flag;public:CScopedLockFlagBool(bool& flag):Flag(flag){Flag=true;locked_flag=true;}~CScopedLockFlagBool(){Flag=false;locked_flag=false;}};
class C3DMeshStats:public CMovableResource {
public:CMovableResourceMemoryPool* MemoryPool;
    bool LoadBinary(CMemoryDataInputStream&){Check(locked_flag&&MemoryPool);Log(1);return stats_success;}
};
static C3DMeshMaterial materials[4];static C3DPrimitive2 primitives[4];static C3DBone2 bones[4];static C3DKeyframe keyframes[4];static CMatrix4x4 transforms[4];static unsigned short name_indices[4];static char name_data[64];
class C3DMesh2:public C3DMeshStats {
public:
 long MemoryHandle,PrimMemoryHandle,BoneNameMemoryHandle;
 unsigned int MaterialCount,PrimitiveCount,BoneCount,BoneNameDataSize;
 C3DMeshMaterial* Materials;C3DPrimitive2* Primitives;C3DBone2* Bones;C3DKeyframe* BoneKeyframes;CMatrix4x4* BoneSpaceTransforms;unsigned short* BoneNameIndices;char* BoneNameData;CMatrix3x4 InverseTransform;bool ClothFlag,LockedFromMemoryRelocation;
 bool LoadBinary(CMemoryDataInputStream&,CMovableResourceMemoryPool*,const CGraphicDataBank*,bool);
 void InitialisePointers(unsigned long n,unsigned long size){Log(6,(unsigned int)n,(unsigned int)size);Check(n<=4);Materials=materials;Primitives=primitives;Bones=bones;BoneKeyframes=keyframes;BoneSpaceTransforms=transforms;}
 void InitialiseBoneNamePointers(unsigned long size){Log(7,(unsigned int)size);BoneNameIndices=name_indices;BoneNameData=name_data;}
};
void CMemoryDataInputStream::ReadCompressed(void* p,unsigned int n){Log(11,n);if(p==name_data){Check(n<=64);memcpy(p,raw_names,n);}else if(n)memset(p,0x6a,n);}
#define FIXTURE_SIZEOF(T) ((unsigned int)sizeof(T))
// BEGIN_AUTHORED_BODY
bool C3DMesh2::LoadBinary(CMemoryDataInputStream& is, CMovableResourceMemoryPool* pool, const CGraphicDataBank* texture_bank, bool discard_primitives)
{
    CScopedLockFlagBool lock(LockedFromMemoryRelocation);
    MemoryPool = pool;
    if (!C3DMeshStats::LoadBinary(is)) {
        return false;
    }
    MaterialCount = is.ReadULONG();
    PrimitiveCount = is.ReadULONG();
    BoneCount = is.ReadULONG();
    BoneNameDataSize = is.ReadULONG();
    ClothFlag = is.ReadEBOOL();
    unsigned long static_block_count = is.ReadUWORD();
    unsigned long anim_block_count = is.ReadUWORD();
    if (discard_primitives)
        PrimitiveCount = 0;
    FableSigned64 overflow_check_1 = BoneCount * ((0x3c + 0x30 + 0x40))
                           + MaterialCount * FIXTURE_SIZEOF(C3DMeshMaterial) + PrimitiveCount * FIXTURE_SIZEOF(C3DPrimitive2);
    if (overflow_check_1 < 0 || overflow_check_1 > 0x7fffffff) {
        return false;
    }
    FableSigned64 overflow_check_2 = static_block_count * 0x20 + anim_block_count * 0x30;
    if (overflow_check_2 < 0 || overflow_check_2 > 0x7fffffff) {
        return false;
    }
    FableSigned64 overflow_check_3 = BoneNameDataSize + BoneCount * FIXTURE_SIZEOF(unsigned short);
    if (overflow_check_3 < 0 || overflow_check_3 > 0x7fffffff) {
        return false;
    }
    unsigned long buffer_size = BoneCount * ((0x3c + 0x30 + 0x40))
                           + MaterialCount * FIXTURE_SIZEOF(C3DMeshMaterial) + PrimitiveCount * FIXTURE_SIZEOF(C3DPrimitive2);
    unsigned long prim_buffer_size = static_block_count * 0x20 + anim_block_count * 0x30;
    unsigned long bone_name_buffer_size = BoneNameDataSize + BoneCount * FIXTURE_SIZEOF(unsigned short);
    if (!MemoryPool->AllocateResource(&MemoryHandle, this, buffer_size, 0)) {
        return false;
    }
    if (!MemoryPool->AllocateResource(&PrimMemoryHandle, this, prim_buffer_size, 0)) {
        return false;
    }
    if (bone_name_buffer_size && !MemoryPool->AllocateResource(&BoneNameMemoryHandle, this, bone_name_buffer_size, 0)) {
        return false;
    }
    InitialisePointers(BoneCount, buffer_size);
    InitialiseBoneNamePointers(bone_name_buffer_size);
    is.Read(BoneNameIndices, BoneCount * FIXTURE_SIZEOF(unsigned short));
    is.ReadCompressed(BoneNameData, BoneNameDataSize);
    char* current_bone_name = BoneNameData;
    for (unsigned long c_bone = 0; c_bone < BoneCount; ++c_bone) {
        BoneNameIndices[c_bone] = (unsigned short)(current_bone_name - BoneNameData);
        current_bone_name += strlen(current_bone_name) + 1;
    }
    is.ReadCompressed(Bones, BoneCount * 0x3c);
    is.ReadCompressed(BoneKeyframes, BoneCount * 0x30);
    is.ReadCompressed(BoneSpaceTransforms, BoneCount * 0x40);
    is.Read(&InverseTransform, FIXTURE_SIZEOF(InverseTransform));
    for (unsigned long c_material = 0; c_material < MaterialCount; ++c_material)
        Materials[c_material].LoadBinary(is, texture_bank);
    if (discard_primitives)
        return true;
    unsigned char* prim_dest = (unsigned char*)pool->GetPointerFromHandle(PrimMemoryHandle);
    MemoryPool->Lock(MemoryHandle);
    MemoryPool->Lock(PrimMemoryHandle);
    for (unsigned long c_prim = 0; c_prim < PrimitiveCount; ++c_prim) {
        if (!Primitives[c_prim].LoadBinary(is, 0, &prim_dest, MemoryPool)) {
            MemoryPool->Unlock(PrimMemoryHandle);
            MemoryPool->Unlock(MemoryHandle);
            return false;
        }
    }
    MemoryPool->Unlock(PrimMemoryHandle);
    MemoryPool->Unlock(MemoryHandle);
    return true;
}

// END_AUTHORED_BODY
#undef FIXTURE_SIZEOF
static int cursor;
static void Want(int id,unsigned int a=0,unsigned int b=0){Check(cursor<event_count);Event e=events[cursor++];Check(e.id==id&&e.a==a&&e.b==b);}
static unsigned int tested;
static void Run(unsigned int m,unsigned int p,unsigned int b,bool discard,bool stats,int allocfail,int primfail,bool flag,unsigned int n,unsigned int sc,unsigned int ac) {
 count_material=m;count_primitive=p;count_bone=b;count_name=n;static_count=sc;anim_count=ac;
 stats_success=stats;allocation_failure=allocfail;primitive_failure=primfail;cloth=flag;event_count=0;allocation_calls=primitive_calls=material_calls=0;locked_flag=false;
 C3DMesh2 mesh;memset(&mesh,0,sizeof(mesh));mesh.MemoryHandle=mesh.PrimMemoryHandle=mesh.BoneNameMemoryHandle=-1;
 memset(name_indices,0,sizeof(name_indices));memset(name_data,0,sizeof(name_data));memset(raw_names,0,sizeof(raw_names));
 if(b<=4){unsigned int k=0;for(unsigned int i=0;i<b;++i){for(unsigned int j=0;j<=i;++j)raw_names[k++]='a'+i;raw_names[k++]=0;}Check(n==k||n>=0x80000000U);}
 CMemoryDataInputStream stream;CMovableResourceMemoryPool pool;CGraphicDataBank bank;
 bool result=mesh.LoadBinary(stream,&pool,&bank,discard);
 Check(!locked_flag&&!mesh.LockedFromMemoryRelocation&&mesh.MemoryPool==&pool);cursor=0;Want(1);bool expected=false;
 if(stats){
  Want(2,0,m);Want(2,1,p);Want(2,2,b);Want(2,3,n);Want(3,flag);Want(4,1,sc);Want(4,2,ac);
  unsigned int effective=discard?0:p;Check(mesh.MaterialCount==m&&mesh.PrimitiveCount==effective&&mesh.BoneCount==b&&mesh.BoneNameDataSize==n&&mesh.ClothFlag==flag);
  unsigned int size=b*172U+m*48U+effective*160U, primsize=sc*32U+ac*48U, namesize=n+b*2U;
  if(size<=0x7fffffffU&&primsize<=0x7fffffffU&&namesize<=0x7fffffffU){
   Want(5,1,size);
   if(allocfail!=1){
    Want(5,2,primsize);
    if(allocfail!=2){
     if(namesize)Want(5,3,namesize);
     if(!namesize||allocfail!=3){
      Want(6,b,size);Want(7,namesize);Want(10,b*2);Want(11,n);
      unsigned int k=0;for(unsigned int i=0;i<b;++i){Check(name_indices[i]==k);k+=i+2;}
      Want(11,b*60);Want(11,b*48);Want(11,b*64);Want(10,48);
      for(unsigned int i=0;i<m;++i)Want(12,i+1);
      expected=true;
      if(!discard){
       Want(13,2);Want(14,1);Want(14,2);
       for(unsigned int i=0;i<effective;++i){Want(15,i+1,i*16);if((int)i+1==primfail){expected=false;break;}}
       Want(16,2);Want(16,1);
      }
     }
    }
   }
  }
 }
 Check(result==expected&&cursor==event_count);++tested;
}
int main(){
 Check(sizeof(unsigned int)==4&&sizeof(unsigned short)==2&&sizeof(C3DMeshMaterial)==48&&sizeof(C3DPrimitive2)==160);
 for(unsigned int m=0;m<4;++m)for(unsigned int p=0;p<4;++p)for(unsigned int b=0;b<4;++b)
 for(int discard=0;discard<2;++discard)for(int stats=0;stats<2;++stats)for(int af=0;af<4;++af)for(int pf=0;pf<5;++pf)for(int flag=0;flag<2;++flag){
  unsigned int n=b*(b+3)/2;Run(m,p,b,discard!=0,stats!=0,af,pf,flag!=0,n,(m+p)*3,(b+p)*2);
 }
 // Every size-check failure and wrapped 32-bit intermediate, with allocation stopped before large buffers.
 const unsigned int counts[]={0x7fffffffU,0x80000000U,0xffffffffU,0x10000000U,0x08000001U};
 for(unsigned int i=0;i<5;++i){Run(counts[i],0,0,false,true,1,0,false,0,0,0);Run(0,counts[i],0,false,true,1,0,false,0,65535,65535);Run(0,0,counts[i],false,true,1,0,false,0,0,0);}
 Run(0,0,0,false,true,0,0,false,0x80000000U,0,0);
 Check(tested==10256);puts("00a8ad40_TEST PASS");return 0;
}