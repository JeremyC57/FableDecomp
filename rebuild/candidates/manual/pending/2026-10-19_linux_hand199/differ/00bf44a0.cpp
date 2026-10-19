// CEngineLandscapePatch::RenderForegroundWater() -- queue the patch's water mesh on the water renderer's foreground list for its water type: reflective sea (4), non-reflective sea (5), ice (8, paired with the patch's bounding box), anything else on the plain foreground list
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BF44A0, length 208 bytes (plain `ret`).
// Grade: DIFFER(210v208) -- the local copy of WaterPatch is stored before the type load (retail after `cmp eax,4`), and the
//        box copy goes through `add ecx,20h` + [ecx+N] (retail displaced [ecx+20h..34h] loads)
// Callees: the CArray<CWaterPatchMesh*> push_back @ 0x00BF5EA0 (the out-of-line copy, used for the default list) and its
// reallocating insert @ 0x00BF5DB0 (STLport _M_insert_overflow, inside the two inlined push_backs); the
// CArray<pair<C3DBoundingBox, CWaterPatchMesh*> > push_back @ 0x00BF5FE0 (manifest label "OnInitLeaf" is a propagation).
// Globals: the engine water renderer @ 0x01436E54.
// Layout (retail; PDB offsets less 4 per earlier CArray): CEngineWaterRenderer ForegroundRenderList +0x1FC,
// IceForegroundRenderList +0x220, SeaReflectiveForegroundRenderList +0x244, SeaNonReflectiveForegroundRenderList +0x250;
// CEngineLandscapePatch WaterPatch +0x14, BoundingBox +0x20 (PDB); CWaterPatchMesh WaterType +0x1C.
// Shape: the mesh is a named local (addressable for push_back's reference); VC inlines push_back for the first two
// sites (the reflective/non-reflective bodies tail-merge, case 5 only reloads the list address) and calls the
// out-of-line copy for the third; the ice pair is a 0x1C-byte local built with dword copies of the box.
#include <new>

struct C3DVector { float X, Y, Z; };
struct C3DBoundingBox { C3DVector MinCoord, MaxCoord; };

enum EWaterType { WATER_TYPE_SEA_REFLECTIVE = 4, WATER_TYPE_SEA_NON_REFLECTIVE = 5, WATER_TYPE_ICE = 8 };

struct CWaterPatchMesh {
    unsigned char _pad0[0x1c];
    EWaterType    WaterType;                    // +0x1c
};

struct SScalarTag { SScalarTag() { } };

struct CWaterPatchMeshArray {                   // CArray<CWaterPatchMesh*> (STLport vector, 12 bytes)
    CWaterPatchMesh** _M_start;
    CWaterPatchMesh** _M_finish;
    CWaterPatchMesh** _M_end_of_storage;
    void _M_insert_overflow(CWaterPatchMesh** position, CWaterPatchMesh* const& value, const SScalarTag& tag,
                            unsigned long count, bool atEnd);   // 0x00BF5DB0
    void Add(CWaterPatchMesh* const& value);    // 0x00BF5EA0 (the out-of-line push_back body)
    __forceinline void push_back(CWaterPatchMesh* const& value)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CWaterPatchMesh*(value);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, value, SScalarTag(), 1, true);
    }
};

struct SBoxedWaterPatch {                       // std::pair<C3DBoundingBox, CWaterPatchMesh*>
    C3DBoundingBox   first;
    CWaterPatchMesh* second;
};

struct CBoxedWaterPatchArray {                  // CArray<pair<C3DBoundingBox, CWaterPatchMesh*> >
    SBoxedWaterPatch* _M_start;
    SBoxedWaterPatch* _M_finish;
    SBoxedWaterPatch* _M_end_of_storage;
    void push_back(const SBoxedWaterPatch& value);   // 0x00BF5FE0
};

struct CEngineWaterRenderer {
    unsigned char         _pad000[0x1fc];
    CWaterPatchMeshArray  ForegroundRenderList;                 // +0x1fc
    unsigned char         _pad208[0x220 - 0x208];
    CBoxedWaterPatchArray IceForegroundRenderList;              // +0x220
    unsigned char         _pad22c[0x244 - 0x22c];
    CWaterPatchMeshArray  SeaReflectiveForegroundRenderList;    // +0x244
    CWaterPatchMeshArray  SeaNonReflectiveForegroundRenderList; // +0x250
};

extern CEngineWaterRenderer* g_pEngineWaterRenderer;   // 0x01436E54

struct CEngineLandscapePatch {
    void*            __vftable;
    void*            Map;                       // +0x04
    void*            LayerMeshList;             // +0x08
    unsigned short*  FastAccessTextureIDs;      // +0x0c
    unsigned short   FastAccessTextureIDCount;  // +0x10
    CWaterPatchMesh* WaterPatch;                // +0x14
    long             FilePos;                   // +0x18
    long             FileDataSize;              // +0x1c
    C3DBoundingBox   BoundingBox;               // +0x20
    void RenderForegroundWater();
};

void CEngineLandscapePatch::RenderForegroundWater()
{
    if (WaterPatch) {
        CWaterPatchMesh* water = WaterPatch;
        if (water->WaterType == WATER_TYPE_SEA_REFLECTIVE)
            g_pEngineWaterRenderer->SeaReflectiveForegroundRenderList.push_back(water);
        else if (water->WaterType == WATER_TYPE_SEA_NON_REFLECTIVE)
            g_pEngineWaterRenderer->SeaNonReflectiveForegroundRenderList.push_back(water);
        else if (water->WaterType == WATER_TYPE_ICE) {
            SBoxedWaterPatch entry;
            entry.first = BoundingBox;
            entry.second = water;
            g_pEngineWaterRenderer->IceForegroundRenderList.push_back(entry);
        }
        else
            g_pEngineWaterRenderer->ForegroundRenderList.Add(water);
    }
}
