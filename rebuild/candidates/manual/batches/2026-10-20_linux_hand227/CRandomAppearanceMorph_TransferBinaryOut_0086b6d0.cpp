// CRandomAppearanceMorph::TransferBinaryOut(CDataOutputStream* out) -- write the morph: the three body parts' mesh lists, the three per-part texture-morph maps, the skeletal morphs, the general texture morphs and the max texture group id
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086B6D0, length 308 bytes, `ret 4`.
// Callees: CDataOutputStream::WriteULong 0x00993EA0, WriteLong 0x00993EB0, WriteBool 0x00993E30 (the skeletal morph's
// flag byte), GFWriteVectorMapMemDump<CTextureMorph, unsigned long, ...>(CDataOutputStream*, const CVectorMap&)
// 0x0086B210 (`__fastcall`).
// Layout (retail; Ego_r.pdb 76 bytes): BodyParts (CArray<CArray<CBodyPartMesh>>, data pointer +0x00, always 3 parts),
// BodyPartTextureMorphs (CArray<CVectorMap<unsigned long, CTextureMorph>>, data pointer +0x0c, 3 maps of 16 bytes),
// SkeletalMorphs (CVectorMap<long, CSkeletalMorph>, 16-byte entries) +0x18, GeneralTextureMorphs +0x28,
// MaxTextureGroupID +0x38. CBodyPartMesh: two longs + a texture-morph map (24 bytes); skeletal entry: key, two
// longs, a flag byte.
// Shape: fixed-count part loops (`< 3`, strength-reduced to byte offsets), size() loops with unsigned `jbe` guards
// and down-counted trip counts.
struct CDataOutputStream {
    void WriteBool(bool value);                        // 0x00993E30
    void WriteULong(unsigned long value);              // 0x00993EA0
    void WriteLong(long value);                        // 0x00993EB0
};

template <class T> struct CArray {                     // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) { return *(_M_start + n); }
};

struct CTextureMorphEntry {                            // pair<unsigned long, CTextureMorph> (12 bytes)
    unsigned long Key;
    long          A;
    long          B;
};
struct CTextureMorphMap {                              // CVectorMap<unsigned long, CTextureMorph, ...> (16 bytes)
    CArray<CTextureMorphEntry> Entries;
    long                       Compare;
};
void __fastcall GFWriteVectorMapMemDump(CDataOutputStream* out, const CTextureMorphMap* map);   // 0x0086B210

struct CBodyPartMesh {                                 // 24 bytes
    long             MeshID;                           // +0x00
    long             Weight;                           // +0x04
    CTextureMorphMap TextureMorphs;                    // +0x08
};
struct CSkeletalMorph {
    long Bone;                                         // +0x04 in the entry
    long Amount;                                       // +0x08
    bool Enabled;                                      // +0x0c
};
struct CSkeletalMorphEntry {                           // pair<long, CSkeletalMorph> (16 bytes)
    long           Key;
    CSkeletalMorph Morph;
};
struct CSkeletalMorphMap {                             // CVectorMap<long, CSkeletalMorph, ...> (16 bytes)
    CArray<CSkeletalMorphEntry> Entries;
    long                        Compare;
};

struct CRandomAppearanceMorph {
    CArray<CArray<CBodyPartMesh> > BodyParts;          // +0x00
    CArray<CTextureMorphMap>       BodyPartTextureMorphs;   // +0x0c
    CSkeletalMorphMap              SkeletalMorphs;     // +0x18
    CTextureMorphMap               GeneralTextureMorphs;    // +0x28
    unsigned long                  MaxTextureGroupID;  // +0x38
    void TransferBinaryOut(CDataOutputStream* out);
};

void CRandomAppearanceMorph::TransferBinaryOut(CDataOutputStream* out)
{
    out->WriteULong(3);
    for (unsigned long part = 0; part < 3; ++part) {
        CArray<CBodyPartMesh>& meshes = BodyParts[part];
        unsigned long count = meshes.size();
        out->WriteULong(count);
        for (unsigned long i = 0; i < count; ++i) {
            CBodyPartMesh& mesh = meshes[i];
            out->WriteLong(mesh.MeshID);
            out->WriteLong(mesh.Weight);
            GFWriteVectorMapMemDump(out, &mesh.TextureMorphs);
        }
    }
    for (unsigned long part = 0; part < 3; ++part)
        GFWriteVectorMapMemDump(out, &BodyPartTextureMorphs[part]);
    unsigned long count = SkeletalMorphs.Entries.size();
    out->WriteULong(count);
    for (unsigned long i = 0; i < count; ++i) {
        out->WriteLong(SkeletalMorphs.Entries[i].Key);
        CSkeletalMorph& morph = SkeletalMorphs.Entries[i].Morph;
        out->WriteLong(morph.Bone);
        out->WriteLong(morph.Amount);
        out->WriteBool(morph.Enabled);
    }
    GFWriteVectorMapMemDump(out, &GeneralTextureMorphs);
    out->WriteULong(MaxTextureGroupID);
}
