// CRegionMinimap::BuildRegionMinimapTGA(const CWideString& filename) -- shade every minimap cell from its theme colour and light value, write the surface out as a TGA, then free the cell list
// Leaf: BuildRegionMinimapTGA
// Retail-asm: 0x0066CE2B `fld dword ptr [esp + 0x30]` ; 0x0066CE2F `fistp dword ptr [esp + 0x34]` -- the engine's inline-assembly float-to-long helper (rasm1 family (a)): the value is stored to the helper's by-value parameter slot and reloaded for a bare dword fistp into a frame word; VC7.1 C++ `(long)x` calls __ftol2, /QIfist emits `fistp qword` and /arch:SSE2 cvttss2si (rasm1 probe family (a)). Everything around it is C++.
// Retail: Fable.exe (TLC, VC7.1) @ 0x0066CD50; reconstructed from the parked reference body (rebuild/candidates/manual/pending), the C++ rounding stand-in replaced by the inline-assembly helper. The light blend weight is rounded by the helper; the three colour scalings stay __ftol2 casts.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14
struct CWideString { void* m_pData; };
struct C2DExtentsI;
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour() {}
    CRGBColour(unsigned char b, unsigned char g, unsigned char r, unsigned char a) : B(b), G(g), R(r), A(a) {}
    CRGBColour(const CRGBColour& c) : B(c.B), G(c.G), R(c.R), A(c.A) {}
};
CRGBColour __cdecl GFGetInterpolatedColour(CRGBColour a, CRGBColour b, unsigned long t);   // 0x00A44250
struct CSurface {
    virtual void v00();
    void* PD3DSurface; unsigned long AllocationSource; void* PAllocatedMemory;
    void SetPixel(long x, long y, const CRGBColour* colour);                                 // 0x009F4120
    bool SaveAsTGA(const CWideString* filename, const C2DExtentsI* extents, bool flag);      // 0x009F4590
};
struct RegionMinimapCellData { long eTheme; float light; };
struct CCellVector {
    RegionMinimapCellData* m_pFirst; RegionMinimapCellData* m_pLast; RegionMinimapCellData* m_pEnd;
    void Free()
    {
        RegionMinimapCellData* first = m_pFirst;
        m_pFirst = 0; m_pLast = 0; m_pEnd = 0;
        if (first)
            free(first);
    }
};
struct CThemeInfo { bool Shaded; unsigned char _pad[7]; };
extern CThemeInfo g_RegionThemeInfo[];             // 0x0137E9DC (8-byte entries, flag byte first)
// the engine's inline-assembly float-to-long helper (VC7.1 expands the __forceinline body and its __asm in place)
__forceinline long RoundToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}

struct CRegionMinimap {
    long          RegionMinimapSize;               // +0x00
    long          RegionMapBox[4];                 // +0x04
    long          RegionMapSquare[4];              // +0x14
    CSurface      RegionMinimapSurface;            // +0x24
    unsigned char RegionMinimapTexture[8];         // +0x34
    float         IlluminationDirection[3];        // +0x3c
    CCellVector   RegionMinimapCellList;           // +0x48
    unsigned long MaxRegionMinimapThemeColours;    // +0x54
    CRGBColour*   RegionMinimapThemeColour;        // +0x58 (vector start)
    void BuildRegionMinimapTGA(const CWideString& filename);
};

void CRegionMinimap::BuildRegionMinimapTGA(const CWideString& filename)
{
    long size = RegionMinimapSize;
    for (long y = 0, flippedY = size - 1; y < size; ++y, --flippedY) {
        for (long x = 0; x < size; ++x) {
            long theme = RegionMinimapCellList.m_pFirst[RegionMinimapSize * y + x].eTheme;
            CRGBColour colour = RegionMinimapThemeColour[theme];
            CRGBColour pixel;
            if (g_RegionThemeInfo[theme].Shaded) {
                unsigned char r = (unsigned char)(long)(colour.R * 0.8f);
                unsigned char g = (unsigned char)(long)(colour.G * 0.8f);
                unsigned char b = (unsigned char)(long)(colour.B * 0.8f);
                float t = (RegionMinimapCellList.m_pFirst[RegionMinimapSize * y + x].light + 1.0f) * 0.5f * 256.0f;
                pixel = GFGetInterpolatedColour(colour, CRGBColour(b, g, r, 0x80), RoundToLong(t));
            } else
                pixel = colour;
            RegionMinimapSurface.SetPixel(x, flippedY, &pixel);
        }
    }
    RegionMinimapSurface.SaveAsTGA(&filename, 0, false);
    RegionMinimapCellList.Free();
}
