// CTCInventoryMap::GetRelativePosOnMiniMap(CRegion& region, const C3DVector& position, C2DVector& relative) const -- place a world position on a region's mini-map graphic
// Grade: DIFFER(632v634) -- logic modelled (region map bounds, aspect scale, mini-map graphic size, offset); retail caches the copied map-id vector's start in ebx and its size in esi across the virtual calls (whole-program style caching; the lane reloads or hoists an explicit pointer local to the ctor), which shifts the frame by 4 and the register choice (5 grades)
#include <stdlib.h>
struct C2DCoordI {
    long X, Y;
    C2DCoordI() { }
    C2DCoordI(const C2DCoordI& o) : X(o.X), Y(o.Y) { }
};
struct C3DVector { float X, Y, Z; };
struct C2DVector { float X, Y; };
struct CLongVector {
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    CLongVector(const CLongVector& o);                  // 0x00514160
    ~CLongVector() { free(_M_start); }
    const long* begin() const { return _M_start; }
    const long* end() const { return _M_finish; }
    unsigned int size() const { return _M_finish - _M_start; }
    const long& operator[](unsigned int n) const { return _M_start[n]; }
};
struct CRegion {
    CLongVector Maps;                                   // +0x00
    unsigned char _pad0c[0x2c - 0x0c];
    float MiniMapScale;                                 // +0x2c
    C2DVector MiniMapOffset;                            // +0x30
};
struct CMap {
    unsigned char _pad[0x1d0c];
    long WorldSizeX;                                    // +0x1d0c
    long WorldSizeY;                                    // +0x1d10
    unsigned char _pad1d14[0x10];
    C2DCoordI WorldPos;                                 // +0x1d24
};
struct CWorldMap {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual const CMap& GetMap(long map) const;         // slot 19 (+0x4c)
};
struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12();
    virtual const CWorldMap* GetWorldMap() const;       // slot 13 (+0x34)
};
struct CGraphicDataBank {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20();
    virtual unsigned long GetGraphicFrameWidth(unsigned long index, unsigned long frame) const;    // slot 21 (+0x54)
    virtual unsigned long GetGraphicFrameHeight(unsigned long index, unsigned long frame) const;   // slot 22 (+0x58)
};
struct CMainGameComponent {
    unsigned char _pad00[0x161c8];
    CGraphicDataBank* GraphicDataBank;                  // +0x161c8
};
extern CMainGameComponent* g_pMainGameComponent;
inline CGraphicDataBank* GetGraphicDataBank() { return g_pMainGameComponent->GraphicDataBank; }
struct CTCInventoryBase {
    long GetMiniMapGraphic(const CRegion* region);      // 0x005BEB57
};
struct CTCInventoryMap : CTCInventoryBase {
    const CWorld* GetWorld() const;                     // 0x00686A80
    void GetRelativePosOnMiniMap(CRegion& region, const C3DVector& position, C2DVector& relative) const;
};

void CTCInventoryMap::GetRelativePosOnMiniMap(CRegion& region, const C3DVector& position, C2DVector& relative) const
{
    const CLongVector maps(region.Maps);
    const long* ids = maps.begin();
    C2DCoordI tl = GetWorld()->GetWorldMap()->GetMap(ids[0]).WorldPos;
    C2DCoordI br;
    br.X = GetWorld()->GetWorldMap()->GetMap(ids[0]).WorldSizeX + tl.X;
    br.Y = GetWorld()->GetWorldMap()->GetMap(ids[0]).WorldSizeY + tl.Y;
    unsigned int count = maps.end() - ids;
    for (unsigned int i = 1; i < count; ++i) {
        const CMap& map = GetWorld()->GetWorldMap()->GetMap(ids[i]);
        if (tl.X > map.WorldPos.X)
            tl.X = map.WorldPos.X;
        long top = map.WorldPos.Y;
        if (tl.Y > top)
            tl.Y = top;
        long right = map.WorldPos.X + map.WorldSizeX;
        if (br.X < right)
            br.X = right;
        long bottom = map.WorldSizeY + top;
        if (br.Y < bottom)
            br.Y = bottom;
    }
    relative.X = position.X - tl.X;
    long height = br.Y - tl.Y;
    relative.Y = position.Y - tl.Y;
    float scaleY = 1.0f;
    long width = br.X - tl.X;
    relative.X /= (float)width;
    relative.Y /= (float)height;
    float scaleX = 1.0f;
    if (width > height)
        scaleY = (float)height / (float)width;
    else if (width < height)
        scaleX = (float)width / (float)height;
    relative.X *= scaleX;
    relative.Y *= scaleY;
    float graphicWidth = (float)GetGraphicDataBank()->GetGraphicFrameWidth(const_cast<CTCInventoryMap*>(this)->GetMiniMapGraphic(&region), 0);
    float graphicHeight = (float)GetGraphicDataBank()->GetGraphicFrameHeight(const_cast<CTCInventoryMap*>(this)->GetMiniMapGraphic(&region), 0);
    relative.X = (relative.X * region.MiniMapScale - 0.5f) * graphicWidth;
    relative.Y = ((1.0f - relative.Y) * region.MiniMapScale - 0.5f) * graphicHeight;
    relative.X += region.MiniMapOffset.X * region.MiniMapScale;
    relative.Y += region.MiniMapOffset.Y * region.MiniMapScale;
}
