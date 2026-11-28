#if !defined(_MSC_VER)
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_PORTABLE
#define __fastcall
#define __int64 long long
#define __cdecl
#define __stdcall
#endif
#include <stdio.h>
#include <stdlib.h>
// 00f3e6b0 CTriangleStripifier2::BuildVertexAndPolyMaps
// Flags: /Gr
// Leaf: ?BuildVertexAndPolyMaps@CTriangleStripifier2@@AAEXABV?$CArray@G@@AAV?$CArray@V?$CArray@J@@@@AAV?$CArray@V?$CArray@G@@@@@Z
#define FABLE_RETAIL_PASS22_TRIANGLE_ORIGINAL_COMPLETE_DECLARATIONS
#define FABLE_RETAIL_CARRAY_ORIGINAL_COMPLETE_DECLARATIONS_PASS22
#define FABLE_RETAIL_CARRAY_ORIGINAL_INDEX_BASE_PASS22
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_LONG_PUSHBACK_OWNER_PASS26
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_STLP_RANDOM_ACCESS_COPY_BACKWARD
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

CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
void CTriangleStripifier2::GenerateTriangleListAndStrip(const CArray<C3DTriangle2>&,
    CArray<C3DTriangle2>&, CArray<unsigned short>&) {}

// Test-only entry to the private method. The input and both output arrays are real.
static CArray<unsigned short> triangles;
static CArray<CArray<long> > polygons;
static CArray<CArray<unsigned short> > adjacent;
void CTriangleStripifier2::ReorderTriangleListInPlace(unsigned short*, long) {
    BuildVertexAndPolyMaps(triangles, polygons, adjacent);
}

static int cases;
static void check(bool condition) {
    if (!condition) {
        fprintf(stderr, "case %d failed\n", cases);
        exit(1);
    }
}
static void exercise(const unsigned short* input, unsigned length) {
    ++cases;
    triangles.clear();
    unsigned i, vertexCount = 0, used = length - length % 3;
    for (i = 0; i < length; ++i) {
        triangles.push_back(input[i]);
        if (i < used && vertexCount <= input[i]) vertexCount = (unsigned)input[i] + 1;
    }
    CTriangleStripifier2 strip;
    strip.ReorderTriangleListInPlace(0, 0);
    check(polygons.size() == vertexCount && adjacent.size() == vertexCount);
    for (unsigned v = 0; v < vertexCount; ++v) {
        unsigned visits = 0, neighbours = 0;
        for (i = 0; i < used; ++i) {
            if (input[i] != v) continue;
            check(visits < polygons[v].size() && polygons[v][visits++] == (long)(i / 3));
            unsigned start = i - i % 3;
            for (unsigned j = start; j < start + 3; ++j) {
                unsigned target = input[j];
                if (target == v) continue;
                bool seen = false;
                for (unsigned k = 0; k < neighbours; ++k)
                    if (adjacent[v][k] == target) seen = true;
                if (seen) continue;
                check(neighbours < adjacent[v].size() && adjacent[v][neighbours++] == target);
            }
        }
        check(visits == polygons[v].size() && neighbours == adjacent[v].size());
    }
    check(triangles.size() == length);
    for (i = 0; i < length; ++i) check(triangles[i] == input[i]);
}
int main() {
    const unsigned short boundary[] = {65535, 0, 65534, 65535, 65535};
    const unsigned short shared[] = {2,5,3,3,5,2,6,2,0,3,3,3,1,6,5};
    exercise(0, 0); exercise(shared, 1); exercise(shared, 2);
    exercise(boundary, 5); exercise(shared, 15); exercise(0, 0);
    unsigned short input[6];
    for (unsigned encoded = 0; encoded < 729; ++encoded) {
        unsigned value = encoded;
        for (unsigned i = 0; i < 6; ++i) {
            input[i] = (unsigned short)(value % 3);
            value /= 3;
        }
        exercise(input, 6);
    }
    puts("00f3e6b0_TEST PASS");
    return 0;
}