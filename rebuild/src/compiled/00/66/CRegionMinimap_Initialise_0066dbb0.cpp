// CRegionMinimap::Initialise() -- reset the region map box to unset (-1), release the surface and texture, light from (1,1,1) and copy the GUI definition's minimap theme colours
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0066DBB0, length 157 bytes.
// Callees: CSurface::Uninitialise @ 0x009F2E20, CTexture::Uninitialise @ 0x009F9F70, PeekGuiDef @ 0x00438570
// (`__fastcall`, no arguments, as in the landed NPlayerGui rows), vector<CRGBColour>::resize(n, const T&) @ 0x00468FA5
// (catalog label "CArray<pair<long,long>>::push_back" is a propagated byte twin).
// Layout: the retail CRegionMinimap of the landed constructor 0x0066CCC0 (PDB names, 12-byte vectors):
// RegionMapBox +0x04, RegionMinimapSurface +0x24, RegionMinimapTexture +0x34, IlluminationDirection +0x3c,
// MaxRegionMinimapThemeColours +0x54, RegionMinimapThemeColour +0x58. CPlayerGuiDef (retail offsets; descriptive
// names): the minimap theme colour count +0x99c and the colour vector +0x9ac.
// Shape: the box is reset edge by edge in member order with one -1 register (a chained assignment stores BRY first); the light direction is assigned from a C3DVector(1,1,1) temporary
// (frame stores reloaded, `lea eax,[esi+3Ch]`: implicit operator= on a class with a user copy constructor); the
// resize takes an empty-constructed colour temporary; the copy loop indexes both vectors through `*(begin() + i)`
// and re-reads the count each iteration.
// Leaf: Initialise@CRegionMinimap
struct IDirect3DSurface9;
struct IDirect3DTexture9;

class CSurface { public:
    IDirect3DSurface9* PD3DSurface;                     // +0x04
    unsigned long      AllocationSource;                // +0x08
    void*              PAllocatedMemory;                // +0x0c
    virtual ~CSurface();                                // slot 0 (0x0041C9C0)
    void Uninitialise();                                // 0x009F2E20
};

class CTexture { public:
    IDirect3DTexture9* PD3DTexture;                     // +0x00
    unsigned long      ByteLengthAndSource;             // +0x04 ByteLength:28, AllocationSource:4
    void Uninitialise();                                // 0x009F9F70
};

template<class T> struct CRetailVector {                // retail std::vector: three pointers
    T* m_first; T* m_last; T* m_end;
    T* begin() { return m_first; }
    const T* begin() const { return m_first; }
    T& operator[](unsigned long n) { return *(begin() + n); }
    const T& operator[](unsigned long n) const { return *(begin() + n); }
    void resize(unsigned long n, const T& value);       // 0x00468FA5 (vector<CRGBColour>)
};

class C2DBoxI { public: long TLX, TLY, BRX, BRY; };
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour() {}
};
struct RegionMinimapCellData;

class CPlayerGuiDef { public:
    unsigned char             _pad[0x99c];
    unsigned long             NoRegionMinimapThemeColours;   // +0x99c
    unsigned char             _pad9a0[0xc];
    CRetailVector<CRGBColour> RegionMinimapThemeColours;     // +0x9ac
};
class CPlayerGui { public: static const CPlayerGuiDef& __fastcall PeekGuiDef(); };                 // 0x00438570

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
    void Initialise();
};

void CRegionMinimap::Initialise()
{
    RegionMapBox.TLX = -1;
    RegionMapBox.TLY = -1;
    RegionMapBox.BRX = -1;
    RegionMapBox.BRY = -1;
    RegionMinimapSurface.Uninitialise();
    RegionMinimapTexture.Uninitialise();
    IlluminationDirection = C3DVector(1.0f, 1.0f, 1.0f);
    const CPlayerGuiDef* def = &CPlayerGui::PeekGuiDef();
    MaxRegionMinimapThemeColours = def->NoRegionMinimapThemeColours;
    RegionMinimapThemeColour.resize(MaxRegionMinimapThemeColours, CRGBColour());
    for (unsigned long i = 0; i < MaxRegionMinimapThemeColours; ++i)
        RegionMinimapThemeColour[i] = def->RegionMinimapThemeColours[i];
}