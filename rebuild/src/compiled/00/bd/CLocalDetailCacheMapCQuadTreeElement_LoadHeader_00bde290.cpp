// CLocalDetailCacheMap::CQuadTreeElement::LoadHeader(CMemoryDataInputStream&) -- read the 44-byte quad-tree element file header and copy its map rectangle, file-block location, fade distance, primitive mask and bounding sphere
// Leaf: LoadHeader
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDE290, length 247 bytes, `ret 4`.
// Callees: the inlined CDataInputStream::Read (64-bit position range check, direct copy while the source chunk has
// room, else CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0).
// Layout: Ego_r.pdb CLocalDetailCacheMap::CQuadTreeElement (76 bytes, the landed 0x00BDE020 model): BoundingSphere
// +0x18, MaxFadeDistance +0x2c, PrimitiveTypeMask +0x30, FileBlockPos +0x34, FileBlockSize +0x38, OffsetIntoFileBlock
// +0x3c, MapX/MapY/Width/Height +0x40..+0x46. Stream fields: StreamPos +4, CurrentSourcePtr +0xc,
// SourceChunkBytesRemaining +0x14.
// Shape: the landed sibling CObjectCacheGroupCollection::LoadHeader (0x00BDE190): a header local whose bounding
// sphere constructor zeroes its four floats (immediate stores), the __forceinline Read, then member-wise copies in
// source order (the four shorts, the file-block trio, fade/mask, then the sphere through its implicit assignment).
extern "C" void* __cdecl memcpy(void*, const void*, unsigned int);
#pragma intrinsic(memcpy)

class CDataInputStream { public:
    void* __vftable;
    unsigned long StreamPos;                            // +0x04
    long StreamSize;                                    // +0x08
    const unsigned char* CurrentSourcePtr;              // +0x0c
    unsigned long SourceChunkStreamPos;                 // +0x10
    long SourceChunkBytesRemaining;                     // +0x14
    __forceinline void Read(void* destination, long bytes)
    {
        __int64 next = (unsigned long)(StreamPos + bytes);
        if (next >= 0 && next <= 0x7fffffff) {
            if (SourceChunkBytesRemaining >= bytes) {
                memcpy(destination, CurrentSourcePtr, bytes);
                CurrentSourcePtr += bytes;
                SourceChunkBytesRemaining -= bytes;
                StreamPos += bytes;
            } else
                ReadWithSrcChunkOverflow((unsigned char*)destination, bytes);
        }
    }
private:
    void ReadWithSrcChunkOverflow(unsigned char*, long);   // 0x00993CA0
};
class CMemoryDataInputStream : public CDataInputStream { public:
    unsigned char* StreamSource;                        // +0x18
};

class C3DBoundingSphere { public:                              // centre + radius
    float X, Y, Z, Radius;
    C3DBoundingSphere() : X(0), Y(0), Z(0), Radius(0) { }
};

struct CQuadTreeElementFileHeader {                     // 0x2c bytes on disk
    C3DBoundingSphere BoundingSphere;                   // +0x00
    float          MaxFadeDistance;                     // +0x10
    unsigned long  PrimitiveTypeMask;                   // +0x14
    long           FileBlockPos;                        // +0x18
    long           FileBlockSize;                       // +0x1c
    long           OffsetIntoFileBlock;                 // +0x20
    unsigned short MapX, MapY, Width, Height;           // +0x24
};

class CLocalDetailCacheMap { public:
    class CObjectCacheGroupCollection;
    class CQuadTreeElement { public:                           // PDB layout (76 bytes)
        virtual ~CQuadTreeElement();                    // slot 0
        CObjectCacheGroupCollection* ObjectCacheGroupCollections;   // +0x04
        CQuadTreeElement*  Children[4];                 // +0x08
        C3DBoundingSphere  BoundingSphere;              // +0x18
        CLocalDetailCacheMap* Map;                      // +0x28
        float              MaxFadeDistance;             // +0x2c
        unsigned long      PrimitiveTypeMask;           // +0x30
        long               FileBlockPos;                // +0x34
        long               FileBlockSize;               // +0x38
        long               OffsetIntoFileBlock;         // +0x3c
        unsigned short     MapX;                        // +0x40
        unsigned short     MapY;                        // +0x42
        unsigned short     Width;                       // +0x44
        unsigned short     Height;                      // +0x46
        void LoadHeader(CMemoryDataInputStream& stream);
    };
};

void CLocalDetailCacheMap::CQuadTreeElement::LoadHeader(CMemoryDataInputStream& stream)
{
    CQuadTreeElementFileHeader header;
    stream.Read(&header, sizeof(header));
    MapX = header.MapX;
    MapY = header.MapY;
    Width = header.Width;
    Height = header.Height;
    FileBlockPos = header.FileBlockPos;
    FileBlockSize = header.FileBlockSize;
    OffsetIntoFileBlock = header.OffsetIntoFileBlock;
    MaxFadeDistance = header.MaxFadeDistance;
    PrimitiveTypeMask = header.PrimitiveTypeMask;
    BoundingSphere = header.BoundingSphere;
}