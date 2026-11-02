// CTriangleStripifier2::GetMeshIslandSubsection(const CArray<unsigned short>&, const CArray<bool>&, const CArray<CArray<long> >&, CArray<unsigned short>&, CArray<long>&) -- traverse one enabled connected triangle island.
// Leaf: ?GetMeshIslandSubsection@CTriangleStripifier2@@AAEXABV?$CArray@G@@ABV?$CArray@_N@@ABV?$CArray@V?$CArray@J@@@@AAV2@AAV?$CArray@J@@@Z
#define FABLE_RETAIL_CARRAY_STRIPIFIER
#include "engine/CTriangleStripifier2.h"
void CTriangleStripifier2::GetMeshIslandSubsection(const CArray<unsigned short>& poly_buffer,const CArray<bool>& poly_mask,const CArray<CArray<long> >& vertex_to_poly_map,CArray<unsigned short>& subsection,CArray<long>& sub_section_to_global_poly_map)
{
 long num_polys=poly_buffer.size()/3;
 subsection.clear();
 sub_section_to_global_poly_map.resize(num_polys,0);
 long start_poly=0;
 for(;start_poly<num_polys;++start_poly)
  if(poly_mask[start_poly])break;
 CArray<long> poly_stack;
 CArray<unsigned short> vertex_stack;
 CArray<bool> polys_processed(num_polys,false);
 CArray<bool> verts_processed(vertex_to_poly_map.size(),false);
 poly_stack.push_back(start_poly);
 polys_processed[start_poly]=true;
 while(poly_stack.size()||vertex_stack.size()){
  while(poly_stack.size()){
   long poly=poly_stack.back();
   poly_stack.pop_back();
   sub_section_to_global_poly_map[subsection.size()/3]=poly;
   for(long i=0;i<3;++i){
    long vert=poly_buffer[poly*3+i];
    subsection.push_back(vert);
    if(!verts_processed[vert]){
     verts_processed[vert]=true;
     vertex_stack.push_back(vert);
    }
   }
  }
  while(vertex_stack.size()){
   unsigned short vert=vertex_stack.back();
   vertex_stack.pop_back();
   for(long pl=0;pl<vertex_to_poly_map[vert].size();++pl){
    long poly=vertex_to_poly_map[vert][pl];
    if(!polys_processed[poly]&&poly_mask[poly]){
     poly_stack.push_back(poly);
     polys_processed[poly]=true;
    }
   }
  }
 }
}
