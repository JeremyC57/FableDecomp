#include <cstdio>
#include <cstring>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#define __cdecl
#endif
static int g_extCopies = 0;   // out-of-line copy constructors the retail body calls
static int g_links = 0;       // CBaseIntelligentPointer::SetPItem calls
namespace N00913590 {
struct CTexture; struct CSurface;
enum EResourceType { RESOURCE_TYPE_NONE };
enum ESurfaceMultisampleType { SURFACE_MULTISAMPLE_NONE };
enum ESurfacePool { SURFACE_POOL_NONE };
struct C2DExtentsI {
    long X, Y;
    C2DExtentsI() { }
    C2DExtentsI(const C2DExtentsI& o) : X(o.X), Y(o.Y) { }
};
struct CPixelFormat { long FormatIndex; };
struct CResourceEntry {                          // retail stride 0x30 (AddResourceEntry 0x00913E70 steps 0x30); Ego_r.pdb lists 0x2C
    EResourceType           ResourceType;        // +0x00
    CTexture*               Texture;             // +0x04
    CSurface*               Surface;             // +0x08
    long                    PageIndex;           // +0x0C
    C2DExtentsI             Dimensions;          // +0x10
    long                    NoLevels;            // +0x18
    CPixelFormat            PixelFormat;         // +0x1C
    ESurfaceMultisampleType MultiSampleType;     // +0x20
    unsigned long           UseageFlags;         // +0x24
    ESurfacePool            MemoryPool;          // +0x28
    unsigned long           RetailExtra;         // +0x2C word the TLC build adds past the PDB layout (copied, meaning unknown)
    CResourceEntry() { }
    __forceinline CResourceEntry(const CResourceEntry& o)   // memberwise copy, spelled out so VC7.1 inlines it as single moves
        : ResourceType(o.ResourceType), Texture(o.Texture), Surface(o.Surface), PageIndex(o.PageIndex), Dimensions(o.Dimensions),
          NoLevels(o.NoLevels), PixelFormat(o.PixelFormat), MultiSampleType(o.MultiSampleType), UseageFlags(o.UseageFlags),
          MemoryPool(o.MemoryPool), RetailExtra(o.RetailExtra) { }
};

void __fastcall _Construct(CResourceEntry* p, const CResourceEntry& value)
{
    if (p != 0)
        new (p) CResourceEntry(value);
}
}

static double g_storage[64];
int main()
{
    using namespace N00913590;
    CResourceEntry src;
    src.ResourceType = (EResourceType)1;
    src.Texture = reinterpret_cast<CTexture*>(0x10);
    src.Surface = reinterpret_cast<CSurface*>(0x20);
    src.PageIndex = 3;
    src.Dimensions.X = 256;
    src.Dimensions.Y = 128;
    src.NoLevels = 9;
    src.PixelFormat.FormatIndex = 21;
    src.MultiSampleType = (ESurfaceMultisampleType)2;
    src.UseageFlags = 0x200u;
    src.MemoryPool = (ESurfacePool)1;
    src.RetailExtra = 0xABCDu;
    CResourceEntry* slot = reinterpret_cast<CResourceEntry*>(g_storage);
    _Construct(slot, src);
    bool ok = g_extCopies == 0 && g_links == 0;
    if (!(slot->ResourceType == src.ResourceType)) ok = false;
    if (!(slot->Texture == src.Texture)) ok = false;
    if (!(slot->Surface == src.Surface)) ok = false;
    if (!(slot->PageIndex == src.PageIndex)) ok = false;
    if (!(slot->Dimensions.X == src.Dimensions.X)) ok = false;
    if (!(slot->Dimensions.Y == src.Dimensions.Y)) ok = false;
    if (!(slot->NoLevels == src.NoLevels)) ok = false;
    if (!(slot->PixelFormat.FormatIndex == src.PixelFormat.FormatIndex)) ok = false;
    if (!(slot->MultiSampleType == src.MultiSampleType)) ok = false;
    if (!(slot->UseageFlags == src.UseageFlags)) ok = false;
    if (!(slot->MemoryPool == src.MemoryPool)) ok = false;
    if (!(slot->RetailExtra == src.RetailExtra)) ok = false;
    _Construct(0, src);                                  // a null slot constructs nothing
    if (g_extCopies != 0 || g_links != 0) ok = false;
    if (!ok) { std::printf("00913590_TEST FAIL\n"); return 1; }
    std::printf("00913590_TEST PASS\n");
    return 0;
}