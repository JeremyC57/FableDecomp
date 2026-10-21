// CEngineLightingManager::AddLightPrimitive(const CEngineInternalPrimitiveLight&) -- queue a point light for this frame: skipped when its radius is below 0.1 or its colour is black (every channel below 1/255); otherwise a CManagerLocalLight from the object heap (its CEngineLocalLightElement built from the primitive, not yet found) is appended to LocalLightList
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B47FD0, length 260 bytes (`ret 4`; called from RenderPrimitive 0x00B481E0 for type 15).
// Callees: CDynamicObjectHeap<72,16>::CreateMoreBlocks @ 0x00B48F90 (on the heap @ 0x01436FB0), CEngineLocalLightElement::
// CEngineLocalLightElement(const C3DVector&, float, float, const CRGBFloatColour&, unsigned long) @ 0x00B44E30,
// vector<CManagerLocalLight*>::_M_insert_overflow @ 0x00B49DF0. Vtable 0x012A2228 = CManagerLocalLight. Constants: 0.1f @ 0x012A20D0,
// 1/255 @ 0x01231724.
// Layout: PDB CEngineInternalPrimitiveLight (WorldPos +0x48, Colour +0x54, Radius +0x64, SpotRadius +0x68, ChannelMask +0x6c),
// PDB CManagerLocalLight (0x48 bytes: LightElement +4, AlreadyFoundFlag +0x44); the heap = PDB CDynamicObjectHeap<72,16>
// (AllocCount +4, FreeList +0x14, AllowHeapToGrow +0x49c); the retail LocalLightList (12-byte STLport vector) is at +0xac
// (PDB +0xb0). `this` is modelled locally.
// Shape: as the spot-light twin 0x00B480E0 (parked): `x < k` early returns, the inlined heap Alloc merged with new's null test,
// an inline CManagerLocalLight constructor (vtable, element, flag) whose by-value floats are homed in the frame / dead
// argument slot, STLport push_back with the __true_type tag.
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

extern CDynamicObjectHeap<72, 16> g_LocalLightHeap;   // 0x01436FB0 (CreateMoreBlocks 0x00B48F90)

struct CEngineLocalLightElement {
    unsigned char _body[52];
    CEngineLocalLightElement(const C3DVector& position, float radius, float spotRadius, const CRGBFloatColour& colour,
                             unsigned long channelMask);   // 0x00B44E30
};

struct CManagerLocalLight {                        // vtable 0x012A2228, 0x48 bytes
    CEngineLocalLightElement LightElement;         // +0x04
    CManagerLocalLight*      NextLocalLight;       // +0x38
    float                    ClippingScore;        // +0x3c
    float                    ThemeWeight;          // +0x40
    bool                     AlreadyFoundFlag;     // +0x44
    virtual void v00();
    CManagerLocalLight(const C3DVector& position, float radius, float spotRadius, const CRGBFloatColour& colour,
                       unsigned long channelMask)
        : LightElement(position, radius, spotRadius, colour, channelMask), AlreadyFoundFlag(false) { }
    static void* operator new(unsigned int) { return g_LocalLightHeap.Alloc(); }
    static void operator delete(void*) { }
};

struct CEngineInternalPrimitiveLight {
    unsigned char   _base[0x48];
    C3DVector       WorldPos;                      // +0x48
    CRGBFloatColour Colour;                        // +0x54
    float           Radius;                        // +0x64
    float           SpotRadius;                    // +0x68
    unsigned long   ChannelMask;                   // +0x6c
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
    unsigned char                      _pad[0xac];
    CPointerVector<CManagerLocalLight> LocalLightList;   // +0xac
    void AddLightPrimitive(const CEngineInternalPrimitiveLight& light);
};

void CEngineLightingManager::AddLightPrimitive(const CEngineInternalPrimitiveLight& light)
{
    if (light.Radius < 0.1f)
        return;
    if (light.Colour.R < 1.0f / 255.0f && light.Colour.G < 1.0f / 255.0f && light.Colour.B < 1.0f / 255.0f)
        return;
    LocalLightList.push_back(new CManagerLocalLight(light.WorldPos, light.Radius, light.SpotRadius, light.Colour,
                                                    light.ChannelMask));
}