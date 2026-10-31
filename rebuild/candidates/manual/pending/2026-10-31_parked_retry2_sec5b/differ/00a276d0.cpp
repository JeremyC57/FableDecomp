// CMeshDataBank::InitialiseEntry(unsigned long type, unsigned long index, const CArray<char>& data) -- parse one mesh bank entry header from its data block: render meshes (types 1,2,4,5) read the physics-mesh index, bounding sphere, bounding box, safe radius and the LOD data sizes / errors; animations (types 6..10) read duration, non-looping duration, movement vector and rotation, and lip-sync animations (type & 0xf == 8) register (phoneme name, index) under the animation name
// Grade: DIFFER(1298v1330) 152/438 -- block layout, frame (0x50) and every call/read match; residue is register allocation: retail keeps type in ebx and this in esi for the whole body and homes the entry pointer in the dead index-argument slot, so the centre vector lives in memory and the sphere is built right after the radius read; here type is re-read from its argument slot and the centre is held in esi/edi/ebx (the read buffers then use a different dead argument slot)
// Leaf: ?InitialiseEntry@CMeshDataBank@@MAEXKKABV?$CArray@D@@@Z
// Flags: /O2 /Oy /W3
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A276D0, 1330 bytes, `ret 0Ch`.
// Name: Ego_r.pdb + FableWin.pdb ?InitialiseEntry@CMeshDataBank@@MAEXKKABV?$CArray@D@@@Z; the type tests are FableWin.pdb's inline
//   ?IsRenderMesh@CMeshDataBank@@SI_NK@Z / ?IsAnimation@CMeshDataBank@@SI_NK@Z (single PDB source; the 1,2,4,5 / 6,10,7,8,9 value
//   sets are read from the retail compares -- review at landing).
// Callees: CMemoryDataInputStream(const void*, unsigned long) 0x00994930, ~CDataStream 0x00993B10 (the implicit stream
//   destructors chain to it), CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (inline Read template),
//   CDataInputStream::Read3DVector 0x009950F0, CMemoryDataInputStream::ReadNullTerminatedString 0x00995D30 (devirtualised on
//   the local stream), map<CCharString, CArray<CLipSyncEntry> >::operator[] 0x00A2E290, CArray<CLipSyncEntry>::push_back
//   0x00A2D8B0, CCharString copy 0x0099EC30 / ~ 0x0099EAE0.
// Layout: engine/CMeshDataBank.h retail offsets (MeshesAndAnimations +0x380, LipSyncAnimations +0x3C8); engine/C3DMeshInfo.h
//   (BoundingBox +0x10, BoundingSphere +0x28, SafeBoundingRadius +0x38, PhysicsMeshIndex +0x3C, LODErrors +0x40,
//   LODDataSizes +0x50); Ego_r.pdb C3DAnimationInfo (Duration +0x10 .. Rotation +0x24), CLipSyncEntry {PhonemeName, AnimationIndex}.
// Levers: inline IsRenderMesh/IsAnimation helpers (render-mesh block falls through as retail: 500 -> 235); centre by
//   assignment + box vectors by direct init (frame 0x5c -> 0x50); memset of the LOD arrays (235 -> 158); box from a
//   two-vector ctor (152). Inert: early return, switch (jump table), animation-first order, hoisted per-branch casts,
//   entry before the stream, named CLipSyncEntry, cast-per-use, aggregate box init.
extern "C" void* __cdecl memcpy(void*, const void*, unsigned int);
extern "C" void* __cdecl memset(void*, int, unsigned int);
#pragma intrinsic(memcpy, memset)
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
class C3DVector { public:
    float X, Y, Z;
    float GetMagnitude() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
};
class C3DBoundingBox { public:
    C3DVector Min;
    C3DVector Max;
    C3DBoundingBox() {}
    C3DBoundingBox(const C3DVector& min, const C3DVector& max) : Min(min), Max(max) {}
};
class C3DBoundingSphere { public:
    C3DVector Centre;
    float Radius;
    C3DBoundingSphere() {}
    C3DBoundingSphere(const C3DVector& centre, float radius) : Centre(centre), Radius(radius) {}
};
class CDataStream { public:
    virtual ~CDataStream();                             // 0x00993B10
};
class CDataInputStream : public CDataStream { public:
    unsigned long StreamPos;                            // +0x04
    long StreamSize;                                    // +0x08
    const unsigned char* CurrentSourcePtr;              // +0x0c
    unsigned long SourceChunkStreamPos;                 // +0x10
    long SourceChunkBytesRemaining;                     // +0x14
    static __forceinline bool IsValidPosition(__int64 p) { return p >= 0 && p <= 0x7fffffff; }
    __forceinline void Read(void* dst, long size)
    {
        __int64 np = (__int64)(StreamPos + size);
        if (!IsValidPosition(np))
            return;
        if (SourceChunkBytesRemaining >= size) {
            memcpy(dst, CurrentSourcePtr, size);
            CurrentSourcePtr += size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        } else
            ReadWithSrcChunkOverflow((unsigned char*)dst, size);
    }
    __forceinline unsigned long ReadULONG() { unsigned long value; Read(&value, 4); return value; }
    __forceinline long ReadSLONG() { long value; Read(&value, 4); return value; }
    __forceinline float ReadFloat() { float value; Read(&value, 4); return value; }
    C3DVector Read3DVector();                           // 0x009950F0
    virtual CCharString ReadNullTerminatedString();
private:
    void ReadWithSrcChunkOverflow(unsigned char*, long);   // 0x00993CA0
};
class CMemoryDataInputStream : public CDataInputStream { public:
    const unsigned char* Data;                          // +0x18
    CMemoryDataInputStream(const void* data, unsigned long size);   // 0x00994930
    virtual CCharString ReadNullTerminatedString();     // 0x00995D30
};
template <class T> class CArray { public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    const T* GetData() const { return size() != 0 ? _M_start : (const T*)this; }
    T& operator[](unsigned long n) { return _M_start[n]; }
    void push_back(const T& x);
};
class CMeshDataBank;
class CMeshDataBankEntry { public:
    void* __vftable;
    CMeshDataBank* Bank;
    unsigned long Index;
};
class C3DMeshInfo : public CMeshDataBankEntry { public:
    void* MeshData;                                     // +0x0c
    C3DBoundingBox BoundingBox;                         // +0x10
    C3DBoundingSphere BoundingSphere;                   // +0x28
    float SafeBoundingRadius;                           // +0x38
    long PhysicsMeshIndex;                              // +0x3c
    float LODErrors[4];                                 // +0x40
    unsigned long LODDataSizes[4];                      // +0x50
};
class C3DAnimationInfo : public CMeshDataBankEntry { public:
    void* Data;                                         // +0x0c
    float Duration;                                     // +0x10
    float NonLoopingDuration;                           // +0x14
    C3DVector MovementVector;                           // +0x18
    float Rotation;                                     // +0x24
};
class CMeshDataBank { public:
    class CLipSyncEntry { public:
        CCharString PhonemeName;
        unsigned long AnimationIndex;
        CLipSyncEntry(const CCharString& name, unsigned long index) : PhonemeName(name), AnimationIndex(index) {}
    };
    class CLipSyncMap { public:
        unsigned char _body[0xc];
        CArray<CLipSyncEntry>& operator[](const CCharString& key);   // 0x00A2E290
    };
    unsigned char _pad0[0x37c];                         // after the vptr
    CArray<CMeshDataBankEntry*> MeshesAndAnimations;    // +0x380
    unsigned char _pad38c[0x3c8 - 0x38c];
    CLipSyncMap LipSyncAnimations;                      // +0x3c8
    static bool __fastcall IsRenderMesh(unsigned long type) { return type == 1 || type == 2 || type == 4 || type == 5; }   // FableWin.pdb ?IsRenderMesh@CMeshDataBank@@SI_NK@Z (inline)
    static bool __fastcall IsAnimation(unsigned long type) { return type == 6 || type == 10 || type == 7 || type == 8 || type == 9; }   // FableWin.pdb ?IsAnimation@CMeshDataBank@@SI_NK@Z (inline)
    virtual void InitialiseEntry(unsigned long type, unsigned long index, const CArray<char>& data);
};
void CMeshDataBank::InitialiseEntry(unsigned long type, unsigned long index, const CArray<char>& data)
{
    CMemoryDataInputStream stream(data.GetData(), data.size());
    CMeshDataBankEntry* entry = MeshesAndAnimations[index];
    if (IsRenderMesh(type)) {
        C3DMeshInfo* info = (C3DMeshInfo*)entry;
        long physicsIndex = stream.ReadSLONG();
        if ((type == 1 || type == 5) && physicsIndex != 0)
            info->PhysicsMeshIndex = physicsIndex;
        C3DVector centre;
        centre = stream.Read3DVector();
        float radius = stream.ReadFloat();
        C3DBoundingSphere sphere(centre, radius);
        C3DVector boxMin = stream.Read3DVector();
        C3DVector boxMax = stream.Read3DVector();
        float safeRadius = radius;
        if (type == 5)
            safeRadius = centre.GetMagnitude() + radius;
        info->BoundingSphere = sphere;
        info->BoundingBox = C3DBoundingBox(boxMin, boxMax);
        info->SafeBoundingRadius = safeRadius;
        unsigned long lods = stream.ReadULONG();
        unsigned long i;
        memset(info->LODDataSizes, 0, sizeof(info->LODDataSizes));
        for (i = 0; i < lods; ++i)
            info->LODDataSizes[i] = stream.ReadULONG();
        memset(info->LODErrors, 0, sizeof(info->LODErrors));
        for (i = 0; i < lods; ++i)
            info->LODErrors[i] = stream.ReadFloat();
        return;
    }
    if (IsAnimation(type)) {
        C3DAnimationInfo* anim = (C3DAnimationInfo*)entry;
        anim->Duration = stream.ReadFloat();
        anim->NonLoopingDuration = stream.ReadFloat();
        anim->MovementVector = stream.Read3DVector();
        anim->Rotation = stream.ReadFloat();
        if ((type & 0xf) == 8) {
            CCharString animationName = stream.ReadNullTerminatedString();
            CCharString phonemeName = stream.ReadNullTerminatedString();
            LipSyncAnimations[animationName].push_back(CLipSyncEntry(phonemeName, index));
        }
    }
}
