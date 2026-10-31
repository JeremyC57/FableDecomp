// CLocalDetailPrimitiveRepeatedMesh::Load(CDataInputStream& stream) -- read the repeated-mesh local-detail primitive: bounding box and sphere, the object count and maximum object scale, allocate the per-object matrix rows (16 bytes each, plain landscape allocation) and the 16-byte-aligned offsets (object count rounded up to 4) and read both, then the optional landscape-normal array (3 floats per aligned object), the optional per-object wind delays and the optional 0x50-byte subsections; false when any allocation fails
// Grade: DIFFER(1532v1533) 104 diff lines (parked stub 93v1533, 574 lines): first full reconstruction on the landed sibling Load 0x00BFB210 stream model -- every read, allocation and early return is in place in retail order; left: register/slot assignment (retail holds this in ebp and the aligned count in edi, spilling both, and homes every read value -- count, scale, the three flags -- in the dead stream-argument slot; ours swaps ebp/edi and homes the count/scale in frame slots, frame 0x10 vs 0xc). Tried: ReadFloat/ReadBool helpers (112: reads move to the argument slot but the frame shrinks to 8), a local subsection count (124), alignedCount before the manager (112)
// Leaf: ?Load@CLocalDetailPrimitiveRepeatedMesh@@UAE_NAAVCDataInputStream@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BFA2A0, length 1533 bytes, `ret 4` (primitive vtable slot 8).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0, __allmul 0x00BFEB50, CEngineLandscapeMemoryManager::
//   AllocVirtual 0x00BD9AF0 / AllocVirtualAlignedSafe_x16 0x00BFB810 / AllocVirtualAlignedSafe_x4 0x00BFB860 /
//   AllocVirtualSafetyChecked 0x00BFB780 / AllocVirtualAlignedSafe_x80 0x00BFB8C0 (landed spellings); the manager is
//   [[0x01436EA8]+0x6b0].
// Layout: the landed sibling CLocalDetailPrimitiveMeshZSpriteBatch::Load 0x00BFB210 (stream reads, AllocArray) and the
//   Ego_r.pdb CLocalDetailPrimitiveRepeatedMesh members as the landed AddObjectsToPrimitiveRenderer 0x00BFACC0 declares
//   them (MaxObjectScale +0x38, ObjectCount +0x3c, SubSectionsCount +0x40, SubSections +0x44, ObjectMatricies +0x48,
//   ObjectOffsets +0x4c, WindDelayArray +0x50, LandscapeNormalArray +0x54).
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CDataInputStream { public:
    void*                __vftable;
    unsigned long        StreamPos;                          // +0x04
    long                 StreamSize;                         // +0x08
    const unsigned char* CurrentSourcePtr;                   // +0x0c
    unsigned long        SourceChunkStreamPos;               // +0x10
    long                 SourceChunkBytesRemaining;          // +0x14
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        if (size <= 0)
            return;
        __int64 newPosition = (__int64)(StreamPos + size);
        if (!IsValidPosition(newPosition))
            return;
        if (size <= SourceChunkBytesRemaining) {
            memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr += size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    template <class T> __forceinline void Read(T& value) { Read(&value, sizeof(T)); }
    __forceinline long ReadLong() { long value; Read(&value, sizeof(value)); return value; }
private:
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
};

class C3DVector { public: float X, Y, Z; };
class C3DBoundingBox { public: C3DVector MinCoord, MaxCoord; };             // 0x18
class C3DBoundingSphere { public: C3DVector Centre; float Radius; };        // 0x10
class C4DVector { public: float X, Y, Z, W; };
struct CSubsectionElement { unsigned char _bytes[0x50]; };

class CEngineLandscapeMemoryManager { public:
    void* AllocVirtual(long bytes, void* owner);                                           // 0x00BD9AF0
    void* AllocVirtualAlignedSafe_x16(long count, unsigned long alignment, long flags);    // 0x00BFB810
    void* AllocVirtualAlignedSafe_x4(long count, unsigned long alignment, long flags);     // 0x00BFB860
    void* AllocVirtualAlignedSafe_x80(long count, unsigned long alignment, long flags);    // 0x00BFB8C0
    void* AllocVirtualSafetyChecked(long size, long flags);                                // 0x00BFB780
    template <class T> __forceinline T* AllocArray(long count, void* owner)
    {
        __int64 bytes = (__int64)count * sizeof(T);
        if (CDataInputStream::IsValidPosition(bytes))
            return (T*)AllocVirtual(count * sizeof(T), owner);
        return 0;
    }
};
struct SEngineLandscape {                                            // reached through the pointer at 0x01436EA8
    unsigned char                  _pad[0x6b0];
    CEngineLandscapeMemoryManager* MemoryManager;                    // +0x6b0
};
extern SEngineLandscape* g_pEngineLandscape;                         // 0x01436EA8

class CLocalDetailPrimitiveRepeatedMesh { public:
    virtual void v00();
    void*               Next;                                        // +0x04
    C3DBoundingBox      BoundingBox;                                 // +0x08
    C3DBoundingSphere   BoundingSphere;                              // +0x20
    void*               Type;                                        // +0x30
    void*               Map;                                         // +0x34
    float               MaxObjectScale;                              // +0x38
    long                ObjectCount;                                 // +0x3c
    long                SubSectionsCount;                            // +0x40
    CSubsectionElement* SubSections;                                 // +0x44
    C4DVector*          ObjectMatricies;                             // +0x48
    C4DVector*          ObjectOffsets;                               // +0x4c
    unsigned char*      WindDelayArray;                              // +0x50
    float*              LandscapeNormalArray;                        // +0x54
    virtual bool Load(CDataInputStream& stream);
};

bool CLocalDetailPrimitiveRepeatedMesh::Load(CDataInputStream& stream)
{
    stream.Read(BoundingBox);
    stream.Read(BoundingSphere);
    ObjectCount = stream.ReadLong();
    float maxScale;
    stream.Read(maxScale);
    MaxObjectScale = maxScale;
    CEngineLandscapeMemoryManager* manager = g_pEngineLandscape->MemoryManager;
    long alignedCount = (ObjectCount + 3) & ~3;
    ObjectMatricies = manager->AllocArray<C4DVector>(ObjectCount, this);
    if (ObjectMatricies == 0)
        return false;
    ObjectOffsets = (C4DVector*)manager->AllocVirtualAlignedSafe_x16(alignedCount, 16, (long)this);
    if (ObjectOffsets == 0)
        return false;
    stream.Read(ObjectMatricies, ObjectCount * sizeof(C4DVector));
    stream.Read(ObjectOffsets, ObjectCount * sizeof(C4DVector));
    bool hasNormals;
    stream.Read(hasNormals);
    if (hasNormals) {
        if (!CDataInputStream::IsValidPosition((__int64)alignedCount * 3))
            return false;
        LandscapeNormalArray = (float*)manager->AllocVirtualAlignedSafe_x4(alignedCount * 3, 16, (long)this);
        if (LandscapeNormalArray == 0)
            return false;
        stream.Read(LandscapeNormalArray, alignedCount * 3 * sizeof(float));
    }
    bool hasWind;
    stream.Read(hasWind);
    if (hasWind) {
        WindDelayArray = (unsigned char*)manager->AllocVirtualSafetyChecked(ObjectCount, (long)this);
        if (WindDelayArray == 0)
            return false;
        stream.Read(WindDelayArray, ObjectCount);
    }
    bool hasSubSections;
    stream.Read(hasSubSections);
    if (hasSubSections) {
        SubSectionsCount = stream.ReadLong();
        SubSections = (CSubsectionElement*)manager->AllocVirtualAlignedSafe_x80(SubSectionsCount, 16, (long)this);
        if (SubSections == 0)
            return false;
        stream.Read(SubSections, SubSectionsCount * sizeof(CSubsectionElement));
    }
    return true;
}
