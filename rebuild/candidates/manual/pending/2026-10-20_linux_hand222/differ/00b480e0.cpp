// Grade: DIFFER(258v255) -- lane gap: retail homes the new light pointer AND the empty __true_type tag in the dead `light` argument slot (both leas = E+4, no frame); the lane gives the pointer its own `push ecx` frame slot. Also the heap entry is coloured eax then copied to ecx (`mov ecx,eax ; mov eax,[eax]`) where the lane loads it straight into ecx.
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
    void* Alloc()
    {
        CObjectEntry* entry = FreeList;
        if (entry == 0) {
            if (!AllowHeapToGrow)
                return 0;
            CreateMoreBlocks();
            entry = FreeList;
        }
        ++AllocCount;
        FreeList = entry->Next;
        return entry;
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
