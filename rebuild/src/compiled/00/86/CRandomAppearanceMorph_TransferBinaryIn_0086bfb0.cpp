// CRandomAppearanceMorph::TransferBinaryIn(CMemoryDataInputStream& stream) -- read the body-part count; for each body part resize its mesh array to the stored count and read every mesh's index, priority and texture-morph map; read each body part's texture-morph map; resize the skeletal-morph map to the stored count and read every entry's key, group, name and variation flag (marking the map sorted); then read the general texture-morph map and the highest texture group id
// Leaf: ?TransferBinaryIn@CRandomAppearanceMorph@@QAEXAAVCMemoryDataInputStream@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0086BFB0, 1305 bytes (int3 padded to 1312), `ret 4`.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0, vector<CBodyPartMesh>::erase 0x0086E150 / _M_fill_insert
//   0x0086E5C0, vector<pair<long, CSkeletalMorph> >::erase 0x0086C720 / _M_fill_insert 0x0086CD70,
//   GFReadVectorMapMemDump<CTextureMorph, unsigned long, CKeyPairCompareLess<..> > 0x0086DEB0 (`__fastcall`),
//   CDefString::TransferBinaryIn 0x0044FC00.
// Shape: the shared CRandomAppearanceMorph / CArray / CVectorMap / STLport vector headers. STLport 4.6 resize(n) builds a default
//   element temporary and calls erase or _M_fill_insert; with _M_fill_insert's body visible (FABLE_RETAIL_STLP_VECTOR, as every
//   retail TU that includes _vector.c) VC7.1 knows the temporary's address is not kept, so the erase arm jumps past the
//   temporary's free test (0x0086C0CA `jmp 0x0086C109`).
// Header overlay: FABLE_HDR_OVERLAY=$A/hdr (engine/CRandomAppearanceMorph.h opt-in FABLE_RETAIL_CRANDOMAPPEARANCEMORPH_TRANSFERBINARYIN,
//   engine/CVectorMap.h opt-in FABLE_RETAIL_CVECTORMAP_SETDIRTY).
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#include "engine/CMemoryDataInputStream.h"
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#define FABLE_RETAIL_CVECTORMAP_CTOR
#define FABLE_RETAIL_CVECTORMAP_SETDIRTY
#define FABLE_RETAIL_CDEFSTRING_TRANSFER
#define FABLE_RETAIL_CRANDOMAPPEARANCEMORPH_TRANSFERBINARYIN
#include "engine/CRandomAppearanceMorph.h"

template<class V,class K,class C> void __fastcall GFReadVectorMapMemDump(CMemoryDataInputStream& stream, CVectorMap<K,V,C>& map);   // 0x0086DEB0

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