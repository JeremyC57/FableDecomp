#ifndef FABLE_ENGINE_CTCAPPEARANCEMORPHBASE_H
#define FABLE_ENGINE_CTCAPPEARANCEMORPHBASE_H
// New header (wfE11, row 0x004A3740 CWorld::UpdateRegionLoad). Retail opt-in only: FABLE_RETAIL_CTCAPPEARANCEMORPHBASE.
// Class: Ego_r.pdb CTCAppearanceMorphBase : CTCBase; interface TCI_APPEARANCE_MORPH (3: the hero's component looked up at 0x004A4FFF..0x004A502D).
// Method: Ego_r.pdb ?UploadTextureMorphsToEngine@CTCAppearanceMorphBase@@QAEXAAVCIEngine@@PBV?$set@JU?$less@J@std@@V?$allocator@J@2@@std@@_NM@Z
// (0x006CA7F0, called with Get3DEngine(), 0, true, 3.0f). Members are not modelled.
#ifdef FABLE_RETAIL_CTCAPPEARANCEMORPHBASE
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "rebuild_abi.h"
#include "engine/CTCBase.h"
#include "stlport/fable_containers.h"
#ifdef FABLE_RETAIL_CTCAPPEARANCEMORPHBASE_MESHMORPHSET
#include "engine/CEngineTextureMorph.h"
#endif
class CIEngine;
class CTCAppearanceMorphBase : public CTCBase {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_APPEARANCE_MORPH; }
    void UploadTextureMorphsToEngine(CIEngine& engine, const std::set<long>* morphs, bool, float);   // 0x006CA7F0
#ifdef FABLE_RETAIL_CTCAPPEARANCEMORPHBASE_MESHMORPHSET
    // NF62 opt-in (2026-11-17, rows 0x00722270 ??_G / 0x007222B0 CCountedPointer<..>::DeleteData): the nested Ego_r.pdb classes
    // CTCAppearanceMorphBase::CMeshMorphSet (MeshParts +0, OriginalMeshTextureMorphs +0x0C; retail 0x18, Ego_r 0x20) and
    // CMeshMorphSet::CMeshMorphSetParts (MeshIndex +0, TextureMorphs +4; retail 0x10, Ego_r 0x14), with the user destructor
    // ??1CMeshMorphSet@CTCAppearanceMorphBase@@QAE@XZ (Ego_r.pdb) inline. Needs FABLE_RETAIL_CENGINETEXTUREMORPHSET (engine/CEngineTextureMorph.h).
    class CMeshMorphSet {
    public:
        class CMeshMorphSetParts {
        public:
            unsigned long MeshIndex;                          // +0x00
            CEngineTextureMorphSet TextureMorphs;             // +0x04
        };
        ~CMeshMorphSet() { }
        CArray<CMeshMorphSetParts> MeshParts;                 // +0x00
        CEngineTextureMorphSet OriginalMeshTextureMorphs;     // +0x0C
    };
#endif
};
#endif
#endif // FABLE_ENGINE_CTCAPPEARANCEMORPHBASE_H
