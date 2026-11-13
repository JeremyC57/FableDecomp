// NCameraModes::CTools::TestMeshForOccupationAndGetType(const CPhysicsMeshObject& mesh, const C3DVector& position, const C3DVector& up,
//   float& height) const -- probes the mesh around `position`: size = squared X-edge + squared Y-edge of the mesh bounding box;
//   normal = up normalised, side = normal x up-axis normalised, both scaled by size. Two vertical probes (up then down, 3*size):
//   both must hit (else type 0), `height` takes the hit Z. Then four probes rotated a quarter turn apart around the axis
//   (position + normal*cos + side*sin). Returns 1 (occupied) when five or six probes hit, else 0.
// Leaf: ?TestMeshForOccupationAndGetType@CTools@NCameraModes@@QBE?BW4EMeshType@2@ABVCPhysicsMeshObject@@ABVC3DVector@@1AAM@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00875B10, 838 bytes, `ret 10h` (`this` unused). Ego_r.pdb / FableWin.pdb mangled name as the
//   Leaf line: the return type is the const NAMESPACE-scope enum NCameraModes::EMeshType (`W4EMeshType@2@`, back-reference 2 =
//   NCameraModes), not a CTools-nested one. Its enumerator names are found nowhere; MESH_TYPE_NONE / MESH_TYPE_OCCUPIED are
//   descriptive (values 0 / 1 from the two epilogues).
// Callees: CPhysicsMeshObject::GetBoundingBoxXEdge() const 0x00A11C20, GetBoundingBoxYEdge() const 0x00A11C90 (by value),
//   CPhysicsMeshObject::GetPointIntersectionFromTo(const C3DVector&, const C3DVector&, CMeshIntersectionData&) const 0x00A12450,
//   __ftol2 0x00BFEA70. Inline: C3DVector::Normalise (GFFastOneOverSqrt, table GFastOneOverSqrtTable 0x013CE558), CrossWithUp,
//   operator*, operator+, operator*=. Constants 3.0f 0x0122DED4, 0.5f 0x0122F59C, 0.0f 0x0122DEDC, 0.25f 0x0123078C,
//   2*pi (double) 0x0124F2B8.
// Shape: the vertical probe is `position + C3DVector(0, 0, dir*size*3)` (the X/Y int copies of position come after __ftol2,
//   0x00875D24); the end is a switch with separate case 5 / case 6 returns (`sub ebp,5 ; je ; dec ebp ; jne`).
//   Needs the header opt-ins FABLE_RETAIL_NCAMERAMODES_CTOOLS_MESHTYPE (declaration) and FABLE_RETAIL_C3DVECTOR_SCALE_NAMED_RESULT (x87 scheduling of the loop-2 Y product, 0x00875DD3).
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_PAREN_SQUARES_ZYX
#define FABLE_RETAIL_C3DVECTOR_CROSSWITHUP
#define FABLE_RETAIL_C3DVECTOR_SCALE_NAMED_RESULT
#define FABLE_RETAIL_CPHYSICSMESHOBJECT_EDGES
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS_MESHTYPE
#include <math.h>
#pragma intrinsic(sin, cos)
#include "engine/C3DVector.h"
#include "engine/CMeshIntersectionData.h"
#include "engine/CPhysicsMeshObject.h"
#include "engine/NCameraModes_CTools.h"

const NCameraModes::EMeshType NCameraModes::CTools::TestMeshForOccupationAndGetType(const CPhysicsMeshObject& mesh, const C3DVector& position, const C3DVector& up, float& height) const
{
    float size = mesh.GetBoundingBoxXEdge().GetSquaredMagnitude() + mesh.GetBoundingBoxYEdge().GetSquaredMagnitude();
    C3DVector normal = up;
    normal.Normalise();
    C3DVector side = normal.CrossWithUp();
    side.Normalise();
    normal *= size;
    side *= size;
    long hits = 0;
    CMeshIntersectionData intersection;
    for (long i = 0; i < 2; ++i) {
        C3DVector to = position + C3DVector(0.0f, 0.0f, (float)(long)((0.5f - (float)i) * 2.0f) * size * 3.0f);
        if (!mesh.GetPointIntersectionFromTo(position, to, intersection))
            return MESH_TYPE_NONE;
        height = intersection.IntersectionPosition.Z;
        ++hits;
    }
    for (long j = 0; j < 4; ++j) {
        double angle = (float)j * 0.25f * 6.283185307179586;
        C3DVector to = position + normal * (float)cos(angle) + side * (float)sin(angle);
        if (mesh.GetPointIntersectionFromTo(position, to, intersection))
            ++hits;
    }
    switch (hits) {
    case 5:
        return MESH_TYPE_OCCUPIED;
    case 6:
        return MESH_TYPE_OCCUPIED;
    default:
        return MESH_TYPE_NONE;
    }
}