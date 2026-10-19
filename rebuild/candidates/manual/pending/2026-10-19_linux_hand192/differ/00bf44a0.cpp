// CEngineLandscapePatch::RenderForegroundWater() -- queue the patch's water mesh with the water renderer: type 4 and type 5 meshes into their own lists, type 8 together with the patch bounding box, anything else into the general list
// Grade: DIFFER(208v208) -- same size: a __forceinline copy of push_back at the type 4/5 sites (default list keeps the out-of-line 0x00BF5EA0 call), a __forceinline SBoxedWaterPatch ctor and the null test on the member before the local copy fix everything but one store: the water local is spilled to its frame word before the type load/compare, retail after the cmp
// Leaf: RenderForegroundWater
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BF44A0, 208 bytes.
// Callees: the water renderer (global 0x01436E54) lists: vector<CWaterPatchMesh*>::push_back(const T&) 0x00BF5EA0 (out of
//   line) and its _M_insert_overflow 0x00BF5DB0, vector<SBoxedWaterPatch>::push_back(const T&) 0x00BF5FE0 (manifest label
//   "CScriptModuleTransitionBetweenPoints::OnInitLeaf" is propagated).
// Layout (Ego_r.pdb CEngineLandscapePatch, retail agrees): WaterPatch +0x14, BoundingBox +0x20 (C3DBoundingBox, 24 bytes);
//   CWaterPatchMesh type at +0x1c. The water renderer's lists (descriptive names, STLport 12-byte vectors): +0x1fc general,
//   +0x220 boxed (type 8), +0x244 type 4, +0x250 type 5.
// Shape: a switch on the type; the in-class push_back is inlined at its first call(s) (types 4 and 5, cross-jumped into one
//   tail) and called out of line for the default; the water pointer is a named local (its address is the push_back
//   argument); the type-8 record {box, mesh} is built in the frame and passed by reference.
inline void* operator new(unsigned int, void* where) { return where; }
struct __true_type { __true_type() { } };

struct CWaterPatchMesh {
    unsigned char _pad[0x1c];
    long          Type;                                // +0x1c
};

template <class T> struct CRetailVector {              // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned int fill, bool atEnd);
    void push_back(const T& x);                       // 0x00BF5EA0 (the out-of-line copy)
    __forceinline void push_back_inline(const T& x)   // the same body, inlined at the first two call sites
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};

struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct C3DBoundingBox { C3DVector Min, Max; };

struct SBoxedWaterPatch {                              // 28 bytes
    C3DBoundingBox   Box;
    CWaterPatchMesh* Mesh;
    __forceinline SBoxedWaterPatch(const C3DBoundingBox& box, CWaterPatchMesh* mesh) : Box(box), Mesh(mesh) { }
};
struct CBoxedWaterPatchVector {
    SBoxedWaterPatch* _M_start;
    SBoxedWaterPatch* _M_finish;
    SBoxedWaterPatch* _M_end_of_storage;
    void push_back(const SBoxedWaterPatch& x);         // 0x00BF5FE0
};

struct CEngineWaterRenderer {
    unsigned char                  _pad000[0x1fc];
    CRetailVector<CWaterPatchMesh*> GeneralPatches;    // +0x1fc
    unsigned char                  _pad208[0x220 - 0x208];
    CBoxedWaterPatchVector         BoxedPatches;       // +0x220
    unsigned char                  _pad22c[0x244 - 0x22c];
    CRetailVector<CWaterPatchMesh*> Type4Patches;      // +0x244
    CRetailVector<CWaterPatchMesh*> Type5Patches;      // +0x250
};
extern CEngineWaterRenderer* g_pEngineWaterRenderer;   // 0x01436E54

struct CEngineLandscapePatch {
    void*            __vftable;                        // +0x00
    void*            Map;                              // +0x04
    void*            LayerMeshList;                    // +0x08
    unsigned short*  FastAccessTextureIDs;             // +0x0c
    unsigned short   FastAccessTextureIDCount;         // +0x10
    CWaterPatchMesh* WaterPatch;                       // +0x14
    long             FilePos;                          // +0x18
    long             FileDataSize;                     // +0x1c
    C3DBoundingBox   BoundingBox;                      // +0x20
    void RenderForegroundWater();
};

void CEngineLandscapePatch::RenderForegroundWater()
{
    if (!WaterPatch)
        return;
    CWaterPatchMesh* water = WaterPatch;
    long type = water->Type;
    if (type == 4)
        g_pEngineWaterRenderer->Type4Patches.push_back_inline(water);
    else if (type == 5)
        g_pEngineWaterRenderer->Type5Patches.push_back_inline(water);
    else if (type == 8)
        g_pEngineWaterRenderer->BoxedPatches.push_back(SBoxedWaterPatch(BoundingBox, water));
    else
        g_pEngineWaterRenderer->GeneralPatches.push_back(water);
}
