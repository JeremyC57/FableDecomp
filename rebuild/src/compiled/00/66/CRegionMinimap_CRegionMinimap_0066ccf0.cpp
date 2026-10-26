// CRegionMinimap::~CRegionMinimap() -- free the theme-colour and cell vectors, uninitialise the minimap texture, then the minimap surface (its vtable stored, Uninitialise as a tail jump)
// Leaf: ??1CRegionMinimap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0066CCF0, length 58 bytes.
// Callees: free @ 0x00BFEA14 twice (the inline vector destructors), CTexture::Uninitialise @ 0x009F9F70 (the inline
// ~CTexture), CSurface::Uninitialise @ 0x009F2E20 (the inline ~CSurface, after its vtable 0x0122F84C is stored; tail jump).
// Evidence: the only caller is CMiniMapDisplay's destructor @ 0x00442610 (vtable 0x01231768), which destroys this
// member at +0x54; the PDB CRegionMinimap has RegionMinimapSurface (CSurface) at +0x24 and RegionMinimapTexture
// (CTexture) at +0x34 exactly where the body works. The catalog label "~CPatchTesselationEdgeStrip" is propagated.
// Layout: engine/CRegionMinimap.h holds the PDB layout with 16-byte vectors and opaque byte arrays for the members
// that have destructors, so it cannot model this body: the retail vectors are 12 bytes (RegionMinimapCellList +0x48,
// MaxRegionMinimapThemeColours +0x54, RegionMinimapThemeColour +0x58, retail size 0x64); a local model carries the PDB names.
// engine/CSurface.h and engine/CTexture.h are the PDB layouts as pack(1) PODs (the CSurface vptr as a data word, the
// CTexture bitfields as byte arrays); the inline destructors need a polymorphic CSurface, so local models carry the
// same members.
// Shape: members destroyed in reverse order; each vector `test ; je ; push ; call free ; add esp,4`; the CSurface member's
// inline destructor stores its vtable through `lea ecx,[esi+24h]` and its Uninitialise call becomes the tail jump.
extern "C" void __cdecl free(void* block);         // 0x00BFEA14

struct IDirect3DSurface9;
struct IDirect3DTexture9;

class CSurface { public:                                  // vtable 0x0122F84C, 0x10 bytes
    virtual void v00();
    IDirect3DSurface9* PD3DSurface;                // +0x04
    unsigned long      AllocationSource;           // +0x08
    void*              PAllocatedMemory;           // +0x0c
    void Uninitialise();                           // 0x009F2E20
    ~CSurface() { Uninitialise(); }
};

class CTexture { public:                                  // 8 bytes, not polymorphic
    IDirect3DTexture9* PD3DTexture;                // +0x00
    unsigned long      ByteLengthAndSource;        // +0x04 ByteLength:28, AllocationSource:4
    void Uninitialise();                           // 0x009F9F70
    ~CTexture() { Uninitialise(); }
};

template <class T>
struct CRetailVector {                             // retail std::vector of PODs: first / last / end
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    ~CRetailVector() { if (m_pFirst) free(m_pFirst); }
};

struct RegionMinimapCellData;
class CRGBColour { public: unsigned char B, G, R, A; };

struct CRegionMinimap {
    long          RegionMinimapSize;                         // +0x00
    long          RegionMapBox[4];                           // +0x04 C2DBoxI
    long          RegionMapSquare[4];                        // +0x14 C2DBoxI
    CSurface      RegionMinimapSurface;                      // +0x24
    CTexture      RegionMinimapTexture;                      // +0x34
    float         IlluminationDirection[3];                  // +0x3c C3DVector
    CRetailVector<RegionMinimapCellData> RegionMinimapCellList;   // +0x48
    unsigned long MaxRegionMinimapThemeColours;              // +0x54
    CRetailVector<CRGBColour> RegionMinimapThemeColour;      // +0x58
    ~CRegionMinimap();
};

CRegionMinimap::~CRegionMinimap()
{
}