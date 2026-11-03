// CEngineLightingManager::AddLightsToLookupGrid() -- with local lights enabled, link every local light into each cell of the 16x16 light lookup grid its radius square covers (cells of 16 world units from LookupGridWorldX/Y, clamped to the grid): one pool-allocated CLocalLightElementLink per cell, pushed on the cell's list
// Grade: RELOCATION_MATCH (557 bytes)
// Leaf: ?AddLightsToLookupGrid@CEngineLightingManager@@AAEXXZ
// Flags: none
// Retail-asm: 0x00B466EE..0x00B46708 `fld [ebp-24h] ; fsub [ebp-30h] ; fistp dword ptr [ebp-1Ch] ; fild [ebp-1Ch] ; fadd [ebp-2Ch] ; fstp [ebp-34h] ; mov eax,[ebp-24h] ; cmp eax,[ebp-34h] ; jne ; inc dword ptr [ebp-1Ch]` (and 0x00B46726, 0x00B4675E, 0x00B46796) -- four expansions of the engine's inline-assembly floor helper GFFloatToLongFloor (standalone 0x0048D940; same helper body as landed 0x00B46A30); VC7.1 emits no bare dword fistp from C++. Here the expansions sit in a loop, so VC hoists their 0.5f/1.0f frame constants above it (0x00B46695..0x00B466C6).
// Helper (2026-11-03 asm twins): the retail inline-asm block is the shared engine/GFFloatToLongFloor.h (two-path: the byte-matching asm, and a FABLE_PORTABLE C++ twin); this row carries no asm of its own.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B46660, length 557 bytes, ebp frame (0x5C), plain `ret`.
// Name: Ego_r.pdb + FableWin.pdb `?AddLightsToLookupGrid@CEngineLightingManager@@AAEXXZ` (private).
// Callees: CDynamicObjectHeap<12,16>::CreateMoreBlocks 0x00B48F20 (inline Alloc of the static operator new
//   ??2CLocalLightElementLink@CEngineLightingManager@@SIPAXI@Z); CLocalLightElementLink vtable 0x012A20DC (its ctor).
// Globals: LOCAL_HEAP_CEngineLightingManager__LightElementLink 0x01436ED0 (FableWin: the dynamic initializer of that name, 0x012246E0).
// Layout: engine/CEngineLightingManager.h (LocalLightList +0xAC as CArray<CManagerLocalLight*>, LookupGridWorldX/Y,
//   LocalLightsEnabled +0xCC, LightLookupGrid +0xEC); CDynamicObjectHeap as the landed heap rows (0x00AF8B00, 0x012246E0).
// Shape: LocalLightList is named at each use (size() and operator[]): a local CArray reference lets VC carry _M_start
//   across the loop (retail re-reads it at the loop top, 0x00B466D0).
#pragma optimize("y", off)
#define CEngineLightingManager CEngineLightingManagerLayout
#include "engine/CEngineLightingManager.h"
#undef CEngineLightingManager
#include "engine/C3DVector.h"
#include "engine/CArray.h"

class CRGBFloatColour { public: float R, G, B, A; };
class CEngineLocalLightElement { public:
    C3DVector WorldPos;
    float Radius;
    float RadiusSquared;
    float SpotRadius;
    CRGBFloatColour Colour;
    float AttenuationDistSquared;
    float AttenuationConstant;
    unsigned long ChannelMask;
};

class CDynamicObjectHeapBase { public:
    virtual ~CDynamicObjectHeapBase();
    long AllocCount;
    long TotalAllocatorBlocks;
    bool RefCountReleaseEnabled;
    CDynamicObjectHeapBase* HeapNext;
private:
    virtual void Release() = 0;
};
template <long ObjectSize, long BlockCount> class CDynamicObjectHeap : public CDynamicObjectHeapBase { public:
    struct CObjectEntry { CObjectEntry* NextEntry; unsigned char Pad[ObjectSize - 4]; };
    struct CObjectAllocatorBlockEntry { CObjectEntry Blocks[BlockCount]; CObjectAllocatorBlockEntry* NextEntry; };
    CObjectEntry* FreeList;
    CObjectAllocatorBlockEntry StartBlock;
    bool AllowHeapToGrow;
    void* Alloc(long)
    {
        CObjectEntry* entry = FreeList;
        if (!entry) {
            if (AllowHeapToGrow) {
                CreateMoreBlocks();
                entry = FreeList;
            } else
                return 0;
        }
        ++AllocCount;
        FreeList = entry->NextEntry;
        return entry;
    }
private:
    virtual void Release();
    void CreateMoreBlocks();
};
extern CDynamicObjectHeap<12, 16> LOCAL_HEAP_CEngineLightingManager__LightElementLink;   // 0x01436ED0

#include "engine/GFFloatToLongFloor.h"

class CEngineLightingManager : public CEngineLightingManagerLayout {
public:
    struct CManagerLocalLight {
        virtual void Delete();
        CEngineLocalLightElement LightElement;
        CManagerLocalLight* NextLocalLight;
        float ClippingScore;
        float ThemeWeight;
        bool AlreadyFoundFlag;
    };
    struct CLocalLightElementLink {
        virtual void Delete();
        CManagerLocalLight* Light;
        CLocalLightElementLink* Next;
        CLocalLightElementLink() { }
        static void* __fastcall operator new(unsigned int size)
        {
            return LOCAL_HEAP_CEngineLightingManager__LightElementLink.Alloc((long)size);
        }
    };
private:
    void AddLightsToLookupGrid();
};

void CEngineLightingManager::AddLightsToLookupGrid()
{
    if (!LocalLightsEnabled)
        return;
    CLocalLightElementLink* (*grid)[16] = reinterpret_cast<CLocalLightElementLink* (*)[16]>(LightLookupGrid);
    for (unsigned long i = 0; i < reinterpret_cast<CArray<CManagerLocalLight*>&>(LocalLightList).size(); ++i) {
        CManagerLocalLight* light = reinterpret_cast<CArray<CManagerLocalLight*>&>(LocalLightList)[i];
        long minX = GFFloatToLongFloor((light->LightElement.WorldPos.X - LookupGridWorldX - light->LightElement.Radius) * 0.0625);
        long minY = GFFloatToLongFloor((light->LightElement.WorldPos.Y - LookupGridWorldY - light->LightElement.Radius) * 0.0625);
        long maxX = GFFloatToLongFloor((light->LightElement.WorldPos.X - LookupGridWorldX + light->LightElement.Radius) * 0.0625);
        long maxY = GFFloatToLongFloor((light->LightElement.WorldPos.Y - LookupGridWorldY + light->LightElement.Radius) * 0.0625);
        if (minX < 0) minX = 0;
        if (minY < 0) minY = 0;
        if (maxX > 15) maxX = 15;
        if (maxY > 15) maxY = 15;
        for (long y = minY; y <= maxY; ++y) {
            for (long x = minX; x <= maxX; ++x) {
                CLocalLightElementLink* link = new CLocalLightElementLink;
                link->Light = light;
                link->Next = grid[x][y];
                grid[x][y] = link;
            }
        }
    }
}