// 00f3e6b0 CTriangleStripifier2::BuildVertexAndPolyMaps
// Flags: /Gr
// Leaf: ?BuildVertexAndPolyMaps@CTriangleStripifier2@@AAEXABV?$CArray@G@@AAV?$CArray@V?$CArray@J@@@@AAV?$CArray@V?$CArray@G@@@@@Z
#define FABLE_RETAIL_PASS22_TRIANGLE_ORIGINAL_COMPLETE_DECLARATIONS
#define FABLE_RETAIL_CARRAY_ORIGINAL_COMPLETE_DECLARATIONS_PASS22
#define FABLE_RETAIL_CARRAY_ORIGINAL_INDEX_BASE_PASS22
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STRIPIFIER_VERTEX_MAPS_PASS30
#define FABLE_RETAIL_STLP_LONG_OVERFLOW_PASS30
#define FABLE_RETAIL_STLP_VECTOR_LONG_PUSHBACK_OWNER_PASS26
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_RANDOM_ACCESS_COPY_BACKWARD
#define FABLE_RETAIL_QUICK_MENU_PAIR_ALLOCATE_PASS27
#include "fable_compiler.h"
#include "fable_assert.h"
#include "engine/CTriangleStripifier2.h"
void CTriangleStripifier2::BuildVertexAndPolyMaps(
    const CArray<unsigned short>& poly_buffer,
    CArray<CArray<long> >& vertex_to_poly_map,
    CArray<CArray<unsigned short> >& vertex_link_map)
{
    vertex_to_poly_map.clear();
    for (long pl = 0; pl < poly_buffer.size() / 3; ++pl) {
        FABLE_RETAIL_ASSERT(poly_buffer[pl * 3] != poly_buffer[pl * 3 + 1]);
        FABLE_RETAIL_ASSERT(poly_buffer[pl * 3] != poly_buffer[pl * 3 + 2]);
        FABLE_RETAIL_ASSERT(poly_buffer[pl * 3 + 1] != poly_buffer[pl * 3 + 2]);
        for (long i = 0; i < 3; ++i) {
            long index = poly_buffer[pl * 3 + i];
            if (vertex_to_poly_map.size() <= index)
                vertex_to_poly_map.resize(index + 1);
            vertex_to_poly_map[index].push_back(pl);
        }
    }
    long vertex_count = vertex_to_poly_map.size();
    vertex_link_map.clear();
    vertex_link_map.resize(vertex_count);
    for (long vt = 0; vt < vertex_count; ++vt) {
        for (long pi = 0; pi < vertex_to_poly_map[vt].size(); ++pi) {
            long poly = vertex_to_poly_map[vt][pi];
            for (long i = 0; i < 3; ++i) {
                long index = poly_buffer[poly * 3 + i];
                if (index == vt)
                    continue;
                bool already_added = false;
                for (long n = 0; n < vertex_link_map[vt].size(); ++n) {
                    if (vertex_link_map[vt][n] == index) {
                        already_added = true;
                        break;
                    }
                }
                if (!already_added)
                    vertex_link_map[vt].push_back((unsigned short)index);
            }
        }
    }
}