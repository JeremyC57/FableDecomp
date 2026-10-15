// CRegionMinimap::CreateRegionMinimap(CWorldMap*, const CWideString&) -- size the cell list, initialise the surface and region box, populate the minimap from the world map, write it out as a TGA, then free the cell list
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0066DC50, length 158 bytes (`ret 8`).
// Leaf: CreateRegionMinimap
// Label correction: the catalog "CParticleSimulation::Clear" is propagated and wrong: every callee is a CRegionMinimap member
// (InitialiseRegionBox 0x0066C7B0, PopulateRegionMinimap(CWorldMap*) 0x0066CF00, BuildRegionMinimapTGA(CWideString*)
// 0x0066CD50, and the inlined InitialiseSurface whose landed copy is 0x0066C750), the offsets are the retail CRegionMinimap
// layout of the landed destructor 0x0066CCF0, and the only caller (0x00829FBB) is in CMiniMapDisplay. The method name is
// descriptive (the PDB name is not known).
// Callees: vector<RegionMinimapCellData>::resize @ 0x0066E6D0, CPixelFormat::Initialise(8, 8, 8, 0, true) @ 0x009E3790,
// CSurface::InitialiseAsImage @ 0x009F3230, the three members above, free @ 0x00BFEA14.
// Shape: InitialiseSurface inlined (size read first, the pixel format's inline ctor stores -1 into its frame word, the
// extents are two named words); the cell list is released at the end by an inline Free(): first read, the three words
// zeroed, then freed under a null test.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14
struct CWorldMap;
struct CWideString { void* m_pData; };
struct C2DExtentsI {
    long X;
    long Y;
};
struct CPixelFormat {
    long FormatIndex;
    CPixelFormat() : FormatIndex(-1) {}
    void Initialise(long r, long g, long b, long a, bool alphaSigned);   // 0x009E3790
};
struct CSurface {                                  // vtable 0x0122F84C, 0x10 bytes
    virtual void v00();
    void*         PD3DSurface;                     // +0x04
    unsigned long AllocationSource;                // +0x08
    void*         PAllocatedMemory;                // +0x0c
    bool InitialiseAsImage(const C2DExtentsI* extents, const CPixelFormat* format, bool flag);   // 0x009F3230
};
struct CTexture {
    void*         PD3DTexture;
    unsigned long ByteLengthAndSource;
};
struct RegionMinimapCellData {                     // 8 bytes
    long  eTheme;
    float light;
};
struct CCellVector {                               // retail std::vector<RegionMinimapCellData>
    RegionMinimapCellData* m_pFirst;
    RegionMinimapCellData* m_pLast;
    RegionMinimapCellData* m_pEnd;
    void resize(unsigned int n);                   // 0x0066E6D0
    void Free()
    {
        RegionMinimapCellData* first = m_pFirst;
        m_pFirst = 0;
        m_pLast = 0;
        m_pEnd = 0;
        if (first)
            free(first);
    }
};
struct CRGBColour { unsigned char B, G, R, A; };
struct CColourVector { CRGBColour* m_pFirst; CRGBColour* m_pLast; CRGBColour* m_pEnd; };
struct CRegionMinimap {
    long          RegionMinimapSize;               // +0x00
    long          RegionMapBox[4];                 // +0x04 C2DBoxI
    long          RegionMapSquare[4];              // +0x14 C2DBoxI
    CSurface      RegionMinimapSurface;            // +0x24
    CTexture      RegionMinimapTexture;            // +0x34
    float         IlluminationDirection[3];        // +0x3c C3DVector
    CCellVector   RegionMinimapCellList;           // +0x48
    unsigned long MaxRegionMinimapThemeColours;    // +0x54
    CColourVector RegionMinimapThemeColour;        // +0x58
    void InitialiseSurface()
    {
        long size = RegionMinimapSize;
        CPixelFormat format;
        format.Initialise(8, 8, 8, 0, true);
        C2DExtentsI extents;
        extents.X = size;
        extents.Y = size;
        RegionMinimapSurface.InitialiseAsImage(&extents, &format, false);
    }
    void InitialiseRegionBox();                                    // 0x0066C7B0
    void PopulateRegionMinimap(CWorldMap* worldMap);               // 0x0066CF00
    void BuildRegionMinimapTGA(const CWideString& filename);       // 0x0066CD50
    void CreateRegionMinimap(CWorldMap* worldMap, const CWideString& filename);
};
void CRegionMinimap::CreateRegionMinimap(CWorldMap* worldMap, const CWideString& filename)
{
    RegionMinimapCellList.resize(RegionMinimapSize * RegionMinimapSize);
    InitialiseSurface();
    InitialiseRegionBox();
    PopulateRegionMinimap(worldMap);
    BuildRegionMinimapTGA(filename);
    RegionMinimapCellList.Free();
}