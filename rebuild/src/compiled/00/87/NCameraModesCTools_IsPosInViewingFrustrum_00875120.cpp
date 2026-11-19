// NCameraModes::CTools::IsPosInViewingFrustrum(const C3DVector& pos, const C3DVector& look_at, const C3DVector& camera, float fov) const -- true when the direction camera->pos lies inside the cone of half-angle fov/2 (fov as a fraction of a turn) around camera->look_at
// Leaf: ?IsPosInViewingFrustrum@CTools@NCameraModes@@QBE_NABVC3DVector@@00M@Z
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00875120, length 164 bytes (`ret 10h`, `this` unused; 176 with padding).
// Identity: Ego_r.pdb 0x0085B8F0 and FableWin.pdb 0x029C2520 ?IsPosInViewingFrustrum@CTools@NCameraModes@@QBE_NABVC3DVector@@00M@Z
//   (const member of the camera helper, bool); engine/NCameraModes_CTools.h FRUSTRUM_POS declares it with the PDB parameter names.
//   Callers 0x00697F50 (CGameCameraManager::IsPosInViewingFrustrum) and 0x00697F80 (Is2DAreaInViewingFrustrum, five calls).
//   Was a local `struct CTools` with a long result (q11337 moved it onto the shared header).
// Callees: C3DVector::AccurateNormalise @ 0x00A14440 (x2). Constants: 0x0122F59C = 0.5f, 0x0124F2B8 = (double)6.2831855f.
// Shape: the two differences are inline C3DVector operator- results built straight in their locals (user
// constructor); the dot product is written y, x, z so VC7.1's right-operand-first FPU order gives retail's z, x, y
// loads; `cos` is the intrinsic over `fov * 0.5f * (double)TWO_PI`. The bool is `return dot > cos(...);`, which VC7.1
// materialises full-register with two exits (`mov eax,1` / `xor eax,eax`, 0x008751B1 / 0x008751BC; as `return a && b;` in
// 0x0083B9F0); an if / return true / return false gives `mov al,1` / `xor al,al` (DIFFER 161v164).
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS_FRUSTRUM_POS
#include <math.h>
#pragma intrinsic(cos)
#include "engine/C3DVector.h"
#include "engine/NCameraModes_CTools.h"
static const float TWO_PI = 6.28318548f;

bool NCameraModes::CTools::IsPosInViewingFrustrum(const C3DVector& pos, const C3DVector& look_at, const C3DVector& camera, float fov) const
{
    C3DVector view = look_at - camera;
    view.AccurateNormalise();
    C3DVector dir = pos - camera;
    dir.AccurateNormalise();
    return view.Y * dir.Y + view.X * dir.X + view.Z * dir.Z > cos(fov * 0.5f * (double)TWO_PI);
}
