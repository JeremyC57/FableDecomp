// CTools::GetLandscapePointIntersectionFromTo(const C3DVector& from, const C3DVector& to, C3DVector& hitPos, C3DVector& hitNormal, bool includeWater) -- when the start point is below the ground (or water) height, snap it up and return the ground normal; otherwise intersect the segment with the world map using the landscape camera-blocking test
// Leaf: GetLandscapePointIntersectionFromTo
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00875550, length 197 bytes (`ret 14h`).
// Callees: CWorld::PeekWorldMap (CWorld slot 13, +0x34), CWorldMap::GetGroundSizeZAt @ 0x0063A300,
// CWorldMap::GetWaterSizeZAt @ 0x0063A420, CWorldMap::GetGroundFaceNormalAt @ 0x0063AB10 (C3DVector by hidden pointer),
// CTools::GetIntersectionWithWorldMap @ 0x008751D0 with IsLandscapeToBlockCameraBetween @ 0x00874F80 (`__fastcall`).
// Layout (Ego_r.pdb, as the landed CTools::FrameUpdate 0x00877D20): CTools CameraManager +0; CGameCameraManager World
// +0x74.
// Shape: the ground height is a named float homed in the dead `from` slot; the water height enters through a ternary
// over an inline by-value `HigherOf(water, ground)` (an if/assign stores and reloads the height; the ternary keeps it on
// the FPU for the compare and the `fstp` into hitPos.z); the fallback returns the call's bool via `setne`.
struct C3DVector {
    float x, y, z;
};
struct CWorldMap {
    float GetGroundSizeZAt(const C3DVector& pos) const;    // 0x0063A300
    float GetWaterSizeZAt(const C3DVector& pos) const;     // 0x0063A420
    C3DVector GetGroundFaceNormalAt(const C3DVector& pos) const;   // 0x0063AB10
};
struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual CWorldMap* PeekWorldMap();                      // slot 13 (+0x34)
};
struct CGameCameraManager {
    unsigned char _pad00[0x74];
    CWorld*       World;                                    // +0x74
};
typedef bool (__fastcall* TBlockTest)(CWorldMap* map, const C3DVector* a, const C3DVector* b, bool flag);
bool __fastcall IsLandscapeToBlockCameraBetween(CWorldMap* map, const C3DVector* a, const C3DVector* b, bool flag);   // 0x00874F80

inline float HigherOf(float candidate, float current) { return candidate > current ? candidate : current; }

struct CTools {
    CGameCameraManager* CameraManager;                      // +0x00
    bool GetIntersectionWithWorldMap(const C3DVector* from, const C3DVector* to, C3DVector* pos, C3DVector* normal, bool flag, TBlockTest test);   // 0x008751D0
    bool GetLandscapePointIntersectionFromTo(const C3DVector& from, const C3DVector& to, C3DVector& hitPos, C3DVector& hitNormal, bool includeWater);
};

bool CTools::GetLandscapePointIntersectionFromTo(const C3DVector& from, const C3DVector& to, C3DVector& hitPos, C3DVector& hitNormal, bool includeWater)
{
    CWorldMap* map = CameraManager->World->PeekWorldMap();
    float ground = map->GetGroundSizeZAt(from);
    float height = includeWater ? HigherOf(map->GetWaterSizeZAt(from), ground) : ground;
    if (height > from.z) {
        hitPos = from;
        hitPos.z = height;
        hitNormal = map->GetGroundFaceNormalAt(hitPos);
        return true;
    }
    return GetIntersectionWithWorldMap(&from, &to, &hitPos, &hitNormal, includeWater, IsLandscapeToBlockCameraBetween) ? true : false;
}
