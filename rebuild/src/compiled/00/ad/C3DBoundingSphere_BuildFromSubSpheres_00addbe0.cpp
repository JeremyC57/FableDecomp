// C3DBoundingSphere::BuildFromSubSpheres(const C3DBoundingSphere* spheres, long count) -- build the sphere that encloses `count`
// sub-spheres: take the axis-aligned box around them (each sphere's centre -/+ its radius), centre on the middle of that box, then
// grow the radius from 0 to the largest (distance from the new centre to a sub-sphere's centre + that sub-sphere's radius).
// Retail 0x00ADDBE0, 414 bytes, __fastcall, `ret 8`. Callees: none (sqrt is the fsqrt intrinsic). Constants: -1e30f at 0x0129DCA4
// (maxZ lives in st0 for the whole first loop), 0.5f at 0x0122F59C. Only caller: CQuadTreeElement::
// CalcBoundingSphereAndFadeDistanceFromChildren (0x00BDF34E).
// Leaf: ?BuildFromSubSpheres@C3DBoundingSphere@@QAEXPBV1@J@Z
// Shape notes (each one is a byte fact, see NOTES.md):
//  - the six box bounds and the three centre components are separate float locals: retail's frame is 5 floats (0x14) with maxZ kept
//    on the x87 stack, and the centre is homed in the two dead argument slots and minZ's slot. C3DVector locals keep 12-byte
//    aggregates on the frame (0x20 / 0x24) and copy `Center` through a second pointer (`mov eax,ecx`);
//  - both loops are the plain counted `for` over spheres[i] with every bound expression spelled twice (test, then store): that body
//    stays rolled (`mov edi,esi ... dec edi ; jne`). A single `float v` temporary, a reference or a walking pointer makes VC7.1
//    unroll the loop by four (1088..1104 bytes);
//  - the radius is read through the inline accessor GetRadius() in the first loop: that gives `fld Center.c ; fadd Radius` for the
//    upper bounds. With the member spelled directly VC7.1 loads the radius first (`fld Radius ; fadd Center.c`), whichever way round
//    the sum is written;
//  - the distance is `dy*dy + (dx*dx + dz*dz)`: retail squares Z, then X, adds, then Y (0x00ADDD44..0x00ADDD52).
#define FABLE_RETAIL_C3DBOUNDINGSPHERE_BUILDFROMSUBSPHERES
#include "engine/C3DBoundingSphere.h"

void C3DBoundingSphere::BuildFromSubSpheres(const C3DBoundingSphere* spheres, long count)
{
    float minX = 1e30f, minY = 1e30f, minZ = 1e30f;
    float maxX = -1e30f, maxY = -1e30f, maxZ = -1e30f;
    long i;
    for (i = 0; i < count; ++i) {
        if (minX > spheres[i].Center.X - spheres[i].GetRadius()) minX = spheres[i].Center.X - spheres[i].GetRadius();
        if (minY > spheres[i].Center.Y - spheres[i].GetRadius()) minY = spheres[i].Center.Y - spheres[i].GetRadius();
        if (minZ > spheres[i].Center.Z - spheres[i].GetRadius()) minZ = spheres[i].Center.Z - spheres[i].GetRadius();
        if (maxX < spheres[i].Center.X + spheres[i].GetRadius()) maxX = spheres[i].Center.X + spheres[i].GetRadius();
        if (maxY < spheres[i].Center.Y + spheres[i].GetRadius()) maxY = spheres[i].Center.Y + spheres[i].GetRadius();
        if (maxZ < spheres[i].Center.Z + spheres[i].GetRadius()) maxZ = spheres[i].Center.Z + spheres[i].GetRadius();
    }
    Radius = 0.0f;
    float cx = (maxX + minX) * 0.5f, cy = (maxY + minY) * 0.5f, cz = (maxZ + minZ) * 0.5f;
    Center.X = cx;
    Center.Y = cy;
    Center.Z = cz;
    for (i = 0; i < count; ++i) {
        float dx = spheres[i].Center.X - cx;
        float dy = spheres[i].Center.Y - cy;
        float dz = spheres[i].Center.Z - cz;
        float reach = (float)sqrt(dy * dy + (dx * dx + dz * dz)) + spheres[i].Radius;
        if (reach > Radius) Radius = reach;
    }
}