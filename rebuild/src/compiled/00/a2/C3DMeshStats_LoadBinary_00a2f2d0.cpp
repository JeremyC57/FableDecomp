// C3DMeshStats::LoadBinary(CMemoryDataInputStream*) -- read name, animated flag, bounds and the five counts, allocate the buffers, bulk-read helper/dummy/name blocks, then load every mesh volume and generator
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A2F2D0, 1136 bytes, ret 4.
// Callees: stream vtable slot 6 (+0x18) ReadNullTerminatedString (hidden CCharString result), slot 4 (+0x10)
//   Read(void*, ulong); CCharString::operator= 0x0099EFB0, ~CCharString 0x0099EAE0,
//   CDataInputStream::Read3DVector 0x009950F0 (hidden C3DVector result), ReadWithSrcChunkOverflow 0x00993CA0,
//   C3DMeshStats::AllocateBuffers 0x00A2E760, MeshVolumes resize 0x00A35580, C3DMeshVolume::LoadBinary 0x00A2E860,
//   MeshGenerators resize 0x00A34DF0, C3DGeneratorObject::LoadBinary 0x00A2E5B0.
// Shape: every scalar read is the inlined CDataInputStream::Read (64-bit position range check, direct copy while the
//   chunk has room, else ReadWithSrcChunkOverflow), as in the landed 0x007AD7F0 / 0x00AAAB10.
// Layout: retail differs from Ego_r.pdb after +0x58 (MeshVolumes is 0x10 bytes in retail, 0x14 in the PDB):
//   MeshGenerators +0x68, PackedNames +0x74, PackedNamesSize +0x78, AnimatedFlag +0x7a. Local model.
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CMovableResourceMemoryPool;
class CCharString { public:
    void* m_pStringData;
    ~CCharString();                                       // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);     // 0x0099EFB0
};
class C3DVector { public: float X, Y, Z; };

class CMemoryDataInputStream { public:
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void Read(void* buffer, unsigned long size);  // slot 4 (+0x10)
    virtual void v5();
    virtual CCharString ReadNullTerminatedString();       // slot 6 (+0x18)
    C3DVector Read3DVector();                             // 0x009950F0
    unsigned long m_position;              // +4
    long StreamSize;                       // +8
    const unsigned char* m_pData;          // +0xC
    long SourceChunkStreamPos;             // +0x10
    long m_remaining;                      // +0x14
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void ReadInline(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(m_position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (m_remaining >= size) {
            memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    __forceinline bool ReadBool() { unsigned char value; ReadInline(&value, sizeof(value)); return value != 0; }
    __forceinline unsigned long ReadULong() { unsigned long value; ReadInline(&value, sizeof(value)); return value; }
    __forceinline float ReadFloat() { float value; ReadInline(&value, sizeof(value)); return value; }
    __forceinline unsigned short ReadUShort() { unsigned short value; ReadInline(&value, sizeof(value)); return value; }
};

class C3DBoundingSphere { public: C3DVector Centre; float Radius; };
class C3DBoundingBox { public:
    C3DVector Min; C3DVector Max;
    void Set(const C3DVector& mn, const C3DVector& mx) { Min = mn; Max = mx; }
};
class C3DHelperPoint { public: unsigned char _data[0x14]; };
class C3DDummyObject { public: unsigned char _data[0x38]; };
class C3DMeshVolume { public:
    unsigned char _data[0xc];
    bool LoadBinary(CMemoryDataInputStream* in, CMovableResourceMemoryPool* pool);   // 0x00A2E860
};
class C3DGeneratorObject { public:
    unsigned char _data[0x40];
    void LoadBinary(CMemoryDataInputStream* in);                                    // 0x00A2E5B0
};
template <class T> struct CPreAllocVectorMap {
    void* m_pPool;                         // +0
    T* Data;                               // +4
    unsigned long Count;                   // +8
    bool Sorted;                           // +0xc
    unsigned char _pad[3];
};
struct CMeshVolumeEntry { unsigned long Key; C3DMeshVolume Value; };
struct CMeshVolumeMap {
    CMeshVolumeEntry* _M_start;            // +0
    CMeshVolumeEntry* _M_finish;           // +4
    CMeshVolumeEntry* _M_end_of_storage;   // +8
    unsigned char _cmp;                    // +0xc
    bool Sorted;                           // +0xd
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    CMeshVolumeEntry* begin() { return _M_start; }
    CMeshVolumeEntry& operator[](unsigned long n) { return *(begin() + n); }
    void resize(unsigned long n);          // 0x00A35580
};
struct CGeneratorArray {
    C3DGeneratorObject* _M_start;
    C3DGeneratorObject* _M_finish;
    C3DGeneratorObject* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    void resize(unsigned long n);          // 0x00A34DF0
};

class C3DMeshStats { public:
    void* __vftable;                                       // +0x00
    CMovableResourceMemoryPool* MemoryPool;                // +0x04
    long StatsMemoryHandle;                                // +0x08
    CCharString Name;                                      // +0x0c
    C3DBoundingSphere BoundingSphere;                      // +0x10
    C3DBoundingBox BoundingBox;                            // +0x20
    CPreAllocVectorMap<C3DHelperPoint> HelperPoints;       // +0x38
    CPreAllocVectorMap<C3DDummyObject> DummyObjects;       // +0x48
    CMeshVolumeMap MeshVolumes;                            // +0x58
    CGeneratorArray MeshGenerators;                        // +0x68
    char* PackedNames;                                     // +0x74
    unsigned short PackedNamesSize;                        // +0x78
    bool AnimatedFlag;                                     // +0x7a
    bool AllocateBuffers(unsigned long helpers, unsigned long dummies, unsigned long namesSize);   // 0x00A2E760
    bool LoadBinary(CMemoryDataInputStream* in);
};

bool C3DMeshStats::LoadBinary(CMemoryDataInputStream* in)
{
    float u0 = 0.0f;
    float u1 = 0.0f;
    float u2 = 0.0f;
    float u3 = 0.0f;
    float u4 = 0.0f;
    float u5 = 0.0f;
    float u6 = 0.0f;
    float u7 = 0.0f;
    Name = in->ReadNullTerminatedString();
    AnimatedFlag = in->ReadBool();
    BoundingSphere.Centre = in->Read3DVector();
    BoundingSphere.Radius = in->ReadFloat();
    C3DVector boxMin = in->Read3DVector();
    C3DVector boxMax = in->Read3DVector();
    BoundingBox.Set(boxMin, boxMax);
    unsigned long numHelpers = in->ReadUShort();
    unsigned long numDummies = in->ReadUShort();
    PackedNamesSize = in->ReadUShort();
    unsigned long numVolumes = in->ReadUShort();
    unsigned long numGenerators = in->ReadUShort();
    if (!AllocateBuffers(numHelpers, numDummies, PackedNamesSize))
        return false;
    in->Read(HelperPoints.Data, HelperPoints.Count * sizeof(C3DHelperPoint));
    HelperPoints.Sorted = false;
    in->Read(DummyObjects.Data, DummyObjects.Count * sizeof(C3DDummyObject));
    HelperPoints.Sorted = false;
    in->Read(PackedNames, PackedNamesSize);
    MeshVolumes.resize(numVolumes);
    for (unsigned long i = 0; i < MeshVolumes.size(); ++i) {
        MeshVolumes[i].Key = in->ReadULong();
        if (!MeshVolumes[i].Value.LoadBinary(in, MemoryPool))
            return false;
    }
    MeshVolumes.Sorted = false;
    MeshGenerators.resize(numGenerators);
    for (unsigned long j = 0; j < MeshGenerators.size(); ++j)
        MeshGenerators._M_start[j].LoadBinary(in);
    return true;
}