// CEngineSkyRenderer::BuildBaseBandMesh(CTVertexColTex1**, unsigned short**, long, long) -- build the sky's base band: a centre vertex 10000 below the camera, a ring of 36 vertices of radius 6500 at depth 500 (the last one closing the ring at angle 0), and the 36-triangle fan joining them
// Leaf: ?BuildBaseBandMesh@CEngineSkyRenderer@@AAEXPAPAVCTVertexColTex1@@PAPAGJJ@Z
// Plan phase 5 (2026-11-05), CArray wave: the local CArray stand-in replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>). Also: the local CArray's pointer members Data are the header CArray's (public STLport vector<T>) begin() (STLport vector start +0 / finish +4).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B61CD0, 209 bytes (+ int3 padding), `ret 10h` (`this` unused).
// Callees: none (intrinsic fcos/fsin). Float pool: 0x0122DEDC = 0.0f, 0x012A2930 = 6500.0f, 0x012A2928 = 2*pi/35
//   (0.17951958f); -10000.0f and -500.0f are immediates.
// Layout: the vertex (24 bytes) {X, Y, Z, Colour +0xc, U +0x10, V +0x14}; both arrays are reached through their first word
//   (the data pointer).
// Shape: the angle lives on the x87 stack for the whole loop (`if (i == 35) angle = 0.0f;` reloads the zero); each ring
//   vertex's constants are stored before its cos/sin terms; the fan loop writes (base, base + j + 1, base + j) for j = 1..35
//   (a separate triangle counter, which VC turns into a count-down) with a running index, then the closing triangle (base,
//   base + 1, base + 36); the first vertex is the LAST argument (after the index array and the first index).
// NF256 (2026-11-22, nf254 L120 follow-on): self signature off the public CArray<SSkyVertex>& / CArray<unsigned short>& local model onto
//   the header's private ?BuildBaseBandMesh@CEngineSkyRenderer@@AAEXPAPAVCTVertexColTex1@@PAPAGJJ@Z (Ego_r.pdb + FableWin.pdb; the
//   caller 0x00B620A0 binds it; the body reads each array through its first word, i.e. the data pointer) with
//   FABLE_RETAIL_CENGINESKYRENDERER_BUILDBUFFERS_CALLEES; the vertex is engine/CTVertexColTex1.h (24 bytes, X/Y/Z, DiffuseColour, U1/V1).
#define FABLE_RETAIL_CENGINESKYRENDERER
#define FABLE_RETAIL_CENGINESKYRENDERER_BUILDBUFFERS_CALLEES
#include "engine/CEngineSkyRenderer.h"
#include "engine/CTVertexColTex1.h"
#include <math.h>
#pragma intrinsic(cos, sin)

void CEngineSkyRenderer::BuildBaseBandMesh(CTVertexColTex1** verts_ptr, unsigned short** indices_ptr, long index_offset, long vertex_offset)
{
    float angle = 0.0f;
    CTVertexColTex1* centre = &(*verts_ptr)[vertex_offset];
    centre->X = 0.0f;
    centre->Y = 0.0f;
    centre->Z = -10000.0f;
    centre->U1 = 0.0f;
    centre->V1 = 0.0f;
    centre->DiffuseColour = 0;
    CTVertexColTex1* vertex = centre + 1;
    for (unsigned long i = 0; i < 36; ++i) {
        if (i == 35)
            angle = 0.0f;
        CTVertexColTex1* ring = vertex++;
        ring->Z = -500.0f;
        ring->V1 = 0.0f;
        ring->U1 = 0.0f;
        ring->DiffuseColour = 0xffffffff;
        ring->X = (float)(cos(angle) * 6500.0f);
        ring->Y = (float)(sin(angle) * 6500.0f);
        angle += 0.17951958f;
    }
    unsigned short* index = &(*indices_ptr)[index_offset];
    long n = 0;
    unsigned long j = 1;
    for (long triangle = 0; triangle < 35; ++triangle, ++j) {
        index[n++] = (unsigned short)vertex_offset;
        index[n++] = (unsigned short)(vertex_offset + 1 + j);
        index[n++] = (unsigned short)(vertex_offset + j);
    }
    index[n++] = (unsigned short)vertex_offset;
    index[n++] = (unsigned short)(vertex_offset + 1);
    index[n++] = (unsigned short)(vertex_offset + j);
}
