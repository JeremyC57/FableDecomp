// NCameraModes::CTools::GetThemePointIntersectionFromTo(const C3DVector& from, const C3DVector& to, C3DVector& hitPos, C3DVector& hitNormal, bool useCameraBlocking) const -- for a segment with some horizontal extent, intersect it with the world map using the camera- or passability-blocking theme test (chosen by the flag) and copy out the hit position and normal
// Leaf: ?GetThemePointIntersectionFromTo@CTools@NCameraModes@@QBE_NABVC3DVector@@0AAV3@1_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008754A0, length 165 bytes (`ret 14h`; 176 with padding).
// Naming: Ego_r.pdb ?GetThemePointIntersectionFromTo@CTools@NCameraModes@@QBE_NABVC3DVector@@0AAV3@1_N@Z (CTools is
//   NCameraModes::CTools; the method is const). Callees with their PDB signatures: the private const
//   ?GetIntersectionWithWorldMap@CTools@NCameraModes@@ABE_NABVC3DVector@@0AAV3@1_NP6I_NABVCWorldMap@@002@Z@Z @ 0x008751D0
//   (from, to, hitPos, hitNormal, flag, a `__fastcall` bool(const CWorldMap&, const C3DVector&, const C3DVector&, bool)
//   test) and the two tests it is handed, NCameraModes::IsThemeToBlockCameraBetween @ 0x00874EE0 and
//   IsThemeToBlockPassabilityBetween @ 0x00874F10 (Ego_r.pdb ?IsThemeToBlock...Between@NCameraModes@@YI_NABVCWorldMap@@
//   ABVC3DVector@@1_N@Z; both landed under these names). FINDINGS chunk 404: the landed source declared the tests as
//   "CameraBlockTest_00874ee0/_00874f10" taking pointers and the class without its namespace.
// Global: the float epsilon 0x0129BA3C (0.0001f, an extern variable).
// Shape: `to - from` per component, `d = dx*dx; d += dy*dy;` keeps retail's dx-first squares; the two flag arms are
// separate calls that VC7.1 tail-merges after their last two pushes (the false arm's flag folds to 0); two uninitialised
// POD locals receive the hit.
class C3DVector {
public:
    float x, y, z;
};
class CWorldMap;
extern float g_fIntersectionEpsilon;                        // 0x0129BA3C (0.0001f)

namespace NCameraModes {
typedef bool (__fastcall* TBlockTest)(const CWorldMap& map, const C3DVector& from, const C3DVector& to, bool flag);
bool __fastcall IsThemeToBlockCameraBetween(const CWorldMap& map, const C3DVector& from, const C3DVector& to, bool flag);       // 0x00874EE0
bool __fastcall IsThemeToBlockPassabilityBetween(const CWorldMap& map, const C3DVector& from, const C3DVector& to, bool flag);  // 0x00874F10

class CTools {
public:
    bool GetThemePointIntersectionFromTo(const C3DVector& from, const C3DVector& to, C3DVector& hitPos, C3DVector& hitNormal,
                                         bool useCameraBlocking) const;
private:
    bool GetIntersectionWithWorldMap(const C3DVector& from, const C3DVector& to, C3DVector& pos, C3DVector& normal, bool flag,
                                     TBlockTest test) const;   // 0x008751D0
};

bool CTools::GetThemePointIntersectionFromTo(const C3DVector& from, const C3DVector& to, C3DVector& hitPos, C3DVector& hitNormal,
                                             bool useCameraBlocking) const
{
    float dx = to.x - from.x;
    float dy = to.y - from.y;
    float d = dx * dx;
    d += dy * dy;
    if (d > g_fIntersectionEpsilon * g_fIntersectionEpsilon) {
        C3DVector pos;
        C3DVector normal;
        bool hit;
        if (useCameraBlocking)
            hit = GetIntersectionWithWorldMap(from, to, pos, normal, useCameraBlocking, IsThemeToBlockCameraBetween);
        else
            hit = GetIntersectionWithWorldMap(from, to, pos, normal, useCameraBlocking, IsThemeToBlockPassabilityBetween);
        if (hit) {
            hitPos = pos;
            hitNormal = normal;
            return true;
        }
    }
    return false;
}
}
