// std::_Construct(CGPUScratchBufferTextureManager::CResourceEntry*, const CGPUScratchBufferTextureManager::CResourceEntry&) -- placement copy of a scratch-buffer resource entry: twelve scalar words copied member by member (no block copy)
// Retail: Fable.exe @ 0x00913590, 75 bytes; __fastcall-shaped STLport helper (ecx = element slot, edx = source element).
// Callees: none (inline memberwise copy); caller CGPUScratchBufferTextureManager AddResourceEntry 0x00913E70.
// Leaf: _Construct
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)
namespace N00913590 {
struct CTexture; struct CSurface;
enum EResourceType { RESOURCE_TYPE_NONE };
enum ESurfaceMultisampleType { SURFACE_MULTISAMPLE_NONE };
enum ESurfacePool { SURFACE_POOL_NONE };
struct C2DExtentsI {
    long X, Y;
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
    __forceinline CResourceEntry(const CResourceEntry& o)   // memberwise copy, spelled out so VC7.1 inlines it as single moves
        : ResourceType(o.ResourceType), Texture(o.Texture), Surface(o.Surface), PageIndex(o.PageIndex), Dimensions(o.Dimensions),
          NoLevels(o.NoLevels), PixelFormat(o.PixelFormat), MultiSampleType(o.MultiSampleType), UseageFlags(o.UseageFlags),
          MemoryPool(o.MemoryPool), RetailExtra(o.RetailExtra) { }
};

// STLport _Construct: placement-copy one element (null-checked placement new, then the implicit copy constructor)
void __fastcall _Construct(CResourceEntry* p, const CResourceEntry& value)
{
    if (p != 0)
        new (p) CResourceEntry(value);
}
}