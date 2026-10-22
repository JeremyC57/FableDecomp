// Grade: DIFFER(641v641) -- frame-slot layout only: retail offset@+0x20, level@+0x28, top-level size@+0x30, header@+0x34 (8-byte holes shared with the two GetDimensions temps); ours level@+0x20, offset@+0x24, size@+0x2c, header@+0x30
// CEngineSurfaceCompositionManager::WriteTextureToBank(CTexture& texture, CGraphicDataBank& bank, unsigned long index, bool transparent) -- size the bank entry for every mip level, copy each level's locked surface bytes into the entry through a zeroed staging vector, then build the graphic header (dimensions, pixel format, transparency, mip count, top-level size) and write it as the entry's info
// Leaf: WriteTextureToBank
// Layout: local model; Ego_r.pdb CGraphicHeader (0x1c: Width/Height/Depth/FrameWidth/FrameHeight/FrameCount ushorts, PixelFormat +0xc,
//         TransparencyType +0x10, MipmapLevels +0x11, IsFlatAnimatedGraphic bit +0x12, FrameDataSize +0x14), CSurfaceLockedRect (0x10: PMem +0xc),
//         CSurface (vtable 0x0122F84C, 0x10 bytes); the bank is a CBankFile at offset 0 of CGraphicDataBank.
#include <stdlib.h>
#include <string.h>

struct C2DExtentsI { long X, Y; };
struct CPixelFormat { long FormatIndex; };
struct CSurfaceLockedRect {
    C2DExtentsI    Dimensions;                  // +0x00
    long           Pitch;                       // +0x08
    unsigned char* PMem;                        // +0x0c
};
struct IDirect3DSurface9;
struct CSurface {                               // vtable 0x0122F84C
    IDirect3DSurface9* PD3DSurface;             // +0x04
    unsigned long      AllocationSource;        // +0x08
    void*              PAllocatedMemory;        // +0x0c
    virtual void v00();
    ~CSurface() { Uninitialise(); }
    void Uninitialise();                        // 0x009F2E20
    unsigned long GetByteLength() const;        // 0x009F2FC0
    CSurfaceLockedRect Lock(unsigned long flags);   // 0x009F3010
    void Unlock();                              // 0x009F2DC0
};
struct CTexture {
    long GetNoLevels() const;                   // 0x009F9DF0
    long GetLevelByteLength(long level) const;  // 0x009F9E80
    CSurface GetSurface(long level);            // 0x009F9E00
    C2DExtentsI GetDimensions(unsigned long level) const;   // 0x009F9DB0
    CPixelFormat GetPixelFormat() const;        // 0x009F9D80
};

struct CDataOutputStream;
struct CDataInputStream;
struct CMemoryDataOutputStream {
    unsigned char _body[0x1c];
    CMemoryDataOutputStream();                  // 0x00996AF0
    ~CMemoryDataOutputStream();                 // 0x00996B30
    long GetSize() const;                       // 0x00994B40
    const void* PeekData() const;               // 0x00995E70
    operator CDataOutputStream*() { return (CDataOutputStream*)this; }
};
struct CMemoryDataInputStream {
    unsigned char _body[0x1c];
    CMemoryDataInputStream(const void* data, long size);   // 0x00994930
    ~CMemoryDataInputStream();                  // 0x00993B10
    operator CDataInputStream&() { return *(CDataInputStream*)this; }
};

struct CGraphicHeader {                         // 0x1c bytes
    unsigned short Width;                       // +0x00
    unsigned short Height;                      // +0x02
    unsigned short Depth;                       // +0x04
    unsigned short FrameWidth;                  // +0x06
    unsigned short FrameHeight;                 // +0x08
    unsigned short FrameCount;                  // +0x0a
    CPixelFormat   PixelFormat;                 // +0x0c
    unsigned char  TransparencyType;            // +0x10
    unsigned char  MipmapLevels;                // +0x11
    unsigned char  IsFlatAnimatedGraphic : 1;   // +0x12
    unsigned long  FrameDataSize;               // +0x14
    unsigned long  CompressedMipmapSizes[1];    // +0x18
    CGraphicHeader();                           // 0x009FD6C0
    void Save(CDataOutputStream* stream);       // 0x009FD1E0
};

struct CBankFile {
    void InternalWriteEntryData(unsigned long index, unsigned long start, const void* data, unsigned long size);   // 0x009CCCD0
    void InternalResizeEntryData(unsigned long index, unsigned long size);   // 0x009D1260
    void UpdateEntryInfoOnly(unsigned long index, CDataInputStream& info, unsigned long size);   // 0x009CC8B0
};
struct CGraphicDataBank : CBankFile {
};

struct CByteVector {
    unsigned char* first;
    unsigned char* last;
    unsigned char* end;
    explicit CByteVector(unsigned long n) : first(0), last(0), end(0)
    {
        first = n ? (unsigned char*)malloc(n) : 0;
        last = first;
        end = first + n;
        last = fill_n(first, n);
    }
    static unsigned char* allocate(unsigned long n)
    {
        unsigned char* p = 0;
        if (n)
            p = (unsigned char*)malloc(n);
        return p;
    }
    static unsigned char* fill_n(unsigned char* p, unsigned long n)
    {
        memset(p, 0, n);
        return p + n;
    }
    unsigned long size() const { return (unsigned long)(last - first); }
    unsigned char* data() { return size() ? first : (unsigned char*)this; }
    ~CByteVector() { if (first) free(first); }
};

struct CEngineSurfaceCompositionManager {
    void WriteTextureToBank(CTexture& texture, CGraphicDataBank& bank, unsigned long index, bool transparent);
};

void CEngineSurfaceCompositionManager::WriteTextureToBank(CTexture& texture, CGraphicDataBank& bank, unsigned long index, bool transparent)
{
    unsigned long totalSize = 0;
    unsigned long noLevels = texture.GetNoLevels();
    for (unsigned long i = 0; i < noLevels; ++i)
        totalSize += texture.GetLevelByteLength(i);
    bank.InternalResizeEntryData(index, totalSize);
    unsigned long topLevelSize = texture.GetLevelByteLength(0);
    {
        unsigned long offset = 0;
        for (unsigned long level = 0; level < noLevels; ++level) {
            CSurface surface = texture.GetSurface(level);
            unsigned long size = surface.GetByteLength();
            CByteVector data(size);
            CSurfaceLockedRect rect = surface.Lock(0);
            memcpy(data.data(), rect.PMem, size);
            surface.Unlock();
            bank.InternalWriteEntryData(index, offset, data.data(), size);
            offset += size;
        }
    }
    long height = texture.GetDimensions(0).Y;
    long width = texture.GetDimensions(0).X;
    CGraphicHeader header;
    header.Width = (unsigned short)width;
    header.Height = (unsigned short)height;
    header.Depth = 1;
    header.FrameWidth = (unsigned short)width;
    header.FrameHeight = (unsigned short)height;
    header.FrameCount = 1;
    header.PixelFormat = texture.GetPixelFormat();
    header.TransparencyType = transparent ? 1 : 0;
    header.MipmapLevels = (unsigned char)noLevels;
    header.IsFlatAnimatedGraphic = 0;
    header.FrameDataSize = topLevelSize;
    CMemoryDataOutputStream stream;
    header.Save(stream);
    CMemoryDataInputStream info(stream.PeekData(), stream.GetSize());
    bank.UpdateEntryInfoOnly(index, info, stream.GetSize());
}
