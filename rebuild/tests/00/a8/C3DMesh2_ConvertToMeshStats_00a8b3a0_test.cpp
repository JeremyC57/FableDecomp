#include <cstdio>
#include <string.h>
static bool alloc_fail=false;
static unsigned long alloc_size=0, alloc_alignment=0;
static int alloc_calls=0, get_calls=0, free_calls=0;
static long frees[8];
static unsigned char old_block[1024], new_block[1024];
static int material_destroys=0, primitive_destroys=0, reloc_calls=0, primitive_reloc_calls=0;
static long reloc_handle=0, reloc_size=0;
static void* reloc_old=0;
static void* reloc_new=0;
static bool reloc_saw_lock=false;
#include <string.h>
#include <string.h>
// C3DMesh2::ConvertToMeshStats() -- destroy mesh arrays and migrate the retained bone block into stats memory. Retail Fable.exe @ 0x00A8B3A0, 276 bytes; calls AllocateResource @ 0x00A62E40, GetPointerFromHandle @ 0x00A026B0, Free @ 0x00A62ED0, and virtual RelocateData slot +4. No C3DMesh2 header exists: layout follows landed C3DMesh2::Sizeof/InitialisePointers and RelocateData. The lock assignment uses an ordinary bool pointer to the named member; this preserves the actual field access while matching retail ordering without volatile.
struct C3DMesh2;
struct CMovableResourceMemoryPool {
    bool AllocateResource(long* handle, C3DMesh2* owner, unsigned long size, unsigned long flags); // 0x00A62E40
    void* GetPointerFromHandle(long handle); // 0x00A026B0
    void Free(long handle);                 // 0x00A62ED0
};
struct C3DMeshStats {
    virtual ~C3DMeshStats();
    CMovableResourceMemoryPool* MemoryPool;
    long StatsMemoryHandle;
    unsigned char _stats[0x70];
    virtual void RelocateData(long,void*,void*,long);
};
struct C3DMeshMaterial { virtual void Destroy(unsigned long flags); unsigned char _body[0x2c]; };
struct C3DPrimitive2 { virtual void Destroy(unsigned long flags); unsigned char _body[0x9c]; virtual void RelocateData(long,void*,void*,long); };
struct C3DBone2 { unsigned char _body[0x3c]; };
struct C3DKeyframe { unsigned char _body[0x30]; };
struct CMatrix4x4 { float m[16]; };
struct C3DMesh2 : C3DMeshStats {
    long MemoryHandle;                 // +0x7c
    long PrimMemoryHandle;             // +0x80
    long BoneNameMemoryHandle;         // +0x84
    unsigned long MaterialCount;       // +0x88
    C3DMeshMaterial* Materials;        // +0x8c
    unsigned long PrimitiveCount;      // +0x90
    C3DPrimitive2* Primitives;         // +0x94
    unsigned long BoneCount;           // +0x98
    C3DBone2* Bones;                   // +0x9c
    C3DKeyframe* BoneKeyframes;        // +0xa0
    unsigned short* BoneNameIndices;   // +0xa4
    char* BoneNameData;                // +0xa8
    unsigned long BoneNameDataSize;    // +0xac
    unsigned char _inverse[0x30];      // +0xb0
    CMatrix4x4* BoneSpaceTransforms;   // +0xe0
    bool ClothFlag;                    // +0xe4
    bool LockedFromMemoryRelocation;   // +0xe5
    unsigned long MeshID;              // +0xe8
    virtual void RelocateData(long,void*,void*,long);
    void ConvertToMeshStats();
};
void C3DMesh2::ConvertToMeshStats()
{
    bool* relocationLock = &LockedFromMemoryRelocation;
    *relocationLock=true;
    C3DMeshMaterial* material=Materials;
    unsigned long materialCount=MaterialCount;
    if (material) {
        if (materialCount > 0) do { material->Destroy(0); material+=1; } while (--materialCount);
    }
    C3DPrimitive2* primitive=Primitives;
    unsigned long primitiveCount=PrimitiveCount;
    if (primitive) {
        if (primitiveCount > 0) do { primitive->Destroy(0); primitive+=1; } while (--primitiveCount);
    }
    Materials=0;
    MaterialCount=0;
    Primitives=0;
    PrimitiveCount=0;
    unsigned long bytes=BoneCount*0xac;
    long newHandle;
    if (MemoryPool->AllocateResource(&newHandle,this,bytes,2)) {
        char* oldBuffer=(char*)MemoryPool->GetPointerFromHandle(MemoryHandle);
        char* newBuffer=(char*)MemoryPool->GetPointerFromHandle(newHandle);
        memcpy(newBuffer,oldBuffer,bytes);
        RelocateData(MemoryHandle,oldBuffer,newBuffer,bytes);
        MemoryPool->Free(MemoryHandle);
        MemoryHandle=newHandle;
    }
    if (PrimMemoryHandle!=-1) {
        MemoryPool->Free(PrimMemoryHandle);
        PrimMemoryHandle=-1;
    }
    *relocationLock=false;
}

static C3DMesh2* active_mesh;
static CMovableResourceMemoryPool* active_pool;
static C3DMeshMaterial* expected_materials;
static C3DPrimitive2* expected_primitives;
static bool bad_callback;
static int events[32], event_count;
static void event(int value) { if(event_count<32) events[event_count++]=value; else bad_callback=true;
 if(!active_mesh->LockedFromMemoryRelocation) bad_callback=true; }
C3DMeshStats::~C3DMeshStats() {}
void C3DMeshStats::RelocateData(long,void*,void*,long) {}
void C3DMeshMaterial::Destroy(unsigned long flags) { event(1); if(flags || this!=expected_materials+material_destroys) bad_callback=true; ++material_destroys; }
void C3DPrimitive2::Destroy(unsigned long flags) { event(2); if(flags || this!=expected_primitives+primitive_destroys) bad_callback=true; ++primitive_destroys; }
void C3DPrimitive2::RelocateData(long,void*,void*,long) { ++primitive_reloc_calls; }
void C3DMesh2::RelocateData(long h,void* oldp,void* newp,long n)
{
    event(6); if(this!=active_mesh || memcmp(oldp,newp,n)!=0) bad_callback=true;
    ++reloc_calls; reloc_handle=h; reloc_old=oldp; reloc_new=newp; reloc_size=n;
    reloc_saw_lock=LockedFromMemoryRelocation;
}
bool CMovableResourceMemoryPool::AllocateResource(long* h,C3DMesh2* owner,unsigned long n,unsigned long flags)
{
    event(3); if(this!=active_pool || owner!=active_mesh || owner->Materials || owner->MaterialCount || owner->Primitives || owner->PrimitiveCount) bad_callback=true;
    ++alloc_calls; alloc_size=n; alloc_alignment=flags;
    if (alloc_fail) return false;
    *h=2; return true;
}
void* CMovableResourceMemoryPool::GetPointerFromHandle(long h)
{
    event(h==1?4:5); if(this!=active_pool) bad_callback=true;
    ++get_calls;
    if (h==1) return old_block;
    if (h==2) return new_block;
    return 0;
}
void CMovableResourceMemoryPool::Free(long h) { event(7); if(this!=active_pool || free_calls>=8) { bad_callback=true; return; } frees[free_calls++]=h; }
static int fail(const char* label) { std::printf("00a8b3a0_TEST FAIL: %s\n",label); return 1; }
int main()
{
    CMovableResourceMemoryPool pool;
    C3DMesh2 mesh;
    C3DMeshMaterial materials[2];
    C3DPrimitive2 primitives[2];
    for (unsigned long i=0;i<sizeof(old_block);++i) old_block[i]=(unsigned char)(i*17u+3u);
    memset(new_block,0,sizeof(new_block));
    mesh.MemoryPool=&pool; mesh.StatsMemoryHandle=77;
    mesh.MemoryHandle=1; mesh.PrimMemoryHandle=3; mesh.BoneNameMemoryHandle=-1;
    mesh.MaterialCount=2; mesh.Materials=materials;
    mesh.PrimitiveCount=2; mesh.Primitives=primitives;
    mesh.BoneCount=2; mesh.Bones=0; mesh.BoneKeyframes=0;
    mesh.BoneNameIndices=0; mesh.BoneNameData=0; mesh.BoneNameDataSize=0;
    mesh.BoneSpaceTransforms=0; mesh.ClothFlag=false; mesh.LockedFromMemoryRelocation=false; mesh.MeshID=0;
    active_mesh=&mesh; active_pool=&pool; expected_materials=materials; expected_primitives=primitives;
    mesh.ConvertToMeshStats();
    if (material_destroys!=2) return fail("material destruction");
    if (primitive_destroys!=2) return fail("primitive destruction");
    if (mesh.Materials || mesh.MaterialCount || mesh.Primitives || mesh.PrimitiveCount) return fail("array reset");
    if (alloc_calls!=1 || alloc_size!=2u*0xacu || alloc_alignment!=2) return fail("allocation request");
    if (mesh.MemoryHandle!=2 || mesh.PrimMemoryHandle!=-1 || mesh.LockedFromMemoryRelocation) return fail("handle/lock state");
    if (reloc_calls!=1 || !reloc_saw_lock || reloc_handle!=1 || reloc_old!=old_block || reloc_new!=new_block || reloc_size!=2u*0xacu)
        return fail("relocation dispatch");
    if (memcmp(old_block,new_block,2u*0xacu)!=0) return fail("retained block copy");
    if (free_calls!=2 || frees[0]!=1 || frees[1]!=3) return fail("old handle release order");
    if (get_calls!=2) return fail("pointer lookup count");
    const int expected_events[]={1,1,2,2,3,4,5,6,7,7};
    if(bad_callback || event_count!=10) return fail("callback lock/receiver");
    for(int j=0;j<10;++j) if(events[j]!=expected_events[j]) return fail("callback sequence");

    C3DMesh2 failed_mesh;
    failed_mesh.MemoryPool=&pool; failed_mesh.StatsMemoryHandle=0;
    failed_mesh.MemoryHandle=4; failed_mesh.PrimMemoryHandle=5; failed_mesh.BoneNameMemoryHandle=-1;
    failed_mesh.MaterialCount=0; failed_mesh.Materials=0; failed_mesh.PrimitiveCount=0; failed_mesh.Primitives=0;
    failed_mesh.BoneCount=1; failed_mesh.Bones=0; failed_mesh.BoneKeyframes=0;
    failed_mesh.BoneNameIndices=0; failed_mesh.BoneNameData=0; failed_mesh.BoneNameDataSize=0;
    failed_mesh.BoneSpaceTransforms=0; failed_mesh.ClothFlag=false; failed_mesh.LockedFromMemoryRelocation=false; failed_mesh.MeshID=0;
    alloc_fail=true; alloc_calls=0; get_calls=0; free_calls=0; reloc_calls=0;
    active_mesh=&failed_mesh; event_count=0; bad_callback=false;
    failed_mesh.ConvertToMeshStats();
    if (failed_mesh.MemoryHandle!=4 || failed_mesh.PrimMemoryHandle!=-1 || failed_mesh.LockedFromMemoryRelocation)
        return fail("failed allocation state");
    if (alloc_calls!=1 || get_calls || reloc_calls || free_calls!=1 || frees[0]!=5)
        return fail("failed allocation cleanup");
    if(bad_callback || event_count!=2 || events[0]!=3 || events[1]!=7) return fail("failed allocation callbacks");
    // No primitive block: the failure path must preserve the main handle and skip Free.
    failed_mesh.PrimMemoryHandle=-1; alloc_calls=get_calls=free_calls=reloc_calls=event_count=0;
    failed_mesh.ConvertToMeshStats();
    if(bad_callback || alloc_calls!=1 || get_calls || free_calls || reloc_calls || event_count!=1 || failed_mesh.MemoryHandle!=4 || failed_mesh.LockedFromMemoryRelocation) return fail("absent primitive handle");
    std::printf("00a8b3a0_TEST PASS\n");
    return 0;
}