// Behaviour test for 00b480e0: src/00b480e0.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CEngineLightingManager::AddLightPrimitive(const CEngineInternalPrimitiveShadowedSpotLight&)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CEngineLightingManager::AddLightPrimitive(const CEngineInternalPrimitiveShadowedSpotLight&) -- queue a shadowed spot light for this frame: skipped when its radius or cone radius is below 0.1 or its colour is black (every channel below 1/255); otherwise a CManagerShadowedSpotLight from the object heap is appended to ShadowedSpotLightList
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B480E0, length 253 bytes (`ret 4`; called from RenderPrimitive 0x00B481E0 for type 16).
// Callees: CDynamicObjectHeap<56,16>::CreateMoreBlocks @ 0x00B49010 (on the heap @ 0x01437450), CManagerShadowedSpotLight::
// CManagerShadowedSpotLight(const C3DVector&, const C3DVector&, float, float, float, const CRGBFloatColour&) @ 0x00B493C0,
// vector<CManagerShadowedSpotLight*>::_M_insert_overflow @ 0x00B49ED0. Constants: 0.1f @ 0x012A20D0, 1/255 @ 0x01231724.
// Layout: PDB CEngineInternalPrimitiveShadowedSpotLight (WorldPos +0x48, Direction +0x54, Colour +0x60, Radius +0x70, SpotRadius
// +0x74, ConeRadius +0x78); the heap = PDB CDynamicObjectHeap<56,16> (AllocCount +4, FreeList +0x14, AllowHeapToGrow +0x39c;
// base as the landed CDynamicObjectHeapBase ctor 0x00F36C60); the retail ShadowedSpotLightList (12-byte STLport vector) is at
// +0xb8 (PDB +0xc0). No usable engine header for the manager at these offsets, so `this` is modelled locally.
// Shape: `x < k` early returns (`test ah,5 ; jnp`); the class operator new is the inlined heap Alloc (null when the heap may not
// grow) merged with new's null test; STLport push_back with the __true_type tag homed in the dead argument slot (the pushed
// pointer lives there too).
struct C3DVector { float X, Y, Z; };
struct CRGBFloatColour { float R, G, B, A; };

struct CObjectEntry { CObjectEntry* Next; };

struct CDynamicObjectHeapBase {
    virtual void v00();
    long                    AllocCount;            // +0x04
    long                    TotalAllocatorBlocks;  // +0x08
    bool                    RefCountReleaseEnabled;   // +0x0c
    CDynamicObjectHeapBase* HeapNext;              // +0x10
};

template <int SIZE, int COUNT> struct CDynamicObjectHeap : CDynamicObjectHeapBase {
    CObjectEntry* FreeList;                        // +0x14
    unsigned char StartBlock[4 + SIZE * COUNT];    // +0x18 CObjectAllocatorBlockEntry
    bool          AllowHeapToGrow;
    void CreateMoreBlocks();
    void* Alloc()                                  // the free-list test and the pop re-read FreeList (no entry local): the entry
    {                                              // stays in eax across the merge and is copied to ecx for the constructor
        if (!FreeList) {
            if (!AllowHeapToGrow)
                return 0;
            CreateMoreBlocks();
        }
        ++AllocCount;
        void* result = FreeList;
        FreeList = FreeList->Next;
        return result;
    }
};

struct CManagerShadowedSpotLight;
extern CDynamicObjectHeap<56, 16> g_ShadowedSpotLightHeap;   // 0x01437450 (CreateMoreBlocks 0x00B49010)

struct CManagerShadowedSpotLight {
    unsigned char _body[56];
    CManagerShadowedSpotLight(const C3DVector& position, const C3DVector& direction, float radius, float spotRadius,
                              float coneRadius, const CRGBFloatColour& colour);   // 0x00B493C0
    static void* operator new(unsigned int) { return g_ShadowedSpotLightHeap.Alloc(); }
    static void operator delete(void*) { }
};

struct CEngineInternalPrimitiveShadowedSpotLight {
    unsigned char   _base[0x48];
    C3DVector       WorldPos;                      // +0x48
    C3DVector       Direction;                     // +0x54
    CRGBFloatColour Colour;                        // +0x60
    float           Radius;                        // +0x70
    float           SpotRadius;                    // +0x74
    float           ConeRadius;                    // +0x78
};

inline void* operator new(unsigned int, void* where) { return where; }
struct __true_type { __true_type() { } };

template <class T> struct CPointerVector {         // STLport vector<T*> (12 bytes)
    T** _M_start;
    T** _M_finish;
    T** _M_end_of_storage;
    void _M_insert_overflow(T** position, T* const& x, const __true_type&, unsigned int fill, bool atEnd);
    void push_back(T* const& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T*(x);
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
__declspec(noinline) void CPointerVector<T>::_M_insert_overflow(T** position, T* const& x, const __true_type& p2, unsigned int fill, bool atEnd)
{
    T** p = (T**)GrowVectorStorage_s7(this, fill);
    if (p)
        new (p) T*(x);
}
#pragma auto_inline(on)

struct CEngineLightingManager {
    unsigned char                             _pad[0xb8];
    CPointerVector<CManagerShadowedSpotLight> ShadowedSpotLightList;   // +0xb8
    void AddLightPrimitive(const CEngineInternalPrimitiveShadowedSpotLight& light);
};

void CEngineLightingManager::AddLightPrimitive(const CEngineInternalPrimitiveShadowedSpotLight& light)
{
    if (light.Radius < 0.1f)
        return;
    if (light.ConeRadius < 0.1f)
        return;
    if (light.Colour.R < 1.0f / 255.0f && light.Colour.G < 1.0f / 255.0f && light.Colour.B < 1.0f / 255.0f)
        return;
    ShadowedSpotLightList.push_back(new CManagerShadowedSpotLight(light.WorldPos, light.Direction, light.Radius,
                                                                  light.SpotRadius, light.ConeRadius, light.Colour));
}

// ---- stubs and driver ----
void CDynamicObjectHeapBase::v00() {}
CDynamicObjectHeap<56, 16> g_ShadowedSpotLightHeap;
static unsigned char g_blocks[4][56]; static int g_more = 0;
template <> void CDynamicObjectHeap<56, 16>::CreateMoreBlocks()
{
    ++g_more;
    CObjectEntry* a = (CObjectEntry*)g_blocks[2]; CObjectEntry* b = (CObjectEntry*)g_blocks[3];
    a->Next = b; b->Next = 0; FreeList = a;
}
static int g_ctor = 0; static CManagerShadowedSpotLight* g_this; static const C3DVector* g_pos; static const C3DVector* g_dir;
static float g_rad, g_spot, g_cone; static const CRGBFloatColour* g_col;
CManagerShadowedSpotLight::CManagerShadowedSpotLight(const C3DVector& position, const C3DVector& direction, float radius, float spotRadius,
                                                     float coneRadius, const CRGBFloatColour& colour)
{ ++g_ctor; g_this = this; g_pos = &position; g_dir = &direction; g_rad = radius; g_spot = spotRadius; g_cone = coneRadius; g_col = &colour; }
static CManagerShadowedSpotLight* g_store[8]; static int g_grow = 0;
void* GrowVectorStorage_s7(void* vector, unsigned long count)
{ ++g_grow; void*** v = (void***)vector; void** p = v[1]; v[1] += count; if (v[2] < v[1]) v[2] = v[1]; return p; }
int main()
{
    static CEngineLightingManager mgr; static unsigned char lightStore[sizeof(CEngineInternalPrimitiveShadowedSpotLight)];
    CEngineInternalPrimitiveShadowedSpotLight& light = *(CEngineInternalPrimitiveShadowedSpotLight*)lightStore;
    CPointerVector<CManagerShadowedSpotLight>& list = mgr.ShadowedSpotLightList;
    list._M_start = list._M_finish = g_store; list._M_end_of_storage = g_store + 1;
    CObjectEntry* e0 = (CObjectEntry*)g_blocks[0]; CObjectEntry* e1 = (CObjectEntry*)g_blocks[1]; e0->Next = e1; e1->Next = 0;
    g_ShadowedSpotLightHeap.FreeList = e0; g_ShadowedSpotLightHeap.AllocCount = 0; g_ShadowedSpotLightHeap.AllowHeapToGrow = false;
    light.Radius = 0.05f; light.ConeRadius = 1.0f; light.SpotRadius = 2.0f; light.Colour.R = light.Colour.G = light.Colour.B = 1.0f;
    mgr.AddLightPrimitive(light); CHECK(g_ctor == 0);                                   // radius too small
    light.Radius = 4.0f; light.ConeRadius = 0.01f;
    mgr.AddLightPrimitive(light); CHECK(g_ctor == 0);                                   // cone too small
    light.ConeRadius = 1.0f; light.Colour.R = light.Colour.G = light.Colour.B = 0.0f;
    mgr.AddLightPrimitive(light); CHECK(g_ctor == 0);                                   // black
    light.Colour.B = 0.5f;
    mgr.AddLightPrimitive(light);
    CHECK(g_ctor == 1); CHECK(g_this == (CManagerShadowedSpotLight*)e0); CHECK(g_pos == &light.WorldPos); CHECK(g_dir == &light.Direction);
    CHECK(g_rad == 4.0f); CHECK(g_spot == 2.0f); CHECK(g_cone == 1.0f); CHECK(g_col == &light.Colour);
    CHECK(g_store[0] == (CManagerShadowedSpotLight*)e0); CHECK(g_grow == 0); CHECK(g_ShadowedSpotLightHeap.AllocCount == 1); CHECK(g_ShadowedSpotLightHeap.FreeList == e1);
    light.Colour.B = 0.0f; light.Colour.R = 0.25f;
    mgr.AddLightPrimitive(light); CHECK(g_grow == 1); CHECK(g_store[1] == (CManagerShadowedSpotLight*)e1); CHECK(g_ctor == 2);
    mgr.AddLightPrimitive(light); CHECK(g_ctor == 2); CHECK(g_store[2] == 0); CHECK(g_more == 0);   // empty heap, no growth
    g_ShadowedSpotLightHeap.AllowHeapToGrow = true;
    mgr.AddLightPrimitive(light); CHECK(g_more == 1); CHECK(g_store[3] == (CManagerShadowedSpotLight*)g_blocks[2]); CHECK(g_ctor == 3);
    CHECK(g_ShadowedSpotLightHeap.AllocCount == 3); CHECK(g_ShadowedSpotLightHeap.FreeList == (CObjectEntry*)g_blocks[3]);
    if (fails == 0) printf("00b480e0_TEST PASS\n"); else printf("00b480e0_TEST FAIL\n");
    return fails;
}