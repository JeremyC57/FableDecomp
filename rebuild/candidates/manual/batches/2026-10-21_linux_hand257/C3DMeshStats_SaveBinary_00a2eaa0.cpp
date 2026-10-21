// C3DMeshStats::SaveBinary(CDataOutputStream&) const -- write the name, flags, bounds, table sizes, the raw helper/dummy/name blocks, then each mesh volume and generator
// Retail @ 0x00A2EAA0, 407 bytes (ret 4).
// Callees (CDataOutputStream): WriteBool 0x00993E30, WriteUShort 0x00993E80, WriteULong 0x00993EA0, WriteLong 0x00993EB0,
//   WriteFloat 0x00993EE0, Write3DVector 0x00993FD0, WriteMatrix3x4 0x00994070, WriteNullTerminatedString 0x00995AE0,
//   WriteBuffer(const void*, long) 0x00996610; CContainmentVolume::SaveBinary(CDataOutputStream&) 0x00A75400.
// Shape: straight-line writes in member order of the file format, then two index loops (the mesh-volume loop keeps
// its index in the dead stream-argument slot and a byte offset in ebx; the generator loop the other way round),
// each re-reading the container's begin/end per iteration because every write is an opaque call.
// Layout (no engine header for C3DMeshStats; Ego_r.pdb names): Name +0x0C, BoundingSphere +0x10, BoundingBox +0x20,
// HelperPoints +0x38 (CPreAllocVectorMap: data +4, count +8; 20-byte C3DHelperPoint), DummyObjects +0x48 (56-byte
// C3DDummyObject), MeshVolumes +0x58 (CVectorMap, 16-byte key/value pairs), MeshGenerators +0x68 (64-byte
// C3DGeneratorObject), PackedNames +0x74, PackedNamesSize +0x78, AnimatedFlag +0x7A. Retail CVectorMap is 16 bytes and
// CArray 12 bytes (the PDB has 20/16), so every member after MeshVolumes sits 4 or 8 bytes below the PDB offset.
struct CCharString { void* m_pStringData; };
struct C3DVector { float X, Y, Z; };
struct CMatrix3x4 { float m_row[3][4]; };

struct CDataOutputStream {
    void WriteBool(bool value);                           // 0x00993E30
    void WriteUShort(unsigned short value);               // 0x00993E80
    void WriteULong(unsigned long value);                 // 0x00993EA0
    void WriteLong(long value);                           // 0x00993EB0
    void WriteFloat(float value);                         // 0x00993EE0
    void Write3DVector(const C3DVector& value);           // 0x00993FD0
    void WriteMatrix3x4(const CMatrix3x4& m);             // 0x00994070
    void WriteNullTerminatedString(const CCharString& s); // 0x00995AE0
    void WriteBuffer(const void* data, long size);      // 0x00996610 (compressed block)
};

struct CContainmentVolume {
    void SaveBinary(CDataOutputStream& out) const;        // 0x00A75400
};

struct C3DBoundingSphere { C3DVector Center; float Radius; };
struct C3DBoundingBox { C3DVector MinCoord; C3DVector MaxCoord; };
struct C3DHelperPoint { unsigned char _body[0x14]; };
struct C3DDummyObject { unsigned char _body[0x38]; };
struct C3DMeshVolume {
    CCharString Name;                     // +0
    CContainmentVolume* ConvexVolume;     // +4 (CCountedPointer<CContainmentVolume>: the object pointer)
    long _reserved;                       // +8
};
struct CMeshVolumePair { unsigned long first; C3DMeshVolume second; };
struct C3DGeneratorObject {
    CMatrix3x4 Transform;                 // +0x00
    long BoneIndex;                       // +0x30 (unnamed in the PDB; written as a long)
    CCharString ObjectName;               // +0x34
    unsigned long BankIndex;                 // +0x38
    bool UseLocalOrigin;                     // +0x3c
};

template <class T> struct CPreAllocVectorMap {
    void* m_pPool;                        // +0
    T* m_pData;                           // +4
    unsigned long m_count;                // +8
    unsigned long m_capacity;             // +C
};
struct CMeshVolumeMap {
    CMeshVolumePair* _M_start;
    CMeshVolumePair* _M_finish;
    CMeshVolumePair* _M_end_of_storage;
    long _M_compare;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    const CMeshVolumePair& operator[](unsigned long n) const { return _M_start[n]; }
};
struct CGeneratorArray {
    C3DGeneratorObject* _M_start;
    C3DGeneratorObject* _M_finish;
    C3DGeneratorObject* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    const C3DGeneratorObject& operator[](unsigned long n) const { return _M_start[n]; }
};

struct C3DMeshStats {
    void* _vtable;                                        // +0x00
    void* MemoryPool;                                     // +0x04
    long StatsMemoryHandle;                               // +0x08
    CCharString Name;                                     // +0x0C
    C3DBoundingSphere BoundingSphere;                     // +0x10
    C3DBoundingBox BoundingBox;                           // +0x20
    CPreAllocVectorMap<C3DHelperPoint> HelperPoints;      // +0x38
    CPreAllocVectorMap<C3DDummyObject> DummyObjects;      // +0x48
    CMeshVolumeMap MeshVolumes;                           // +0x58
    CGeneratorArray MeshGenerators;                       // +0x68
    char* PackedNames;                                    // +0x74
    unsigned short PackedNamesSize;                       // +0x78
    bool AnimatedFlag;                                    // +0x7A
    void SaveBinary(CDataOutputStream& out) const;
};

void C3DMeshStats::SaveBinary(CDataOutputStream& out) const
{
    out.WriteNullTerminatedString(Name);
    out.WriteBool(AnimatedFlag);
    out.Write3DVector(BoundingSphere.Center);
    out.WriteFloat(BoundingSphere.Radius);
    out.Write3DVector(BoundingBox.MinCoord);
    out.Write3DVector(BoundingBox.MaxCoord);
    out.WriteUShort((unsigned short)HelperPoints.m_count);
    out.WriteUShort((unsigned short)DummyObjects.m_count);
    out.WriteUShort(PackedNamesSize);
    out.WriteUShort((unsigned short)MeshVolumes.size());
    out.WriteUShort((unsigned short)MeshGenerators.size());
    out.WriteBuffer(HelperPoints.m_pData, HelperPoints.m_count * sizeof(C3DHelperPoint));
    out.WriteBuffer(DummyObjects.m_pData, DummyObjects.m_count * sizeof(C3DDummyObject));
    out.WriteBuffer(PackedNames, PackedNamesSize);
    for (unsigned long i = 0; i < MeshVolumes.size(); ++i) {
        out.WriteULong(MeshVolumes[i].first);
        const C3DMeshVolume& volume = MeshVolumes[i].second;
        out.WriteNullTerminatedString(volume.Name);
        volume.ConvexVolume->SaveBinary(out);
    }
    for (unsigned long j = 0; j < MeshGenerators.size(); ++j) {
        const C3DGeneratorObject& generator = MeshGenerators[j];
        out.WriteMatrix3x4(generator.Transform);
        out.WriteLong(generator.BoneIndex);
        out.WriteNullTerminatedString(generator.ObjectName);
        out.WriteULong(generator.BankIndex);
        out.WriteBool(generator.UseLocalOrigin);
    }
}
