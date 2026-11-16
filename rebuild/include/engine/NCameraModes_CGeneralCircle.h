#ifndef FABLE_ENGINE_NCAMERAMODES_CGENERALCIRCLE_H
#define FABLE_ENGINE_NCAMERAMODES_CGENERALCIRCLE_H
// NCameraModes::CGeneralCircle (agentC42 after agentC40's outline, row 0x00875690): Ego_r.pdb TPI, 40 bytes: C3DVector Origin +0x00,
// float Radius +0x0C, C3DVector Ei +0x10, C3DVector Ej +0x1C (the circle through three points: centre, radius and two unit axes of
// its plane). Constructors: Ego_r.pdb / FableWin.pdb ??0CGeneralCircle@NCameraModes@@QAE@XZ and
// ??0CGeneralCircle@NCameraModes@@QAE@ABVC3DVector@@00@Z (0x00875690). Declarations only.
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/C3DVector.h"
namespace NCameraModes
{
class CGeneralCircle {
public:
    CGeneralCircle();
    CGeneralCircle(const C3DVector& point_a, const C3DVector& point_b, const C3DVector& point_c);   // 0x00875690 (parameter names descriptive)
    C3DVector Origin;                                                    // +0x00
    float     Radius;                                                    // +0x0c
    C3DVector Ei;                                                        // +0x10
    C3DVector Ej;                                                        // +0x1c
};
FABLE_STATIC_ASSERT(offsetof(CGeneralCircle, Radius) == 0x0c);
FABLE_STATIC_ASSERT(offsetof(CGeneralCircle, Ej) == 0x1c);
FABLE_STATIC_ASSERT(sizeof(CGeneralCircle) == 0x28);
}
#endif
