// CGPUScratchBufferTextureManager::CResourceEntry::CResourceEntry(const CResourceEntry&) -- memberwise copy constructor of the 44-byte scratch-buffer resource entry
// Leaf: ??0CResourceEntry
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F37970, length 73 bytes.
// Callees: none.
// Identity: the manifest label CPreTransposedBoneMatrix::Set is propagated (that class is 48 bytes). The only
// caller is the vector fill-insert 0x00F381F0 (`T tmp(value)` into its frame, 44-byte elements by the
// 0x2E8BA2E9 reciprocal), reached from CGPUScratchBufferTextureManager::AllocResourceEntry 0x00F372B0; the
// Ego_r.pdb CResourceEntry is 0x2c bytes: ResourceType, Texture, Surface, PageIndex, Dimensions (C2DExtentsI),
// NoLevels, PixelFormat, MultiSampleType, UseageFlags, MemoryPool.
// Shape: `mov eax,ecx` (returns this) then eleven dword moves through edx in member order.
struct CTexture;
struct CSurface;
enum EResourceType { RESOURCE_TYPE_0 };
enum ESurfaceMultisampleType { SURFACE_MULTISAMPLE_NONE };
enum ESurfacePool { SURFACE_POOL_0 };

struct C2DExtentsI {
    long Width;
    long Height;
};

#include "engine/CPixelFormat.h"                           // FormatIndex, 4 bytes

struct CGPUScratchBufferTextureManager {
    struct CResourceEntry {                                 // 44 bytes (PDB layout = retail)
        EResourceType           ResourceType;               // +0x00
        CTexture*               Texture;                    // +0x04
        CSurface*               Surface;                    // +0x08
        long                    PageIndex;                  // +0x0c
        C2DExtentsI             Dimensions;                 // +0x10
        long                    NoLevels;                   // +0x18
        CPixelFormat            PixelFormat;                // +0x1c
        ESurfaceMultisampleType MultiSampleType;            // +0x20
        unsigned long           UseageFlags;                // +0x24
        ESurfacePool            MemoryPool;                 // +0x28
        CResourceEntry(const CResourceEntry& other);
    };
};

CGPUScratchBufferTextureManager::CResourceEntry::CResourceEntry(const CResourceEntry& other)
    : ResourceType(other.ResourceType), Texture(other.Texture), Surface(other.Surface),
      PageIndex(other.PageIndex), Dimensions(other.Dimensions), NoLevels(other.NoLevels),
      PixelFormat(other.PixelFormat), MultiSampleType(other.MultiSampleType),
      UseageFlags(other.UseageFlags), MemoryPool(other.MemoryPool)
{
}
