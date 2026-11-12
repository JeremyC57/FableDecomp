// TEST: CRandomAppearanceMorph::TransferBinaryIn(CMemoryDataInputStream& stream) -- read the body-part mesh arrays, the per-part texture-morph maps, the skeletal-morph map (marked sorted), the general texture-morph map and the highest texture group id
// Behaviour test for 0x0086BFB0: the row body verbatim over portable stand-ins for the shared headers it uses
// (CArray / CVectorMap over std::vector, a little-endian memory stream with ReadULONG / ReadSLONG / ReadEBOOL, CDefString
// reading its table index, GFReadVectorMapMemDump reading a count and that many (key, NewTexture, GroupID) triples).
// C++03; builds with VC7.1 /Od and g++.
#include <stdio.h>
#include <vector>
#include <utility>
class CMemoryDataInputStream { public:
    const unsigned char* Data; unsigned int Pos, Size;
    unsigned long ReadULONG() { unsigned long v = 0; for (int i = 0; i < 4; ++i) v |= (unsigned long)Data[Pos++] << (8 * i); return v; }
    long ReadSLONG() { return (long)(int)ReadULONG(); }
    bool ReadEBOOL() { return Data[Pos++] != 0; }
};
class CDefString { public:
    CDefString() : TablePos(-1) { }
    void TransferBinaryIn(CMemoryDataInputStream& stream) { TablePos = stream.ReadSLONG(); }
    long TablePos;
};
template<class T> class CArray : public std::vector<T> { public:
    T& operator[](unsigned int i) { return *(this->begin() + i); }
};
template<class K,class V> class CVectorMap : public CArray<std::pair<K,V> > { public:
    CVectorMap() : Dirty(true) { }
    void SetDirty(bool dirty) { Dirty = dirty; }
    bool Dirty;
};
class CRandomAppearanceMorph { public:
    class CTextureMorph { public: unsigned long NewTexture; unsigned long GroupID; };
    typedef CVectorMap<unsigned long,CTextureMorph> TTextureMorphMap;
    class CBodyPartMesh { public: CBodyPartMesh() : MeshIndex(0), Priority(0) { } long MeshIndex; long Priority; TTextureMorphMap TextureMorphs; };
    class CSkeletalMorph { public: CSkeletalMorph() : GroupID(0), AllowVariation(false) { } long GroupID; CDefString Name; bool AllowVariation; };
    void TransferBinaryIn(CMemoryDataInputStream& stream);
    CArray<CArray<CBodyPartMesh> > BodyParts;
    CArray<TTextureMorphMap> BodyPartTextureMorphs;
    CVectorMap<long,CSkeletalMorph> SkeletalMorphs;
    TTextureMorphMap GeneralTextureMorphs;
    unsigned long MaxTextureGroupID;
};
template<class V,class K> void GFReadVectorMapMemDump(CMemoryDataInputStream& stream, CVectorMap<K,V>& map)
{
    unsigned long n = stream.ReadULONG(); map.resize(n);
    for (unsigned long i = 0; i < n; ++i) { map[i].first = stream.ReadULONG(); map[i].second.NewTexture = stream.ReadULONG(); map[i].second.GroupID = stream.ReadULONG(); }
}
// ---- row body (0x0086BFB0) ----
void CRandomAppearanceMorph::TransferBinaryIn(CMemoryDataInputStream& stream)
{
    unsigned long numBodyParts = stream.ReadULONG();
    for (unsigned long i = 0; i < numBodyParts; ++i) {
        CArray<CBodyPartMesh>& meshes = BodyParts[i];
        unsigned long numMeshes = stream.ReadULONG();
        meshes.resize(numMeshes);
        for (unsigned long j = 0; j < numMeshes; ++j) {
            CBodyPartMesh& mesh = meshes[j];
            mesh.MeshIndex = stream.ReadSLONG();
            mesh.Priority = stream.ReadSLONG();
            GFReadVectorMapMemDump(stream, mesh.TextureMorphs);
        }
    }
    for (unsigned long k = 0; k < numBodyParts; ++k)
        GFReadVectorMapMemDump(stream, BodyPartTextureMorphs[k]);
    unsigned long numSkeletalMorphs = stream.ReadULONG();
    SkeletalMorphs.resize(numSkeletalMorphs);
    for (unsigned long m = 0; m < numSkeletalMorphs; ++m) {
        SkeletalMorphs[m].first = stream.ReadSLONG();
        CSkeletalMorph& morph = SkeletalMorphs[m].second;
        morph.GroupID = stream.ReadSLONG();
        morph.Name.TransferBinaryIn(stream);
        morph.AllowVariation = stream.ReadEBOOL();
    }
    SkeletalMorphs.SetDirty(false);
    GFReadVectorMapMemDump(stream, GeneralTextureMorphs);
    MaxTextureGroupID = stream.ReadULONG();
}
static unsigned char g_bytes[512]; static unsigned int g_len;
static void PutL(unsigned long v) { for (int i = 0; i < 4; ++i) g_bytes[g_len++] = (unsigned char)(v >> (8 * i)); }
int main()
{
    // two body parts: part 0 shrinks from 3 meshes to 1 (erase arm), part 1 grows from 0 to 2 (fill arm)
    PutL(2);
    PutL(1); PutL(7); PutL(9); PutL(1); PutL(100); PutL(5); PutL(6);
    PutL(2); PutL(11); PutL(12); PutL(0); PutL(13); PutL(14); PutL(0);
    PutL(0); PutL(1); PutL(200); PutL(1); PutL(2);       // the two body parts' texture-morph maps
    PutL(2); PutL(40); PutL(3); PutL(77); g_bytes[g_len++] = 1; PutL(41); PutL((unsigned long)-2); PutL(78); g_bytes[g_len++] = 0;
    PutL(1); PutL(300); PutL(8); PutL(9);                // general map
    PutL(12);                                            // highest texture group id
    CMemoryDataInputStream stream; stream.Data = g_bytes; stream.Pos = 0; stream.Size = g_len;
    CRandomAppearanceMorph morph;
    morph.BodyParts.resize(2); morph.BodyParts[0].resize(3); morph.BodyPartTextureMorphs.resize(2);
    morph.SkeletalMorphs.resize(5);
    morph.TransferBinaryIn(stream);
    bool ok = stream.Pos == g_len && morph.BodyParts[0].size() == 1 && morph.BodyParts[1].size() == 2
        && morph.BodyParts[0][0].MeshIndex == 7 && morph.BodyParts[0][0].Priority == 9 && morph.BodyParts[0][0].TextureMorphs.size() == 1
        && morph.BodyParts[0][0].TextureMorphs[0].first == 100 && morph.BodyParts[0][0].TextureMorphs[0].second.GroupID == 6
        && morph.BodyParts[1][1].MeshIndex == 13 && morph.BodyParts[1][1].TextureMorphs.size() == 0
        && morph.BodyPartTextureMorphs[0].size() == 0 && morph.BodyPartTextureMorphs[1][0].first == 200
        && morph.SkeletalMorphs.size() == 2 && !morph.SkeletalMorphs.Dirty
        && morph.SkeletalMorphs[0].first == 40 && morph.SkeletalMorphs[0].second.GroupID == 3
        && morph.SkeletalMorphs[0].second.Name.TablePos == 77 && morph.SkeletalMorphs[0].second.AllowVariation
        && morph.SkeletalMorphs[1].second.GroupID == -2 && !morph.SkeletalMorphs[1].second.AllowVariation
        && morph.GeneralTextureMorphs.size() == 1 && morph.GeneralTextureMorphs[0].second.NewTexture == 8 && morph.MaxTextureGroupID == 12;
    puts(ok ? "0086bfb0_TEST PASS" : "0086bfb0_TEST FAIL");
    return ok ? 0 : 1;
}