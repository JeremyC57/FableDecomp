// CTriangleStripifier2::BuildCacheOptimizedVertexList -- emit each strip's vertices in order into the output list, cutting a strip
// Grade: DIFFER(834v853) -- only the inline vector<unsigned short>::push_back growth path (_M_insert_overflow) differs: retail colours len=ebx and reloads the list (this) into ebp from its arg slot after malloc (restoring ebx after), ours keeps the list in ebx and len in ebp (+jmp/7-byte align pad before the fill label); everything else byte-identical
// short when a look-ahead vertex of the next strip would fall out of a 16-entry post-transform cache.
// Retail: Fable.exe @ 0x00F3BA70, 853 bytes, __thiscall, ret 18h (this unused, triangles unused).
// Callees: std::vector<bool>::vector(n,const bool&,const allocator&) 0x00451583, std::vector<unsigned long>::vector(n,..) 0x00512630
//   (ICF, the CArray<long>(n,-1) local), memmove 0x00BFEAE6, malloc 0x00BFEA0E, free 0x00BFEA14 (inline push_back growth / dtors).
// Shape: loop bounds spelled as full expressions (strips[s+1].size(), vertex_to_vertex_map[w].size()) -- with a reference local
//   VC7.1 hoists the size; retail re-reads it each pass. w read before ++ahead; distance = ahead-counter-1 computed per j.
// Leaf: ?BuildCacheOptimizedVertexList@CTriangleStripifier2@@AAEXABV?$CArray@G@@ABV?$CArray@V?$CArray@J@@@@ABV?$CArray@V?$CArray@G@@@@02AAV2@@Z
// Header overlay: FABLE_RETAIL_CTRIANGLESTRIPIFIER2_BUILDCACHE (declares the private member in engine/CTriangleStripifier2.h).
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_FILL_NOINLINE
#define FABLE_RETAIL_CTRIANGLESTRIPIFIER2_BUILDCACHE
#include "engine/CTriangleStripifier2.h"
void CTriangleStripifier2::BuildCacheOptimizedVertexList(const CArray<unsigned short>& triangles,const CArray<CArray<long> >& vertex_to_poly_map,const CArray<CArray<unsigned short> >& vertex_to_vertex_map,const CArray<unsigned short>& vertex_strip,const CArray<CArray<unsigned short> >& strips,CArray<unsigned short>& out)
{
 long num_verts=vertex_to_poly_map.size();
 out.clear();
 CArray<bool> emitted(num_verts,false);
 CArray<long> emitted_at(num_verts,-1);
 long counter=0;
 for(long s=0;s<strips.size();++s){
  for(unsigned long i=0;i<strips[s].size();++i){
   unsigned long vertex=strips[s][i];
   long ahead=counter+1;
   if(s>0&&s<strips.size()-1){
    for(unsigned long j=0;j<strips[s+1].size();++j){
     unsigned long w=strips[s+1][j];
     bool needed=false;
     ++ahead;
     long distance=ahead-counter-1;
     for(unsigned long k=0;k<vertex_to_vertex_map[w].size();++k){
      unsigned short f=vertex_to_vertex_map[w][k];
      if(vertex_strip[f]==s){
       long at=emitted_at[f];
       if(at>=0){
        if(ahead-at>=16)goto next_strip;
       }else{
        if(f!=vertex)continue;
        if(distance>=16)goto next_strip;
       }
       needed=true;
      }
     }
     if(!needed)break;
    }
   }
   emitted[vertex]=true;
   out.push_back(vertex);
   emitted_at[vertex]=++counter;
  }
 next_strip:;
 }
}
