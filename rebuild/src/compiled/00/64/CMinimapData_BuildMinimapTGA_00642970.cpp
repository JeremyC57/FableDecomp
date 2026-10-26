// CMinimapData::BuildMinimapTGA(const CWideString& filename) -- render the minimap's theme grid into its 32-bit surface (one pixel per cell, coloured by theme) and save it as a TGA
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00642970, length 177 bytes (`ret 4`).
// Callees: CPixelFormat::Initialise(long, long, long, long, bool) @ 0x009E3790 (after the inline ctor's FormatIndex = -1),
// CSurface::InitialiseAsImage(const C2DExtentsI*, const CPixelFormat*, bool) @ 0x009F3230, CSurface::SetPixel(long, long, const CRGBColour*)
// @ 0x009F4120, CSurface::SaveAsTGA(const CWideString*, const C2DExtentsI*, bool) @ 0x009F4590.
// Layout: Ego_r.pdb CMinimapData with retail's 12-byte vectors: MinimapExtents +0x00, MinimapThemeList +0x08 (4-byte themes),
// MinimapSurface +0x14, ColourMinimapTheme +0x30 (CRGBColour).
// Shape: width/height read first (ebp / a spilled frame word), the surface pointer kept in the frame, the extents passed through a local
// copy, the cell index `MinimapExtents.X * y + x` re-read from the member each pass, the colour copied to a frame local for SetPixel.
class CWideString;
class C2DExtentsI { public:
    long X, Y;
    C2DExtentsI(long x, long y) : X(x), Y(y) { }
};
class CRGBColour { public:
    unsigned char B, G, R, A;
};
class CPixelFormat { public:
    long FormatIndex;
    CPixelFormat() : FormatIndex(-1) { }
    void Initialise(long r, long g, long b, long a, bool alpha);   // 0x009E3790
};
class CSurface { public:
    virtual void v00();
    void*         PD3DSurface;                  // +0x04
    unsigned long AllocationSource;             // +0x08
    void*         PAllocatedMemory;             // +0x0c
    bool InitialiseAsImage(const C2DExtentsI* size, const CPixelFormat* format, bool a);   // 0x009F3230
    void SetPixel(long x, long y, const CRGBColour* colour);                               // 0x009F4120
    bool SaveAsTGA(const CWideString* filename, const C2DExtentsI* size, bool a);          // 0x009F4590
};
struct MinimapTheme {
    long ThemeIndex;
};
template<class T> struct vector {               // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T& operator[](unsigned long i) { return *(_M_start + i); }
};
class CTexture { public:
    void*         PD3DTexture;
    unsigned long Bits;
};
class CMinimapData { public:
    C2DExtentsI          MinimapExtents;        // +0x00
    vector<MinimapTheme> MinimapThemeList;      // +0x08
    CSurface             MinimapSurface;        // +0x14
    CTexture             MinimapTexture;        // +0x24
    unsigned long        MaxMinimapThemeColours;// +0x2c
    vector<CRGBColour>   ColourMinimapTheme;    // +0x30
    void BuildMinimapTGA(const CWideString& filename);
};

void CMinimapData::BuildMinimapTGA(const CWideString& filename)
{
    long width = MinimapExtents.X;
    long height = MinimapExtents.Y;
    CPixelFormat format;
    format.Initialise(8, 8, 8, 8, true);
    C2DExtentsI size(width, height);
    CSurface* surface = &MinimapSurface;
    surface->InitialiseAsImage(&size, &format, false);
    for (long y = 0; y < height; ++y)
        for (long x = 0; x < width; ++x) {
            CRGBColour colour = ColourMinimapTheme[MinimapThemeList[MinimapExtents.X * y + x].ThemeIndex];
            surface->SetPixel(x, y, &colour);
        }
    surface->SaveAsTGA(&filename, 0, false);
}