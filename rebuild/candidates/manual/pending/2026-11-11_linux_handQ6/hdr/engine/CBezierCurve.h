#ifndef FABLE_ENGINE_CBEZIERCURVE_H
#define FABLE_ENGINE_CBEZIERCURVE_H
// NEW header (chunk Q6, row 0x00912840). Ego_r.pdb CBezierCurve (ghidra_out/struct_layouts_egor.tsv: 48 bytes, Point1..Point4 C3DVector;
// class key V from ?AddRandomBezierCurveToPath@CAIStateGroup_FlyingThingBase@@IAE_NAAV?$deque@VCBezierCurve@@...). No virtuals, no
// out-of-line members in either PDB name set; retail deque<CBezierCurve> holds two 0x30-byte curves per 0x60 node.
#include "rebuild_abi.h"
#include "engine/C3DVector.h"
class CBezierCurve {
public:
    C3DVector Point1;                                                    // +0x00
    C3DVector Point2;                                                    // +0x0c
    C3DVector Point3;                                                    // +0x18
    C3DVector Point4;                                                    // +0x24
};
FABLE_STATIC_ASSERT(sizeof(CBezierCurve) == 0x30);
#endif // FABLE_ENGINE_CBEZIERCURVE_H
