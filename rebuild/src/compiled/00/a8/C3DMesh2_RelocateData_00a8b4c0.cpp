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