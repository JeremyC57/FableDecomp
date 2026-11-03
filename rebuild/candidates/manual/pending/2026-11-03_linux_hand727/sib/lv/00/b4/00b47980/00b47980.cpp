// CEngineLightingManager::FrameEnd(long frame) -- end of frame: give every light-lookup-grid link back to its object heap, clear the small-object grid, free every queued local and shadowed spot light to their heaps and empty both lists, then the primitive-manager base FrameEnd
// Leaf: FrameEnd@CEngineLightingManager
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B47980, 457 bytes (+int3 padding), `ret 4`; slot 2 of CEngineLightingManager's SECOND
//   vtable 0x012A223C (the primary one, 0x012A2274, is CEnginePrimitiveManager's).
// Callees: CDynamicObjectHeapBase::RefCountedRelease 0x00F36CA0 (x3, the inlined heap Free on the last allocation),
//   memmove 0x00BFEAE6 (x2, STLport __copy_trivial in clear()), CEnginePrimitiveManager::FrameEnd 0x00BACDA0 (an empty `ret`).
//   Heaps: the grid links 0x01436ED0, CManagerLocalLight CDynamicObjectHeap<72,16> 0x01436FB0 (as the landed
//   AddLightPrimitive 0x00B47FD0), CManagerShadowedSpotLight 0x01437450 (template sizes of the two others descriptive).
// Layout: the method overrides a virtual of the secondary base at +0x0c, so VC compiles it with `this` at that subobject and every
//   member offset 0x0c lower (the base call is `lea ecx,[ebx-0Ch]`). Ego_r.pdb / engine/CEngineLightingManager.h put
//   LocalLightList at +0xb0 and the grids at +0xf8/+0x4f8, but its CArray members are 16 bytes where retail's STLport vectors
//   are 12: ThemeChannels, LocalLightList and ShadowedSpotLightList shift the grids by 0x0c (LocalLightList +0xac,
//   ShadowedSpotLightList +0xb8, LightLookupGrid +0xec, SmallObjectLookupGrid +0x4ec in the full object) -- hence a local model.
// Shape: the grid walk is column-major (x outer with a pointer column, y inner stepping 0x40); each list is drained with a
//   `while (head)` loop that frees the head through the inline CDynamicObjectHeap::Free (dec AllocCount, push on FreeList,
//   release when it reaches 0 and ref-counted release is armed); the 64x64 grid is a memset (`rep stosd`); the light lists
//   are index loops on size() followed by the STLport clear().
extern "C" void* __cdecl memmove(void* dest, const void* source, unsigned int count);   // 0x00BFEAE6
extern "C" void* __cdecl memset(void* dest, int value, unsigned int count);
#pragma intrinsic(memset)

inline void* CopyTrivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result
        : (char*)memmove(result, first, (const char*)last - (const char*)first) + ((const char*)last - (const char*)first);
}

struct CObjectEntry { CObjectEntry* Next; };

struct CDynamicObjectHeapBase {
    virtual ~CDynamicObjectHeapBase();
    long                    AllocCount;            // +0x04
    long                    TotalAllocatorBlocks;  // +0x08
    bool                    RefCountReleaseEnabled;   // +0x0c
    CDynamicObjectHeapBase* HeapNext;              // +0x10
    void RefCountedRelease();                      // 0x00F36CA0
};

template <int SIZE, int COUNT> struct CDynamicObjectHeap : CDynamicObjectHeapBase {
    CObjectEntry* FreeList;                        // +0x14
    void Free(void* object)
    {
        --AllocCount;
        ((CObjectEntry*)object)->Next = FreeList;
        FreeList = (CObjectEntry*)object;
        if (AllocCount == 0 && RefCountReleaseEnabled)
            RefCountedRelease();
    }
};

extern CDynamicObjectHeap<12, 16> g_LightLinkHeap;          // 0x01436ED0
extern CDynamicObjectHeap<72, 16> g_LocalLightHeap;         // 0x01436FB0
extern CDynamicObjectHeap<56, 16> g_ShadowedSpotLightHeap;  // 0x01437450

struct CLocalLightElementLink {
    virtual void v00();
    void*                   Element;               // +0x04
    CLocalLightElementLink* Next;                  // +0x08
};
struct CManagerLocalLight;
struct CManagerShadowedSpotLight;
class CEngineLocalLightArray;

template <class T> struct CPointerVector {         // STLport vector<T*> (12 bytes)
    T** _M_start;
    T** _M_finish;
    T** _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    T*& operator[](unsigned int n) { return *(_M_start + n); }
    void clear()
    {
        T** i = (T**)CopyTrivial(_M_finish, _M_finish, _M_start);
        _M_finish = i;
    }
};

class CEnginePrimitiveManager { public:
    virtual void v00();
    void* Engine;                                  // +0x04
    bool  Enabled;                                 // +0x08
    void FrameEnd();                               // 0x00BACDA0
};
struct CEngineFrameListener {                      // secondary base at +0x0c
    virtual void v00();
    virtual void v01();
    virtual void FrameEnd(long frame);             // slot 2
    unsigned char _pad04[8];
};

class CEngineLightingManager : public CEnginePrimitiveManager, public CEngineFrameListener { public:
    unsigned char                             _pad18[0xac - 0x18];
    CPointerVector<CManagerLocalLight>        LocalLightList;          // +0xac
    CPointerVector<CManagerShadowedSpotLight> ShadowedSpotLightList;   // +0xb8
    unsigned char                             _padc4[0xec - 0xc4];
    CLocalLightElementLink*                   LightLookupGrid[16][16]; // +0xec
    CEngineLocalLightArray*                   SmallObjectLookupGrid[64][64];   // +0x4ec
    virtual void FrameEnd(long frame);
};

void CEngineLightingManager::FrameEnd(long frame)
{
    for (int x = 0; x < 16; ++x)
        for (int y = 0; y < 16; ++y)
            while (LightLookupGrid[y][x]) {
                CLocalLightElementLink* next = LightLookupGrid[y][x]->Next;
                g_LightLinkHeap.Free(LightLookupGrid[y][x]);
                LightLookupGrid[y][x] = next;
            }
    memset(SmallObjectLookupGrid, 0, sizeof(SmallObjectLookupGrid));
    for (unsigned int i = 0; i < LocalLightList.size(); ++i)
        g_LocalLightHeap.Free(LocalLightList[i]);
    LocalLightList.clear();
    for (unsigned int j = 0; j < ShadowedSpotLightList.size(); ++j)
        g_ShadowedSpotLightHeap.Free(ShadowedSpotLightList[j]);
    ShadowedSpotLightList.clear();
    CEnginePrimitiveManager::FrameEnd();
}