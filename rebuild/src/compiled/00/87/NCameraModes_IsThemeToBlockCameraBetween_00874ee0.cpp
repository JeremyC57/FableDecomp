// NCameraModes::IsThemeToBlockCameraBetween(const CWorldMap&, const C3DVector&, const C3DVector&, bool) -- camera ray-cast callback: blocked when the end position is not camera-passable
// Leaf: ?IsThemeToBlockCameraBetween@
// Retail 0x00874EE0, 0x29 bytes (+ int3 padding), __fastcall (map in ecx, from in edx, to and the flag on the stack, `ret 8`).
// Manifest label "CWorldMap_AreBothPosCameraBlocked" is a descriptive auto label. Ego_r names the callback pair
// ?IsThemeToBlockCameraBetween@NCameraModes@@YI_NABVCWorldMap@@ABVC3DVector@@1_N@Z and ?IsThemeToBlockPassabilityBetween@...
// with the same signature; retail passes this one (0x00874EE0, the camera-passability test) or its twin 0x00874F10 (the
// 0x004FCC40 test) by address to NCameraModes::CTools::GetIntersectionWithWorldMap (push sites 0x008754E2 / 0x00877067).
// Callee: CWorldMap::IsPosCameraPassable(const C3DVector&) const @ 0x004FCD60 (both ends, unconditionally).
// Shape: both ends are tested into bool locals, then an if/else on the first whose two arms are identical in this build
// (`if (!passableTo) return true;` in each): VC keeps the dead `test bl,bl` of the removed branch before `test al,al ;
// jne`, and the shared `return false` keeps the two-exit `mov al,1 / xor al,al` tails (a single `return !to`-style
// arm folds to `sete al`). The flag argument is unused in this build.
class C3DVector { public: float X, Y, Z; };

class CWorldMap
{
public:
    bool IsPosCameraPassable(const C3DVector& pos) const;      // 0x004FCD60
};

namespace NCameraModes
{
    bool __fastcall IsThemeToBlockCameraBetween(const CWorldMap& map, const C3DVector& from, const C3DVector& to, bool flag);
}

bool __fastcall NCameraModes::IsThemeToBlockCameraBetween(const CWorldMap& map, const C3DVector& from, const C3DVector& to, bool flag)
{
    bool passableFrom = map.IsPosCameraPassable(from);
    bool passableTo = map.IsPosCameraPassable(to);
    if (passableFrom)
    {
        if (!passableTo)
            return true;
    }
    else
    {
        if (!passableTo)
            return true;
    }
    return false;
}