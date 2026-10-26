#include <cstdio>
static int base_calls=0, primitive_calls=0;
static long got_handle=0, got_size=0;
static void* got_old=0;
static void* got_new=0;
// C3DMesh2::RelocateData(long handle, void* oldBase, void* newBase, long size) -- rebase mesh blocks or dispatch primitive relocation. Retail Fable.exe @ 0x00A8B4C0, 304 bytes; base fallback C3DMeshStats::RelocateData @ 0x00A2E820 and virtual primitive RelocateData slot +4. No C3DMesh2 header exists: member offsets/element sizes follow landed C3DMesh2::Sizeof and InitialisePointers sources; C3DPrimitive2 slot and size follow landed C3DPrimitive2::RelocateData.
class C3DMeshStats { public:
    virtual void v00();
    void* MemoryPool;            // +0x04
    long StatsMemoryHandle;       // +0x08
    unsigned char _base[0x70];    // through +0x7b
    virtual void RelocateData(long,void*,void*,long); // 0x00A2E820
};
class C3DMeshMaterial { public: unsigned char _body[0x30]; };
class C3DBone2 { public: unsigned char _body[0x3c]; };
class C3DKeyframe { public: unsigned char _body[0x30]; };
class CMatrix4x4 { public: float m[16]; };
class C3DPrimitive2 { public:
    virtual void v00();
    unsigned char _body[0x9c];
    virtual void RelocateData(long,void*,void*,long);
};
class C3DMesh2 : public C3DMeshStats { public:
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
    unsigned char _inverse[0x30];      // +0xb0 .. +0xdf
    CMatrix4x4* BoneSpaceTransforms;   // +0xe0
    virtual void RelocateData(long,void*,void*,long);
};
void C3DMesh2::RelocateData(long handle, void* oldBase, void* newBase, long size)
{
    if (handle == MemoryHandle) {
        if (Materials) Materials=(C3DMeshMaterial*)((char*)Materials-(char*)oldBase+(char*)newBase);
        if (Primitives) Primitives=(C3DPrimitive2*)((char*)Primitives-(char*)oldBase+(char*)newBase);
        if (Bones) Bones=(C3DBone2*)((char*)Bones-(char*)oldBase+(char*)newBase);
        if (BoneKeyframes) BoneKeyframes=(C3DKeyframe*)((char*)BoneKeyframes-(char*)oldBase+(char*)newBase);
        if (BoneSpaceTransforms) BoneSpaceTransforms=(CMatrix4x4*)((char*)BoneSpaceTransforms-(char*)oldBase+(char*)newBase);
    } else if (handle == PrimMemoryHandle) {
        for (unsigned long i=0; i<PrimitiveCount; ++i)
            Primitives[i].RelocateData(handle,oldBase,newBase,size);
    } else if (handle == BoneNameMemoryHandle) {
        if (BoneNameIndices) BoneNameIndices=(unsigned short*)((char*)BoneNameIndices-(char*)oldBase+(char*)newBase);
        if (BoneNameData) BoneNameData=(char*)BoneNameData-(char*)oldBase+(char*)newBase;
    } else {
        C3DMeshStats::RelocateData(handle,oldBase,newBase,size);
    }
}
static C3DPrimitive2* receivers[4];
static C3DMeshStats* base_receiver;
void C3DMeshStats::v00() {}
void C3DPrimitive2::v00() {}
void C3DMeshStats::RelocateData(long h,void* oldp,void* newp,long s)
{
    base_receiver=this; ++base_calls; got_handle=h; got_old=oldp; got_new=newp; got_size=s;
}
void C3DPrimitive2::RelocateData(long h,void* oldp,void* newp,long s)
{
    if(primitive_calls<4) receivers[primitive_calls]=this; ++primitive_calls; got_handle=h; got_old=oldp; got_new=newp; got_size=s;
}
static int fail(const char* label) { std::printf("00a8b4c0_TEST FAIL: %s\n",label); return 1; }
int main()
{
    unsigned char arena[2048];
    char* oldp=(char*)arena+64;
    char* newp=(char*)arena+512;
    C3DMesh2 mesh;
    mesh.MemoryHandle=10; mesh.PrimMemoryHandle=20; mesh.BoneNameMemoryHandle=30;
    mesh.MaterialCount=0; mesh.PrimitiveCount=0; mesh.BoneCount=0;
    mesh.Materials=(C3DMeshMaterial*)(oldp+16);
    mesh.Primitives=(C3DPrimitive2*)(oldp+32);
    mesh.Bones=(C3DBone2*)(oldp+48);
    mesh.BoneKeyframes=(C3DKeyframe*)(oldp+64);
    mesh.BoneSpaceTransforms=(CMatrix4x4*)(oldp+80);
    mesh.BoneNameIndices=0; mesh.BoneNameData=0;
    mesh.RelocateData(10,oldp,newp,123);
    if ((char*)mesh.Materials!=newp+16 || (char*)mesh.Primitives!=newp+32 ||
        (char*)mesh.Bones!=newp+48 || (char*)mesh.BoneKeyframes!=newp+64 ||
        (char*)mesh.BoneSpaceTransforms!=newp+80) return fail("owned block rebase");
    if (mesh.BoneNameIndices || mesh.BoneNameData) return fail("null preservation");

    mesh.Materials=0; mesh.Primitives=0; mesh.Bones=0; mesh.BoneKeyframes=0; mesh.BoneSpaceTransforms=0;
    mesh.RelocateData(10,oldp,newp,123);
    if(mesh.Materials || mesh.Primitives || mesh.Bones || mesh.BoneKeyframes || mesh.BoneSpaceTransforms || base_calls || primitive_calls) return fail("all-null owned block");
    mesh.RelocateData(30,oldp,newp,789);
    if(mesh.BoneNameIndices || mesh.BoneNameData) return fail("all-null bone block");
    mesh.RelocateData(20,oldp,newp,456);
    if(primitive_calls || base_calls) return fail("empty primitives");
    C3DPrimitive2 parts[2];
    mesh.Primitives=parts; mesh.PrimitiveCount=2;
    primitive_calls=0;
    mesh.RelocateData(20,oldp,newp,456);
    if (primitive_calls!=2 || receivers[0]!=parts || receivers[1]!=parts+1 || got_handle!=20 || got_old!=oldp || got_new!=newp || got_size!=456)
        return fail("primitive virtual dispatch");

    mesh.BoneNameIndices=(unsigned short*)(oldp+96);
    mesh.BoneNameData=oldp+112;
    mesh.RelocateData(30,oldp,newp,789);
    if ((char*)mesh.BoneNameIndices!=newp+96 || mesh.BoneNameData!=newp+112)
        return fail("bone-name rebase");

    base_calls=0;
    mesh.RelocateData(99,oldp,newp,321);
    if (base_calls!=1 || base_receiver!=&mesh || got_handle!=99 || got_old!=oldp || got_new!=newp || got_size!=321)
        return fail("base fallback");
    std::printf("00a8b4c0_TEST PASS\n");
    return 0;
}