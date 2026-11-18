// CEngineLandscapeMeshBuilder::BuildSharedIndexBufferGenerationTables(C2DCoordI*, unsigned short*, unsigned short*, long, long, long&) -- number the corners of a width x height grid of alternating-diagonal quads (17x17 lookup), write each triangle and stitch them into one strip
// Grade: RELOCATION_MATCH (q.py, /GS) -- the tri-list block is one if/else whose arms each write poly_verts[0], [1], [2] in order (0x00BE2F74..0x00BE2F95: VC7.1 hoists the common [poly_count*3+2] store above the jne); with the +2 store written once before the if, c2 based the cursor at +1 and the whole colouring cascaded
// Leaf: ?BuildSharedIndexBufferGenerationTables@CEngineLandscapeMeshBuilder@@QAEXPAVC2DCoordI@@PAG1JJAAJ@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BE2CD0, 1088 bytes, thiscall `ret 0x18`.
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_GETASCARRAY_START
#define FABLE_RETAIL_CENGINELANDSCAPEMESHBUILDER_GENERATIONTABLES
#include <string.h>
#include "engine/CArray.h"
#include "engine/C2DCoordI.h"
#include "engine/CEngineLandscapeMeshBuilder.h"
#ifdef _MSC_VER
#pragma intrinsic(memset)
#endif

void CEngineLandscapeMeshBuilder::BuildSharedIndexBufferGenerationTables(C2DCoordI* verts, unsigned short* tri_striped_indicies,
    unsigned short* tri_list_indicies, long width, long height, long& tri_striped_index_count)
{
    __int64 overflow_check = (__int64)width * height * 6;
    if (overflow_check < 0 || overflow_check > 0x7fffffff)
        return;
    long vertex_id[17][17];
    memset(vertex_id, -1, sizeof(vertex_id));
    CArray<unsigned short> temp_tri_striped_buffer;
    if (tri_striped_indicies == 0) {
        temp_tri_striped_buffer.resize(width * height * 6);
        tri_striped_indicies = temp_tri_striped_buffer.GetAsCArray();
    }
    long ofs_table_x[2][2][3] = { { { 0, 1, 1 }, { 0, 0, 1 } }, { { 0, 1, 0 }, { 1, 0, 1 } } };
    long ofs_table_y[2][2][3] = { { { 0, 0, 1 }, { 0, 1, 1 } }, { { 0, 0, 1 }, { 0, 1, 1 } } };
    long poly_count = 0;
    long vertex_count = 0;
    tri_striped_index_count = 0;
    for (long x = 0; x < width; ++x) {
        for (long y = 0; y < height; ++y) {
            for (long poly_within_quad = 0; poly_within_quad < 2; ++poly_within_quad) {
                long odd_square = (x ^ y) & 1;
                unsigned short poly_verts[3];
                for (long i = 0; i < 3; ++i) {
                    long idx = ofs_table_x[odd_square][poly_within_quad][i] + x;
                    long idy = ofs_table_y[odd_square][poly_within_quad][i] + y;
                    long vert = vertex_id[idx][idy];
                    if (vert < 0) {
                        vert = vertex_count++;
                        verts[vert].X = idx;
                        verts[vert].Y = idy;
                        vertex_id[idx][idy] = vert;
                    }
                    poly_verts[i] = (unsigned short)vert;
                }
                if (tri_list_indicies) {
                    if (poly_within_quad == 1) {
                        tri_list_indicies[poly_count * 3] = poly_verts[0];
                        tri_list_indicies[poly_count * 3 + 1] = poly_verts[1];
                        tri_list_indicies[poly_count * 3 + 2] = poly_verts[2];
                    } else {
                        tri_list_indicies[poly_count * 3 + 1] = poly_verts[0];
                        tri_list_indicies[poly_count * 3] = poly_verts[1];
                        tri_list_indicies[poly_count * 3 + 2] = poly_verts[2];
                    }
                }
                ++poly_count;
                long odd_strip_length = tri_striped_index_count & 1;
                bool needs_order_flip = (poly_within_quad == odd_strip_length);
                if (needs_order_flip) {
                    if (tri_striped_index_count > 3) {
                        tri_striped_indicies[tri_striped_index_count] = tri_striped_indicies[tri_striped_index_count - 1];
                        tri_striped_indicies[tri_striped_index_count + 1] = poly_verts[0];
                        tri_striped_indicies[tri_striped_index_count + 2] = poly_verts[0];
                        tri_striped_indicies[tri_striped_index_count + 3] = poly_verts[0];
                        tri_striped_indicies[tri_striped_index_count + 4] = poly_verts[1];
                        tri_striped_indicies[tri_striped_index_count + 5] = poly_verts[2];
                        tri_striped_index_count += 6;
                    } else {
                        tri_striped_indicies[tri_striped_index_count] = poly_verts[0];
                        tri_striped_indicies[tri_striped_index_count + 1] = poly_verts[0];
                        tri_striped_indicies[tri_striped_index_count + 2] = poly_verts[1];
                        tri_striped_indicies[tri_striped_index_count + 3] = poly_verts[2];
                        tri_striped_index_count += 4;
                    }
                } else {
                    if (tri_striped_index_count > 2 && poly_verts[0] == tri_striped_indicies[tri_striped_index_count - 2] &&
                        poly_verts[1] == tri_striped_indicies[tri_striped_index_count - 1]) {
                        tri_striped_indicies[tri_striped_index_count] = poly_verts[2];
                        tri_striped_index_count += 1;
                    } else if (tri_striped_index_count < 3) {
                        tri_striped_indicies[tri_striped_index_count] = poly_verts[0];
                        tri_striped_indicies[tri_striped_index_count + 1] = poly_verts[1];
                        tri_striped_indicies[tri_striped_index_count + 2] = poly_verts[2];
                        tri_striped_index_count += 3;
                    } else {
                        tri_striped_indicies[tri_striped_index_count] = tri_striped_indicies[tri_striped_index_count - 1];
                        tri_striped_indicies[tri_striped_index_count + 1] = poly_verts[0];
                        tri_striped_indicies[tri_striped_index_count + 2] = poly_verts[0];
                        tri_striped_indicies[tri_striped_index_count + 3] = poly_verts[1];
                        tri_striped_indicies[tri_striped_index_count + 4] = poly_verts[2];
                        tri_striped_index_count += 5;
                    }
                }
            }
        }
    }
}
