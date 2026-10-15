// CRegionMinimap::CRegionMinimap() -- construct the region minimap's member objects: the surface (vtable + two cleared words), the texture and the two empty vectors
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0066CCC0, length 42 bytes. No callees.
// Vtable: CSurface 0x0122F84C (rtti-vtables.tsv; slot 0 = 0x0041C9C0).
// Layout: PDB names with the retail offsets. engine/CRegionMinimap.h agrees up to +0x48, but the PDB's vectors are 16 bytes
// and retail's 12 (begin/end/capacity, no allocator word), so retail MaxRegionMinimapThemeColours is +0x54 and
// RegionMinimapThemeColour +0x58..+0x63 (the header has +0x58/+0x5c): the class is modelled locally.
// Shape: only member objects with inline constructors are initialised -- CSurface stores its vtable and zeroes PD3DSurface and
// AllocationSource (PAllocatedMemory is left alone), CTexture zeroes its pointer and its packed ByteLength:28 /
// AllocationSource:4 word (modelled as one dword), each vector zeroes its three pointers; the plain members (size, boxes,
// illumination direction, colour count) are not touched. `this` moves to eax and ecx becomes the zero register.
// Leaf: ??0CRegionMinimap
struct IDirect3DSurface9;
struct IDirect3DTexture9;

struct CSurface {
    IDirect3DSurface9* PD3DSurface;                     // +0x04
    unsigned long      AllocationSource;                // +0x08
    void*              PAllocatedMemory;                // +0x0c
    CSurface() : PD3DSurface(0), AllocationSource(0) {}
    virtual ~CSurface();                                // slot 0 (0x0041C9C0)
};

struct CTexture {
    IDirect3DTexture9* PD3DTexture;                     // +0x00
    unsigned long      ByteLengthAndSource;             // +0x04 ByteLength:28, AllocationSource:4
    CTexture() : PD3DTexture(0), ByteLengthAndSource(0) {}
};

template<class T> struct CRetailVector {                // retail std::vector: three pointers
    T* m_first; T* m_last; T* m_end;
    CRetailVector() : m_first(0), m_last(0), m_end(0) {}
};

struct C2DBoxI { long TLX, TLY, BRX, BRY; };
struct C3DVector { float X, Y, Z; };
struct CRGBColour { unsigned char B, G, R, A; };
struct RegionMinimapCellData;

struct CRegionMinimap {
    long                                 RegionMinimapSize;              // +0x00
    C2DBoxI                              RegionMapBox;                   // +0x04
    C2DBoxI                              RegionMapSquare;                // +0x14
    CSurface                             RegionMinimapSurface;           // +0x24
    CTexture                             RegionMinimapTexture;           // +0x34
    C3DVector                            IlluminationDirection;          // +0x3c
    CRetailVector<RegionMinimapCellData> RegionMinimapCellList;          // +0x48
    unsigned long                        MaxRegionMinimapThemeColours;   // +0x54
    CRetailVector<CRGBColour>            RegionMinimapThemeColour;       // +0x58
    CRegionMinimap();
};

CRegionMinimap::CRegionMinimap()
{
}