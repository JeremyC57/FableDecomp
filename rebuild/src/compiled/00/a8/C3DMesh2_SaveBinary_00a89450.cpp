// C3DMesh2::SaveBinary(CDataOutputStream&) const -- write the mesh stats, counts and raw bone/keyframe blocks, then every material and primitive; return the bytes written
// Retail @ 0x00A89450, 403 bytes (ret 4).
// Callees: CDataOutputStream::GetPosition (vtable slot 2), CDataOutputStream::WriteData (vtable slot 4),
//   C3DMeshStats::SaveBinary 0x00A2EAA0, WriteBool 0x00993E30, WriteUShort 0x00993E80, WriteULong 0x00993EA0,
//   WriteBuffer(const void*, long) 0x00996610 (compressed block), C3DMeshMaterial::SaveBinary 0x00ABF6B0,
//   C3DPrimitive2::SaveBinary 0x00A8FD40.
// Shape: the start position lives in the dead stream-argument slot; the per-primitive static/animated block
// totals are summed in one loop (two registers, pointer stepped by sizeof(C3DPrimitive2)) before being written
// as two unsigned shorts; the two object loops index Materials[i] / Primitives[i] with a byte-offset register.
// Layout: retail C3DMeshStats is 0x7C bytes (the PDB view is 0x84), so every C3DMesh2 member sits 8 below its
// Ego_r.pdb offset: MaterialCount +0x88 ... BoneSpaceTransforms +0xE0, ClothFlag +0xE4. Retail C3DPrimitive2 is
// 0xA0 bytes (PDB 0xAC: its CArray is 12 bytes), StaticBlockCount +0x40, AnimatedBlockCount +0x48.
class CDataOutputStream { public:
    virtual void v0(); virtual void v1();
    virtual unsigned long GetPosition();                       // slot 2 (+0x08)
    virtual void v3();
    virtual void WriteData(const void* data, unsigned long size); // slot 4 (+0x10)
    void WriteUShort(unsigned short value);                    // 0x00993E80
    void WriteBuffer(const void* data, long size);             // 0x00996610 (compressed block)
public:
    void WriteEBOOL(bool);   // 0x00993E30
    void WriteULONG(unsigned long);   // 0x00993EA0
};

class CMatrix3x4 { public: float m_row[3][4]; };
class CMatrix4x4 { public: float m_row[4][4]; };
class C3DBone2 { public: unsigned char _body[0x3C]; };
class C3DKeyframe { public: unsigned char _body[0x30]; };

class C3DMeshMaterial { public:
    unsigned char _body[0x30];
    void SaveBinary(CDataOutputStream& out) const;             // 0x00ABF6B0
};
class C3DPrimitive2 { public:
    unsigned char _pad0[0x40];
    unsigned long StaticBlockCount;                            // +0x40 (PDB +0x44)
    void* StaticBlocks;                                        // +0x44
    unsigned long AnimatedBlockCount;                          // +0x48 (PDB +0x4C)
    unsigned char _pad4c[0xA0 - 0x4C];
    void SaveBinary(CDataOutputStream& out) const;             // 0x00A8FD40
};

class C3DMeshStats { public:
    unsigned char _body[0x7C];
    void SaveBinary(CDataOutputStream& out) const;             // 0x00A2EAA0
};

class C3DMesh2 : public C3DMeshStats { public:
    long MemoryHandle;                    // +0x7C
    long PrimMemoryHandle;                // +0x80
    long BoneNameMemoryHandle;            // +0x84
    unsigned long MaterialCount;          // +0x88
    C3DMeshMaterial* Materials;           // +0x8C
    unsigned long PrimitiveCount;         // +0x90
    C3DPrimitive2* Primitives;            // +0x94
    unsigned long BoneCount;              // +0x98
    C3DBone2* Bones;                      // +0x9C
    C3DKeyframe* BoneKeyframes;           // +0xA0
    unsigned short* BoneNameIndices;      // +0xA4
    char* BoneNameData;                   // +0xA8
    unsigned long BoneNameDataSize;       // +0xAC
    CMatrix3x4 InverseTransform;          // +0xB0
    CMatrix4x4* BoneSpaceTransforms;      // +0xE0
    bool ClothFlag;                       // +0xE4
    bool LockedFromMemoryRelocation;      // +0xE5
    unsigned long MeshID;                 // +0xE8
    unsigned long SaveBinary(CDataOutputStream& out) const;
};

unsigned long C3DMesh2::SaveBinary(CDataOutputStream& out) const
{
    unsigned long start = out.GetPosition();
    C3DMeshStats::SaveBinary(out);
    out.WriteULONG(MaterialCount);
    out.WriteULONG(PrimitiveCount);
    out.WriteULONG(BoneCount);
    out.WriteULONG(BoneNameDataSize);
    out.WriteEBOOL(ClothFlag);
    unsigned long staticBlocks = 0;
    unsigned long animatedBlocks = 0;
    for (unsigned long p = 0; p < PrimitiveCount; ++p) {
        staticBlocks += Primitives[p].StaticBlockCount;
        animatedBlocks += Primitives[p].AnimatedBlockCount;
    }
    out.WriteUShort((unsigned short)staticBlocks);
    out.WriteUShort((unsigned short)animatedBlocks);
    out.WriteData(BoneNameIndices, BoneCount * sizeof(unsigned short));
    out.WriteBuffer(BoneNameData, BoneNameDataSize);
    out.WriteBuffer(Bones, BoneCount * sizeof(C3DBone2));
    out.WriteBuffer(BoneKeyframes, BoneCount * sizeof(C3DKeyframe));
    out.WriteBuffer(BoneSpaceTransforms, BoneCount * sizeof(CMatrix4x4));
    out.WriteData(&InverseTransform, sizeof(CMatrix3x4));
    for (unsigned long i = 0; i < MaterialCount; ++i)
        Materials[i].SaveBinary(out);
    for (unsigned long j = 0; j < PrimitiveCount; ++j)
        Primitives[j].SaveBinary(out);
    return out.GetPosition() - start;
}