// CTCAppearanceMorphBase::UploadTextureMorphsToEngine(CIEngine& engine, const std::set<long>* ids, bool flag, float amount) -- when texture morphs are enabled globally: refresh the morphs (virtual slot 28) and hand the mesh morph set's original texture morphs to the engine (slot 60) with the flag, amount and id set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CA7F0, length 59 bytes.
// Leaf: UploadTextureMorphsToEngine
// Callees: this class's virtual slots 28 (+0x70, result unused) and 29 (+0x74, GetMeshTextureMorphs -- Ego_r.pdb
// `?GetMeshTextureMorphs@CTCRandomAppearanceMorph@@UBEPAVCMeshMorphSet@CTCAppearanceMorphBase@@XZ`), the engine's virtual
// slot 60 (+0xf0). The global flag byte @ 0x0137573E (initialised 1). Ego_r.pdb: `?UploadTextureMorphsToEngine@CTCAppearanceMorphBase
// @@QAEXAAVCIEngine@@PBV?$set@J...@std@@_NM@Z` (`ret 10h`); CMeshMorphSet = CArray<CMeshMorphSetParts> MeshParts (12 bytes in
// retail) + CEngineTextureMorphSet OriginalMeshTextureMorphs +0x0c.
// Shape: the morph set is fetched before the engine and its vtable are loaded, then the arguments are pushed through esi:
// the texture-morph set is a NAMED REFERENCE local (`add eax,0Ch` before the last argument load). A pointer local to the
// morph set puts the `add` after the last push; the call inline in the argument list pushes everything first.
// engine/CIEngine.h is a flattened PDB POD (no virtuals), so the engine interface is modelled locally.
struct CLongSet;                               // std::set<long>

struct CEngineTextureMorphSet { unsigned char _body[4]; };

struct CMeshMorphSet {
    unsigned char _meshParts[0xc];             // +0x00 CArray<CMeshMorphSetParts> (12-byte retail CArray)
    CEngineTextureMorphSet OriginalMeshTextureMorphs;   // +0x0c
};

struct CIEngine {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void UploadTextureMorphs(const CEngineTextureMorphSet& morphs, bool flag, float amount, const CLongSet* ids);   // slot 60 (+0xf0)
};

extern bool g_bTextureMorphsEnabled;           // 0x0137573E

struct CTCAppearanceMorphBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void UpdateMorphs();               // slot 28 (+0x70)
    virtual CMeshMorphSet* GetMeshTextureMorphs() const;   // slot 29 (+0x74)
    void UploadTextureMorphsToEngine(CIEngine& engine, const CLongSet* ids, bool flag, float amount);
};

void CTCAppearanceMorphBase::UploadTextureMorphsToEngine(CIEngine& engine, const CLongSet* ids, bool flag, float amount)
{
    if (g_bTextureMorphsEnabled) {
        UpdateMorphs();
        const CEngineTextureMorphSet& morphs = GetMeshTextureMorphs()->OriginalMeshTextureMorphs;
        engine.UploadTextureMorphs(morphs, flag, amount, ids);
    }
}
