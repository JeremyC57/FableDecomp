// NCameraModes::IsLandscapeToBlockCameraBetween(const CWorldMap& map, const C3DVector& a, const C3DVector& b, bool includeWater) -- landscape camera-blocking test: either end off the map or in an unloaded map blocks; otherwise blocked when a is above its camera height while b is below its own
// Leaf: ?IsLandscapeToBlockCameraBetween@NCameraModes@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00874F80, length 150 bytes, `__fastcall` (map in ecx, a in edx, b and the
//   flag on the stack), `ret 8`.
// Identity: Ego_r.pdb `?IsLandscapeToBlockCameraBetween@NCameraModes@@YI_NABVCWorldMap@@ABVC3DVector@@1_N@Z` (the
//   catalog name; a namespace function taking const references), the block-test callback the landed
//   CTools::GetLandscapePointIntersectionFromTo 0x00875550 passes to CTools::GetIntersectionWithWorldMap.
// Callees: CWorldMap::IsPosInMap(const C3DVector&) const @ 0x004FC600 (landed), CWorldMap virtual slots 16
//   (GetMapNumberAt(const C3DVector&), +0x40) and 10 (IsMapLoaded(long), +0x28),
//   NCameraModes::GetCameraHeightfieldHeightAtPos @
//   0x00874F40 (this chunk, `__fastcall`).
// Shape: the vtable is fetched once per end into a callee-saved register for both virtual calls; the two early `return
//   true` exits keep their own epilogues (before / after the ebp push); a's camera height is homed in b's dead argument
//   slot and b's stays on the x87 stack; `a->Z > heightA && heightB > b->Z` (`fcomp` against memory, `test ah,41h`).
class C3DVector {
public:
    float X, Y, Z;
};

class CWorldMap {
public:
    virtual ~CWorldMap();                               // slot 0
    virtual void DeleteAllLevels();                     // slot 1
    virtual void DeleteAllRegions();                    // slot 2
    virtual void LoadFromFile();                        // slot 3
    virtual void SaveToFile();                          // slot 4
    virtual void RemoveLevel(long map);                 // slot 5
    virtual void LoadTopologyFromFile();                // slot 6
    virtual void PostLoadTopology(long map);            // slot 7
    virtual void ForceLoad(long map);                   // slot 8
    virtual void ForceUnload(long map);                 // slot 9
    virtual bool IsMapLoaded(long map) const;           // slot 10 (+0x28)
    virtual bool IsMapUsed(long map) const;             // slot 11
    virtual long GetGranularity() const;                // slot 12
    virtual void GetDimensions() const;                 // slot 13
    virtual void GetMapAt();                            // slot 14
    virtual void PeekMapAt() const;                     // slot 15
    // overloaded virtuals are laid out in reverse declaration order (the C3DVector form is slot 16)
    virtual long GetMapNumberAt(const long* coord) const;     // slot 17 (+0x44, the C2DCoordI form)
    virtual long GetMapNumberAt(const C3DVector& pos) const;  // slot 16 (+0x40)
    bool IsPosInMap(const C3DVector& pos) const;        // 0x004FC600
};

namespace NCameraModes {
float __fastcall GetCameraHeightfieldHeightAtPos(const CWorldMap& map, const C3DVector& pos, bool includeWater);   // 0x00874F40

bool __fastcall IsLandscapeToBlockCameraBetween(const CWorldMap& map, const C3DVector& a, const C3DVector& b, bool includeWater)
{
    if (!map.IsPosInMap(a) || !map.IsMapLoaded(map.GetMapNumberAt(a)))
        return true;
    if (!map.IsPosInMap(b) || !map.IsMapLoaded(map.GetMapNumberAt(b)))
        return true;
    float heightA = GetCameraHeightfieldHeightAtPos(map, a, includeWater);
    float heightB = GetCameraHeightfieldHeightAtPos(map, b, includeWater);
    if (a.Z > heightA && heightB > b.Z)
        return true;
    return false;
}
}