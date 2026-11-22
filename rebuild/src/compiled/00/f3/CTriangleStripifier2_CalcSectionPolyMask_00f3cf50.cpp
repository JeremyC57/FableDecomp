#define FABLE_RETAIL_STLP_BOOL_FILL_OWNER_PASS26
#define FABLE_RETAIL_STLP_BOOL_VECTOR_SIZE_OWNERS_PASS26
// Original complete shared owner and CArray types; all Ego_d locals restored.
// Leaf: ?CalcSectionPolyMask@CTriangleStripifier2@@AAEJABV?$CArray@G@@ABV?$CArray@V?$CArray@J@@@@0AAV?$CArray@_N@@@Z
// Flags: (none)
#define FABLE_RETAIL_PASS22_TRIANGLE_ORIGINAL_COMPLETE_DECLARATIONS
#define FABLE_RETAIL_CARRAY_ORIGINAL_INDEX_BASE_PASS22
#define FABLE_RETAIL_CARRAY_ORIGINAL_COMPLETE_DECLARATIONS_PASS22
#include "engine/CTriangleStripifier2.h"
long CTriangleStripifier2::CalcSectionPolyMask(const CArray<unsigned short>& poly_buffer,const CArray<CArray<long> >& vertex_to_poly_map,const CArray<unsigned short>& vertex_list,CArray<bool>& mask)
{
 CArray<bool> polys_processed(poly_buffer.size()/3,false);
 CArray<bool> verts_in_section(vertex_to_poly_map.size(),false);
 CArray<long> vert_cache_stamp(vertex_to_poly_map.size(),-0x40000000);
 long current_cache_stamp=0;
 for(long vt=0;vt<vertex_list.size();++vt) {
  long vert=vertex_list[vt];
  verts_in_section[vert]=true;
  ++current_cache_stamp;
  vert_cache_stamp[vert]=current_cache_stamp;
 }
 mask.clear();mask.resize(poly_buffer.size()/3,false);
 long poly_count=0;
 for(long vt=0;vt<vertex_list.size();++vt) {
  long vert=vertex_list[vt];
  for(long pl=0;pl<vertex_to_poly_map[vert].size();++pl) {
   long poly=vertex_to_poly_map[vert][pl];
   if(!polys_processed[poly]) {
    if(verts_in_section[poly_buffer[poly*3]]&&verts_in_section[poly_buffer[poly*3+1]]&&verts_in_section[poly_buffer[poly*3+2]]) {
     long min_stamp=((vert_cache_stamp[poly_buffer[poly*3]]<vert_cache_stamp[poly_buffer[poly*3+1]]?vert_cache_stamp[poly_buffer[poly*3]]:vert_cache_stamp[poly_buffer[poly*3+1]])<vert_cache_stamp[poly_buffer[poly*3+2]]?(vert_cache_stamp[poly_buffer[poly*3]]<vert_cache_stamp[poly_buffer[poly*3+1]]?vert_cache_stamp[poly_buffer[poly*3]]:vert_cache_stamp[poly_buffer[poly*3+1]]):vert_cache_stamp[poly_buffer[poly*3+2]]);
     long max_stamp=((vert_cache_stamp[poly_buffer[poly*3]]>vert_cache_stamp[poly_buffer[poly*3+1]]?vert_cache_stamp[poly_buffer[poly*3]]:vert_cache_stamp[poly_buffer[poly*3+1]])>vert_cache_stamp[poly_buffer[poly*3+2]]?(vert_cache_stamp[poly_buffer[poly*3]]>vert_cache_stamp[poly_buffer[poly*3+1]]?vert_cache_stamp[poly_buffer[poly*3]]:vert_cache_stamp[poly_buffer[poly*3+1]]):vert_cache_stamp[poly_buffer[poly*3+2]]);
     if(max_stamp-min_stamp<15) {mask[poly]=true;++poly_count;}
    }
    polys_processed[poly]=true;
   }
  }
 }
 return poly_count;
}