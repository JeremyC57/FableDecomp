// CTriangleStripifier2::BuildCacheOptimizedVertexList builds a vertex list ring by ring until a linked vertex would fall out of the sixteen-entry cache.
// Leaf: ?BuildCacheOptimizedVertexList@CTriangleStripifier2@@AAEXABV?$CArray@G@@ABV?$CArray@V?$CArray@J@@@@ABV?$CArray@V?$CArray@G@@@@02AAV2@@Z
// Flags: (none)
// Retail: 0x00F3BA70 through 0x00F3BDC4 inclusive. Ego_r 0x00BD7470 and FableWin 0x0331EC20 give the private member signature.
// Ego_d gives every local and scope. FableWin 0x0331EE36 and 0x0331EE59 set found_linked_vert in separate arms.
#define FABLE_RETAIL_STLP_BOOL_FILL_OWNER_PASS26
#define FABLE_RETAIL_STLP_BOOL_VECTOR_SIZE_OWNERS_PASS26
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CARRAY_ORIGINAL_INDEX_BASE_PASS22
#define FABLE_RETAIL_CARRAY_ORIGINAL_COMPLETE_DECLARATIONS_PASS22
#define FABLE_RETAIL_PASS22_TRIANGLE_ORIGINAL_COMPLETE_DECLARATIONS
#define private public
#include "engine/CTriangleStripifier2.h"
#undef private

void CTriangleStripifier2::BuildCacheOptimizedVertexList(
    const CArray<unsigned short>& poly_buffer, const CArray<CArray<long> >& vertex_to_poly_map,
    const CArray<CArray<unsigned short> >& vertex_link_map, const CArray<unsigned short>& vertex_to_ring_map,
    const CArray<CArray<unsigned short> >& rings, CArray<unsigned short>& dest_vert_list)
{
    long vertex_count = vertex_to_poly_map.size();
    dest_vert_list.clear();
    CArray<bool> verts_processed(vertex_count, false);
    CArray<long> vert_cache_stamp(vertex_count, -1);
    long current_cache_stamp = 0;

    for (long ring = 0; ring < rings.size(); ++ring) {
        for (long vt = 0; vt < rings[ring].size(); ++vt) {
            long vert = rings[ring][vt];
            long look_ahead_stamp = current_cache_stamp + 1;
            if (ring > 0 && ring < rings.size() - 1) {
                for (long lavt = 0; lavt < rings[ring + 1].size(); ++lavt) {
                    long lavert = rings[ring + 1][lavt];
                    bool found_linked_vert = false;
                    ++look_ahead_stamp;
                    for (long lv = 0; lv < vertex_link_map[lavert].size(); ++lv) {
                        long link_vert = vertex_link_map[lavert][lv];
                        if (vertex_to_ring_map[link_vert] == ring) {
                            if (vert_cache_stamp[link_vert] >= 0) {
                                if (look_ahead_stamp - vert_cache_stamp[link_vert] >= 16)
                                    goto next_ring;
                                found_linked_vert = true;
                            } else if (link_vert == vert) {
                                if (look_ahead_stamp - (current_cache_stamp + 1) >= 16)
                                    goto next_ring;
                                found_linked_vert = true;
                            }
                        }
                    }
                    if (!found_linked_vert)
                        break;
                }
            }
            verts_processed[vert] = true;
            dest_vert_list.push_back(vert);
            ++current_cache_stamp;
            vert_cache_stamp[vert] = current_cache_stamp;
        }
    next_ring:;
    }
}

#include <stdio.h>

CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
void CTriangleStripifier2::GenerateTriangleListAndStrip(const CArray<C3DTriangle2>&, CArray<C3DTriangle2>&, CArray<unsigned short>&) {}

static bool run_case(unsigned int tail_length, bool link_to_first, unsigned int expected_middle)
{
    CArray<unsigned short> polys, owner(4, 0), dest;
    CArray<CArray<long> > membership(4);
    CArray<CArray<unsigned short> > links(4), rings(3);
    owner[1] = owner[2] = 1;
    owner[3] = 2;
    rings[0].push_back(0);
    rings[1].push_back(1);
    rings[1].push_back(2);
    for (unsigned int i = 0; i < tail_length; ++i)
        rings[2].push_back(3);
    links[3].push_back(0);  // linked vertex belongs to another ring
    links[3].push_back(link_to_first ? 1 : 2);
    dest.push_back(99);
    CTriangleStripifier2 stripifier;
    stripifier.BuildCacheOptimizedVertexList(polys, membership, links, owner, rings, dest);
    if (dest.size() != 1 + expected_middle + tail_length || dest[0] != 0)
        return false;
    for (unsigned int k = 1; k <= expected_middle; ++k)
        if (dest[k] != k)
            return false;
    for (unsigned int t = 0; t < tail_length; ++t)
        if (dest[1 + expected_middle + t] != 3)
            return false;
    return true;
}

int main()
{
    CArray<unsigned short> polys, owner, dest;
    CArray<CArray<long> > membership;
    CArray<CArray<unsigned short> > links, rings;
    CTriangleStripifier2 stripifier;
    dest.push_back(77);
    stripifier.BuildCacheOptimizedVertexList(polys, membership, links, owner, rings, dest);
    if (dest.size() != 0 || !run_case(0,true,2) || !run_case(14,true,2) ||
        !run_case(15,true,1) || !run_case(16,true,0) || !run_case(16,false,1))
        return 1;
    puts("00f3ba70_TEST PASS");
    return 0;
}