// NCameraModes::IsThemeToBlockPassabilityBetween(const CWorldMap& map, const C3DVector& a, const C3DVector& b, bool flag) -- theme passability block test: blocked unless b's position is passable (a's passability is computed and tested, but both arms agree)
// Leaf: ?IsThemeToBlockPassabilityBetween@NCameraModes@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00874F10, length 42 bytes (+ int3 padding), `__fastcall` (map in ecx, a in
//   edx, b and the unused flag on the stack), `ret 8`.
// Identity: Ego_r.pdb names two theme block tests of this shape, `?IsThemeToBlockCameraBetween@NCameraModes@@` and
//   `?IsThemeToBlockPassabilityBetween@NCameraModes@@` (YI_NABVCWorldMap@@ABVC3DVector@@1_N@Z); the landed
//   CTools::GetThemePointIntersectionFromTo 0x008754A0 passes 0x00874EE0 when its flag is set and this one otherwise.
//   0x00874EE0 tests CWorldMap 0x004FCD60 (IsPosCameraPassable by elimination) and this one IsPosPassable 0x004FCC40,
//   so this is the passability test. The catalog label "CWorldMap_AreBothPosCameraBlocked_v2" is an auto label.
// Callee: CWorldMap::IsPosPassable(const C3DVector&) const @ 0x004FCC40 (landed), twice.
// Shape: both results are taken first (a's kept in bl); the per-arm `if (!passableB) return true;` blocks are
//   identical, so VC7.1 cross-jumps them and drops the branch on bl, leaving the dead `test bl,bl`; with the shared
//   `return false` after the if/else the exits stay `jne ; mov al,1 ; ret 8 ; xor al,al ; ret 8` (per-arm `return
//   false; return true;` pairs fold into `sete`, and `if (passableB) return false;` arms swap the exits).
class C3DVector {
public:
    float X, Y, Z;
};

class CWorldMap {
public:
    bool IsPosPassable(const C3DVector& pos) const;     // 0x004FCC40
};

namespace NCameraModes {
bool __fastcall IsThemeToBlockPassabilityBetween(const CWorldMap& map, const C3DVector& a, const C3DVector& b, bool flag)
{
    bool passableA = map.IsPosPassable(a);
    bool passableB = map.IsPosPassable(b);
    if (passableA) {
        if (!passableB)
            return true;
    }
    else {
        if (!passableB)
            return true;
    }
    return false;
}
}