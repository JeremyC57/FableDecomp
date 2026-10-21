// CRegionMinimap::IlluminateRegionMinimapTheme(const CWorldMap*, const C3DVector&) -- light intensity of the theme at a position (dot of the light direction)
// Grade: DIFFER(ours v retail) -- 348v354: pop esi placement plus x87 liveness of light.Z (retail keeps it alive for `fmul st(1)` and pops at the end)
#include "engine/CRegionMinimap.h"
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558

struct C3DVector {
    float X, Y, Z;
    __forceinline void Normalise()
    {
        float lengthSquared = X * X + Y * Y + Z * Z;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r * 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
    float Dot(const C3DVector& v) const { return X * v.X + Y * v.Y + Z * v.Z; }
};

struct CWorldMap {
    C3DVector GetGroundFaceNormalAt(const C3DVector& pos) const;   // 0x0063AB10
};

struct CRegionMinimapX : CRegionMinimap {
    float IlluminateRegionMinimapTheme(const CWorldMap* map, const C3DVector& pos);
};

float CRegionMinimapX::IlluminateRegionMinimapTheme(const CWorldMap* map, const C3DVector& pos)
{
    C3DVector normal = map->GetGroundFaceNormalAt(pos);
    C3DVector light = *(const C3DVector*)&IlluminationDirection_X;
    normal.Normalise();
    light.Normalise();
    return normal.Dot(light);
}
