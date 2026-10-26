// Behaviour test for 00bf44a0: src/00bf44a0.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CEngineLandscapePatch::RenderForegroundWater()
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CEngineLandscapePatch::RenderForegroundWater() -- queue the patch's water mesh with the water renderer: type 4 and type 5 meshes into their own lists, type 8 together with the patch bounding box, anything else into the general list
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

class CWaterPatchMesh { public:
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
void* GrowVectorStorage_s7(void* vector, unsigned long count);

#pragma auto_inline(off)
// Same-TU body of the vector growth member (_M_insert_overflow, instantiated in retail's TU): the new storage receives the value
// read through its const reference; the empty tag is never touched.
template <class T>
__declspec(noinline) void CRetailVector<T>::_M_insert_overflow(T* position, const T& x, const __true_type& p2, unsigned int fill, bool atEnd)
{
    T* p = (T*)GrowVectorStorage_s7(this, fill);
    if (p)
        *p = x;
}
template <class T>
__declspec(noinline) void CRetailVector<T>::push_back(const T& x)
{
    if (_M_finish != _M_end_of_storage) {
        new (_M_finish) T(x);
        ++_M_finish;
    } else
        _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
}
#pragma auto_inline(on)

class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
class C3DBoundingBox { public: C3DVector Min, Max; };

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

class CEngineWaterRenderer { public:
    unsigned char                  _pad000[0x1fc];
    CRetailVector<CWaterPatchMesh*> GeneralPatches;    // +0x1fc
    unsigned char                  _pad208[0x220 - 0x208];
    CBoxedWaterPatchVector         BoxedPatches;       // +0x220
    unsigned char                  _pad22c[0x244 - 0x22c];
    CRetailVector<CWaterPatchMesh*> Type4Patches;      // +0x244
    CRetailVector<CWaterPatchMesh*> Type5Patches;      // +0x250
};
extern CEngineWaterRenderer* g_pEngineWaterRenderer;
void* GrowBoxedStorage_s7(void* vector, unsigned long count);
#pragma auto_inline(off)
__declspec(noinline) void CBoxedWaterPatchVector::push_back(const SBoxedWaterPatch& x)
{
    if (_M_finish != _M_end_of_storage) {
        new (_M_finish) SBoxedWaterPatch(x);
        ++_M_finish;
    } else {
        SBoxedWaterPatch* p = (SBoxedWaterPatch*)GrowBoxedStorage_s7(this, 1);
        if (p)
            new (p) SBoxedWaterPatch(x);
    }
}
#pragma auto_inline(on)
   // 0x01436E54

class CEngineLandscapePatch { public:
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

// ---- stubs and driver ----
static int g_grow = 0, g_growBox = 0;
static CEngineWaterRenderer* g_r;
CEngineWaterRenderer* g_pEngineWaterRenderer = 0;
void* GrowVectorStorage_s7(void* storage, unsigned long count)
{
    ++g_grow; CRetailVector<CWaterPatchMesh*>* v = (CRetailVector<CWaterPatchMesh*>*)storage; CWaterPatchMesh** p = v->_M_finish; v->_M_finish += count; if (v->_M_end_of_storage < v->_M_finish) v->_M_end_of_storage = v->_M_finish; return p;
}
void* GrowBoxedStorage_s7(void* storage, unsigned long count)
{
    ++g_growBox; CBoxedWaterPatchVector* v = (CBoxedWaterPatchVector*)storage; SBoxedWaterPatch* p = v->_M_finish; v->_M_finish += count; if (v->_M_end_of_storage < v->_M_finish) v->_M_end_of_storage = v->_M_finish; return p;
}
static CWaterPatchMesh* g_gen[4]; static CWaterPatchMesh* g_t4[4]; static CWaterPatchMesh* g_t5[4]; static unsigned char g_boxRaw[4 * sizeof(SBoxedWaterPatch)];
int main()
{
    static CEngineWaterRenderer r; g_pEngineWaterRenderer = &r;
    SBoxedWaterPatch* boxes = (SBoxedWaterPatch*)g_boxRaw;
    r.GeneralPatches._M_start = r.GeneralPatches._M_finish = g_gen; r.GeneralPatches._M_end_of_storage = g_gen + 1;
    r.Type4Patches._M_start = r.Type4Patches._M_finish = g_t4; r.Type4Patches._M_end_of_storage = g_t4 + 1;
    r.Type5Patches._M_start = r.Type5Patches._M_finish = g_t5; r.Type5Patches._M_end_of_storage = g_t5 + 4;
    r.BoxedPatches._M_start = r.BoxedPatches._M_finish = boxes; r.BoxedPatches._M_end_of_storage = boxes + 1;
    CWaterPatchMesh m4, m5, m8, m1; m4.Type = 4; m5.Type = 5; m8.Type = 8; m1.Type = 1;
    static unsigned char praw[sizeof(CEngineLandscapePatch)]; CEngineLandscapePatch& p = *(CEngineLandscapePatch*)praw;
    p.BoundingBox.Min.X = 1; p.BoundingBox.Min.Y = 2; p.BoundingBox.Min.Z = 3; p.BoundingBox.Max.X = 4; p.BoundingBox.Max.Y = 5; p.BoundingBox.Max.Z = 6;
    p.WaterPatch = 0; p.RenderForegroundWater(); CHECK(r.GeneralPatches._M_finish == g_gen && r.Type4Patches._M_finish == g_t4);
    p.WaterPatch = &m4; p.RenderForegroundWater(); CHECK(r.Type4Patches._M_finish == g_t4 + 1 && g_t4[0] == &m4 && g_grow == 0);
    p.RenderForegroundWater(); CHECK(r.Type4Patches._M_finish == g_t4 + 2 && g_t4[1] == &m4 && g_grow == 1);
    p.WaterPatch = &m5; p.RenderForegroundWater(); CHECK(r.Type5Patches._M_finish == g_t5 + 1 && g_t5[0] == &m5 && g_grow == 1);
    p.WaterPatch = &m1; p.RenderForegroundWater(); p.RenderForegroundWater(); CHECK(r.GeneralPatches._M_finish == g_gen + 2 && g_gen[0] == &m1 && g_gen[1] == &m1 && g_grow == 2);
    p.WaterPatch = &m8; p.RenderForegroundWater(); p.RenderForegroundWater();
    CHECK(r.BoxedPatches._M_finish == boxes + 2 && g_growBox == 1);
    CHECK(boxes[0].Mesh == &m8 && boxes[0].Box.Min.X == 1 && boxes[0].Box.Max.Z == 6 && boxes[1].Mesh == &m8 && boxes[1].Box.Min.Y == 2 && boxes[1].Box.Max.X == 4);
    CHECK(r.GeneralPatches._M_finish == g_gen + 2 && r.Type4Patches._M_finish == g_t4 + 2 && r.Type5Patches._M_finish == g_t5 + 1);
    if (fails == 0) printf("00bf44a0_TEST PASS\n"); else printf("00bf44a0_TEST FAIL\n");
    return fails;
}