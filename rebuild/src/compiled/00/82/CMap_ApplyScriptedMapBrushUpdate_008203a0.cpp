// CMap::ApplyScriptedMapBrushUpdate(float amount, CScriptedMapBrush* brush)
// Leaf: ?ApplyScriptedMapBrushUpdate@CMap@
extern "C" double __cdecl pow(double x, double y);
extern "C" double __cdecl sqrt(double x);
extern "C" double __cdecl fabs(double x);
#pragma intrinsic(pow, sqrt, fabs)
inline float fabs(float x) { return (float)fabs((double)x); }
extern const float g_zero_0122dedc;               // 0x0122DEDC (0.0f)
extern const float g_maxSize_012724c0;            // 0x012724C0 (2048.0f)
extern const float g_epsilon_0129ba3c;            // 0x0129BA3C (1e-4f)

struct C2DCoordI {
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) {}
    C2DCoordI operator-(const C2DCoordI& o) const { return C2DCoordI(X - o.X, Y - o.Y); }
    C2DCoordI operator/(long d) const { return C2DCoordI(X / d, Y / d); }
};
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
    explicit C2DVector(const C2DCoordI& c) : X((float)c.X), Y((float)c.Y) {}
    float GetLength() const { return (float)sqrt(X * X + Y * Y); }
};
struct C2DBoxI {
    long TLX, TLY, BRX, BRY;
    C2DBoxI() {}
    C2DBoxI(long tlx, long tly, long brx, long bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) {}
};
struct CScriptedMapBrush {
    const C2DBoxI& GetGameArea() const;                                   // 0x0082E280
    float GetGameHeight(long x, long y) const;                            // 0x0082E290
    unsigned long GetTheme(long x, long y, unsigned long layer) const;    // 0x0082E330
    unsigned char GetThemeBlend(long x, long y, unsigned long layer) const;   // 0x0082E3D0
};
struct CHeightMapCell {                            // PDB, 12 bytes
    float         Height;                          // +0x00
    unsigned char EngineThemePaletteIndex[3];      // +0x04
    unsigned char EngineBlend[2];                  // +0x07
    unsigned char Flags;                           // +0x09
    unsigned char _pad0a[2];
};
template <class T> struct CRetailVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T& operator[](unsigned long n) { return *(_M_start + n); }
};
struct CHeightMap {
    virtual ~CHeightMap();
    void*                          World;          // +0x04
    void*                          Map;            // +0x08
    unsigned long*                 WorldSeed;      // +0x0c
    CRetailVector<CHeightMapCell>  Cells;          // +0x10
    unsigned char                  _MapUpdateAreas[0x1e8];
    long                           SizeX;          // +0x204
    long                           SizeY;          // +0x208
    __forceinline CHeightMapCell& GetCellIncludingExtraCells(long x, long y) { return Cells[(SizeY - y) * (SizeX + 1) + x]; }
    void SetSizeZAt(const C2DCoordI& cell, float size)   // out-of-line copy 0x00825440
    {
        float value;
        if (size < g_zero_0122dedc) {
            value = g_zero_0122dedc;
        } else {
            float limit = g_maxSize_012724c0 - g_epsilon_0129ba3c;
            value = size > limit ? limit : size;
        }
        GetCellIncludingExtraCells(cell.X, cell.Y).Height = value;
    }
};
template <class T> struct scoped_ptr {
    T* ptr;
    T* operator->() const { return ptr; }
};
struct CMapRegionTable {
    virtual void r00(); virtual void r01(); virtual void r02(); virtual void r03(); virtual void r04(); virtual void r05();
    virtual void r06(); virtual void r07(); virtual void r08(); virtual void r09(); virtual void r10(); virtual void r11();
    virtual void r12(); virtual void r13(); virtual void r14(); virtual void r15(); virtual void r16();
    virtual long GetRegionIndex(const void* key);  // slot 17 (+0x44)
    C2DBoxI GetWorldBox(long index, const C2DBoxI& box);   // 0x004FC560
};
struct CMapUpdateSink {
    virtual void u00(); virtual void u01(); virtual void u02(); virtual void u03(); virtual void u04(); virtual void u05();
    virtual void u06(); virtual void u07(); virtual void u08(); virtual void u09(); virtual void u10(); virtual void u11();
    virtual void u12(); virtual void u13(); virtual void u14(); virtual void u15(); virtual void u16(); virtual void u17();
    virtual void u18(); virtual void u19(); virtual void u20(); virtual void u21(); virtual void u22(); virtual void u23();
    virtual void u24(); virtual void u25(); virtual void u26(); virtual void u27();
    virtual void AddUpdateArea(const C2DBoxI& box);   // slot 28 (+0x70)
};
struct CMapOwner {
    virtual void o00(); virtual void o01(); virtual void o02(); virtual void o03(); virtual void o04(); virtual void o05();
    virtual void o06(); virtual void o07(); virtual void o08(); virtual void o09(); virtual void o10(); virtual void o11();
    virtual CMapUpdateSink* GetUpdateSink();       // slot 12 (+0x30)
    virtual CMapRegionTable* GetRegionTable();     // slot 13 (+0x34)
};
struct CMap {
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03(); virtual void m04(); virtual void m05();
    virtual void m06(); virtual void m07(); virtual void m08(); virtual void m09();
    virtual void UpdateBrushHeights(CScriptedMapBrush* brush);   // slot 10 (+0x28)
    virtual void UpdateBrushBlends(CScriptedMapBrush* brush);    // slot 11 (+0x2c)
    virtual void UpdateBrushThemes(CScriptedMapBrush* brush);    // slot 12 (+0x30)
    unsigned char           _pad04[0x20 - 0x04];
    CMapOwner*              POwner;                // +0x20
    unsigned char           _pad24[0x1a7c - 0x24];
    scoped_ptr<CHeightMap>  HeightMap;             // +0x1a7c
    unsigned char           _pad1a80[0x1d24 - 0x1a80];
    unsigned char           RegionKey[4];          // +0x1d24
    unsigned char AddThemeDefIndexToPalette(unsigned long themeDefIndex);   // 0x008F70F0
    __forceinline void SetEngineThemeAtIncludingExtraCells(long x, long y, unsigned long themeDefIndex, long layer)   // out-of-line copy 0x0081EF30
    {
        if (themeDefIndex > 0) {
            long paletteIndex = AddThemeDefIndexToPalette(themeDefIndex);
            HeightMap->GetCellIncludingExtraCells(x, y).EngineThemePaletteIndex[layer] = (unsigned char)paletteIndex;
        } else
            HeightMap->GetCellIncludingExtraCells(x, y).EngineThemePaletteIndex[layer] = 0;
    }
    void SetEngineBlendAtIncludingExtraCells(long x, long y, unsigned char blend, long layer)
    {
        HeightMap->GetCellIncludingExtraCells(x, y).EngineBlend[layer] = blend;
    }
    void ApplyScriptedMapBrushUpdate(float amount, CScriptedMapBrush* brush);
};

void CMap::ApplyScriptedMapBrushUpdate(float amount, CScriptedMapBrush* brush)
{
    const C2DBoxI& area = brush->GetGameArea();
    C2DBoxI box = area;
    ++box.TLX;
    ++box.TLY;
    long centreX = box.BRX - (box.BRX - box.TLX) / 2;
    long centreY = box.BRY - (box.BRY - box.TLY) / 2;
    float invRadius = 1.0f / C2DVector((float)(centreX - box.BRX), (float)(centreY - box.BRY)).GetLength();
    if (fabs(amount - 1.0f) > 1e-4f) {
        for (long y = box.TLY; y < box.BRY; ++y) {
            for (long x = box.TLX; x < box.BRX; ++x) {
                float distance = C2DVector((float)(x - centreX), (float)(y - centreY)).GetLength() * invRadius;
                float falloff = 1.0f - distance;
                falloff = falloff * falloff;
                float weight = falloff * amount;
                double p3 = pow(amount, 3.0);
                double p4 = pow(amount, 4.0);
                weight = weight * (float)p3 + (float)p4;
                if (weight > 1.0f)
                    weight = 1.0f;
                float oldHeight = HeightMap->GetCellIncludingExtraCells(x, y).Height;
                float height = brush->GetGameHeight(x, y) * weight + (1.0f - weight) * oldHeight;
                HeightMap->SetSizeZAt(C2DCoordI(x, y), height);
            }
        }
    } else {
        for (long y = box.TLY; y < box.BRY; ++y) {
            for (long x = box.TLX; x < box.BRX; ++x) {
                float height = brush->GetGameHeight(x, y);
                HeightMap->SetSizeZAt(C2DCoordI(x, y), height);
                for (long layer = 0; layer < 3; ++layer)
                    SetEngineThemeAtIncludingExtraCells(x, y, brush->GetTheme(x, y, layer), layer);
                for (long blend = 0; blend < 2; ++blend)
                    SetEngineBlendAtIncludingExtraCells(x, y, brush->GetThemeBlend(x, y, blend), blend);
            }
        }
        UpdateBrushHeights(brush);
        UpdateBrushThemes(brush);
        UpdateBrushBlends(brush);
    }
    long region = POwner->GetRegionTable()->GetRegionIndex(RegionKey);
    C2DBoxI worldBox = POwner->GetRegionTable()->GetWorldBox(region, brush->GetGameArea());
    POwner->GetUpdateSink()->AddUpdateArea(worldBox);
}