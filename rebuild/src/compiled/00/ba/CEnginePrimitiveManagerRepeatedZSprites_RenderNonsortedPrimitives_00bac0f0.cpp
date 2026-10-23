// CEnginePrimitiveManagerRepeatedZSprites::RenderNonsortedPrimitives() -- lock the billboard and z-buffer scratch textures, order the sort list (gap pass: by mesh, then by depth), render every entry, empty the list, release the textures and flush the lighting manager's pending fog/colour state
// Leaf: RenderNonsortedPrimitives
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BAC0F0, length 560 bytes (body 0x229 + int3 padding), plain `ret`.
// Callees: CGPUScratchBufferTextureManager::StartSurfaceUse 0x00F36EE0 / 0x00F36F10 and the matching ends 0x00F36F00 / 0x00F36F30
//   on g_pDisplayManager(0x01436E98)+0x3190, RenderFromSortList 0x00BAA870, memmove 0x00BFEAE6, vector _M_fill_insert 0x00BAC800.
// Layout: local model -- the retail sort list is a 12-byte STLport vector (the PDB CArray is 0x10), so every later member sits 4
//   bytes lower than engine/CEnginePrimitiveManagerRepeatedZSprites.h (BillboardTextureHandle +0xa4, ZBufferTextureHandle +0xa8,
//   ControlPointWeightingPower/count +0xc0).
#include <string.h>

struct CEngineInternalPrimitiveRepeatedZSprite {
    unsigned char _pad00[0x50];
    unsigned long MeshKey;                         // +0x50
    unsigned char _pad54[0x8c - 0x54];
    float         Depth;                           // +0x8c
};
typedef CEngineInternalPrimitiveRepeatedZSprite* PSprite;
static __forceinline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
        : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) + ((const char*)__last - (const char*)__first);
}
struct CSortList {                                 // STLport vector<CEngineInternalPrimitiveRepeatedZSprite*>
    PSprite* _M_start;
    PSprite* _M_finish;
    PSprite* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    PSprite& operator[](unsigned long i) { return _M_start[i]; }
    void _M_fill_insert(PSprite* position, unsigned long count, const PSprite& value);   // 0x00BAC800
    __forceinline PSprite* erase(PSprite* first, PSprite* last)
    {
        PSprite* i = (PSprite*)__copy_trivial(last, _M_finish, first);
        _M_finish = i;
        return first;
    }
    __forceinline void resize(unsigned long newSize) { resize(newSize, PSprite()); }
    __forceinline void resize(unsigned long newSize, const PSprite& value)
    {
        if (newSize < size())
            erase(_M_start + newSize, _M_finish);
        else
            _M_fill_insert(_M_finish, newSize - size(), value);
    }
};
static __forceinline bool SortsBefore(const PSprite b, const PSprite a)
{
    if (a->MeshKey > b->MeshKey)
        return true;
    if (a->MeshKey == b->MeshKey && a->Depth > b->Depth)
        return true;
    return false;
}
static __forceinline void Swap(PSprite& a, PSprite& b)
{
    PSprite tmp = a;
    a = b;
    b = tmp;
}
struct CGPUScratchBufferTextureManager {
    long StartSurfaceUse(long handle);             // 0x00F36EE0
    long StartSurfaceUse2(long handle);            // 0x00F36F10
    void EndSurfaceUse(long handle);               // 0x00F36F00
    void EndSurfaceUse2(long handle);              // 0x00F36F30
};
struct CDisplayManagerView {
    unsigned char _pad[0x3190];
    CGPUScratchBufferTextureManager ScratchTextures;   // +0x3190
};
extern CDisplayManagerView* g_pDisplayManager;     // 0x01436E98

struct SFogColour { unsigned char A, B, C, D; };
struct CEngineLightingManagerView {
    unsigned char _pad00[0xce];
    bool          PendingAmbient;                  // +0xce
    unsigned char _padcf[0xe0 - 0xcf];
    SFogColour    Colour;                          // +0xe0
    unsigned long DirtyFlags;                      // +0xe4
    long          PendingLights;                   // +0xe8
    __forceinline void SetColour(const SFogColour& c)
    {
        if (Colour.A != c.A || Colour.B != c.B || Colour.C != c.C) {
            Colour = c;
            if (Colour.C == 0xff) {
                Colour.A = Colour.B;
                Colour.C = 0;
            }
            DirtyFlags |= 0xf;
        }
    }
    __forceinline void Flush()
    {
        if (PendingAmbient) {
            PendingAmbient = false;
            DirtyFlags |= 1;
        }
        SFogColour none;
        none.A = 0;
        none.B = 0;
        none.C = 0;
        SetColour(none);
        if (PendingLights != 0) {
            PendingLights = 0;
            DirtyFlags |= 2;
        }
    }
};
extern CEngineLightingManagerView* g_pEngineLightingManager;   // 0x01436E9C

struct CEnginePrimitiveManagerRepeatedZSprites {
    unsigned char _pad00[0x80];
    CSortList     PrimitiveSortList;               // +0x80
    long          BillboardSurface;                // +0x8c
    long          ZBufferSurface;                  // +0x90
    unsigned char _pad94[0xa4 - 0x94];
    long          BillboardTextureHandle;          // +0xa4
    long          ZBufferTextureHandle;            // +0xa8
    unsigned char _padac[0xc0 - 0xac];
    long          RenderedCount;                   // +0xc0
    void RenderFromSortList(PSprite sprite);       // 0x00BAA870
    void RenderNonsortedPrimitives();
};

void CEnginePrimitiveManagerRepeatedZSprites::RenderNonsortedPrimitives()
{
    CGPUScratchBufferTextureManager* scratch = &g_pDisplayManager->ScratchTextures;
    BillboardSurface = scratch->StartSurfaceUse(BillboardTextureHandle);
    ZBufferSurface = scratch->StartSurfaceUse2(ZBufferTextureHandle);
    for (long gap = PrimitiveSortList.size() - 1; gap > 0; --gap) {
        for (unsigned long j = 0; j < PrimitiveSortList.size() - gap; ++j) {
            if (SortsBefore(PrimitiveSortList[j + gap], PrimitiveSortList[j]))
                Swap(PrimitiveSortList[j], PrimitiveSortList[j + gap]);
        }
    }
    for (unsigned long i = 0; i < PrimitiveSortList.size(); ++i)
        RenderFromSortList(PrimitiveSortList[i]);
    PrimitiveSortList.resize(0);
    RenderedCount = 0;
    scratch->EndSurfaceUse(BillboardTextureHandle);
    BillboardSurface = 0;
    scratch->EndSurfaceUse2(ZBufferTextureHandle);
    ZBufferSurface = 0;
    g_pEngineLightingManager->Flush();
}