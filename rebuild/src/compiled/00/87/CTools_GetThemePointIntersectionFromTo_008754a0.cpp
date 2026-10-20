// CTools::GetThemePointIntersectionFromTo(const C3DVector& from, const C3DVector& to, C3DVector& hitPos, C3DVector& hitNormal, bool flag) -- for a segment with some horizontal extent, intersect it with the world map using one of two camera-blocking tests (chosen by the flag) and copy out the hit position and normal
// Leaf: GetThemePointIntersectionFromTo
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008754A0, length 165 bytes (`ret 14h`; 176 with padding).
// Callees: CTools::GetIntersectionWithWorldMap(const C3DVector*, const C3DVector*, C3DVector*, C3DVector*, bool,
// test) @ 0x008751D0; the test callbacks 0x00874EE0 / 0x00874F10 (`__fastcall`, manifest
// "CWorldMap_AreBothPosCameraBlocked" / "_v2"). Global: the float epsilon 0x0129BA3C (0.0001f, an extern variable).
// Shape: `to - from` per component, `d = dx*dx; d += dy*dy;` keeps retail's dx-first squares; the two flag arms are
// separate calls that VC7.1 tail-merges after their last two pushes (the false arm's flag folds to 0); two uninitialised POD locals receive the hit.
struct C3DVector {
    float x, y, z;
};
struct CWorldMap;
typedef bool (__fastcall* TBlockTest)(CWorldMap* map, const C3DVector* a, const C3DVector* b, bool flag);
bool __fastcall CameraBlockTest_00874ee0(CWorldMap* map, const C3DVector* a, const C3DVector* b, bool flag);   // 0x00874EE0
bool __fastcall CameraBlockTest_00874f10(CWorldMap* map, const C3DVector* a, const C3DVector* b, bool flag);   // 0x00874F10
extern float g_fIntersectionEpsilon;                        // 0x0129BA3C (0.0001f)

struct CTools {
    bool GetIntersectionWithWorldMap(const C3DVector* from, const C3DVector* to, C3DVector* pos, C3DVector* normal, bool flag, TBlockTest test);   // 0x008751D0
    bool GetThemePointIntersectionFromTo(const C3DVector& from, const C3DVector& to, C3DVector& hitPos, C3DVector& hitNormal, bool flag);
};

bool CTools::GetThemePointIntersectionFromTo(const C3DVector& from, const C3DVector& to, C3DVector& hitPos, C3DVector& hitNormal, bool flag)
{
    float dx = to.x - from.x;
    float dy = to.y - from.y;
    float d = dx * dx;
    d += dy * dy;
    if (d > g_fIntersectionEpsilon * g_fIntersectionEpsilon) {
        C3DVector pos;
        C3DVector normal;
        bool hit;
        if (flag)
            hit = GetIntersectionWithWorldMap(&from, &to, &pos, &normal, flag, CameraBlockTest_00874ee0);
        else
            hit = GetIntersectionWithWorldMap(&from, &to, &pos, &normal, flag, CameraBlockTest_00874f10);
        if (hit) {
            hitPos = pos;
            hitNormal = normal;
            return true;
        }
    }
    return false;
}