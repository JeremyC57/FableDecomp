// CTriangleStripifier2::FillInPolygons(const CArray<unsigned short>&, const CArray<CArray<long> >&, const CArray<unsigned short>&, const CArray<bool>&, CArray<long>&, CArray<unsigned short>&, bool) -- choose and rotate triangles in cache order.
// Leaf: ?FillInPolygons@CTriangleStripifier2@@AAEXABV?$CArray@G@@ABV?$CArray@V?$CArray@J@@@@0ABV?$CArray@_N@@AAV?$CArray@J@@AAV2@_N@Z
// Grade: DIFFER(1427v1463) -- retail frame is 8-aligned (push ebp/mov ebp,esp/and esp,-8 under /GS): an 8-aligned local this source lacks; register allocation in the rotation loops follows
// Context: grades against the Codex row context $S/codex11_dl/x/rows/00f3c990/selected_context/hdr (Codex per-feature macros; NOT rebased on rebuild/include); source = Codex selected; 00f3c990 probes in probe/00f3c990.log
// Flags: /GS
#define FABLE_RETAIL_CARRAY_STRIPIFIER
#include "engine/CTriangleStripifier2.h"
void CTriangleStripifier2::FillInPolygons(const CArray<unsigned short>& poly_buffer,const CArray<CArray<long> >& vertex_to_poly_map,const CArray<unsigned short>& vertex_list,const CArray<bool>& poly_mask,CArray<long>& dest_poly_ids,CArray<unsigned short>& dest_polys,bool ignore_cache_checking)
{
 long poly_count=poly_buffer.size();
 long vertex_count=vertex_to_poly_map.size();
 dest_poly_ids.clear();
 dest_polys.clear();
 CArray<bool> polys_added(poly_count,false);
 CArray<bool> verts_cached(vertex_count,false);
 CArray<long> vert_to_list_pos(vertex_count,(-2147483647L-1));
 for(long vt=0;vt<vertex_list.size();++vt)
  vert_to_list_pos[vertex_list[vt]]=vt;
 for(long vt=0;vt<vertex_list.size();++vt){
  long vert=vertex_list[vt];
  for(;;){
   long best_poly=-1;
   long best_poly_score=(-2147483647L-1);
   for(long pl=0;pl<vertex_to_poly_map[vert].size();++pl){
    long poly=vertex_to_poly_map[vert][pl];
    if(poly_mask[poly]&&!polys_added[poly]){
     long score=0;
     for(long i=0;i<3;++i){
      long vert2=poly_buffer[poly*3+i];
      if(!verts_cached[vert2]&&vert_to_list_pos[vert2]>vt){score=-1;break;}
     }
     if(score<0)continue;
     for(long i=0;i<3;++i){
      long this_vert=poly_buffer[poly*3+i];
      if(verts_cached[this_vert]||this_vert==vert)++score;
     }
     unsigned short new_vert_ids[3];
     new_vert_ids[0]=poly_buffer[poly*3];
     new_vert_ids[1]=poly_buffer[poly*3+1];
     new_vert_ids[2]=poly_buffer[poly*3+2];

   long times_rotated=0;
   while(times_rotated<2){
    bool rotated=false;
    for(long t1=1;t1>=0;--t1){
     for(long t2=t1;t2>=0;--t2){
      long cache_order1=vert_to_list_pos[new_vert_ids[t2]];
      long cache_order2=vert_to_list_pos[new_vert_ids[t1+1]];
      if(!verts_cached[new_vert_ids[t2]]&&!verts_cached[new_vert_ids[t1+1]]&&cache_order1>cache_order2){
       unsigned short temp=new_vert_ids[0];
       new_vert_ids[0]=new_vert_ids[2];
       new_vert_ids[2]=new_vert_ids[1];
       new_vert_ids[1]=temp;
       ++times_rotated;
       rotated=true;
       break;
      }
     }
     if(rotated)break;
    }
    if(!rotated)break;
   }

     if(times_rotated<2)score+=100;
     if(score>best_poly_score){best_poly_score=score;best_poly=poly;}
    }
   }
   if(best_poly>=0){
   unsigned short new_vert_ids[3];
   new_vert_ids[0]=poly_buffer[best_poly*3];
   new_vert_ids[1]=poly_buffer[best_poly*3+1];
   new_vert_ids[2]=poly_buffer[best_poly*3+2];

   long times_rotated=0;
   while(times_rotated<2){
    bool rotated=false;
    for(long t1=1;t1>=0;--t1){
     for(long t2=t1;t2>=0;--t2){
      long cache_order1=vert_to_list_pos[new_vert_ids[t2]];
      long cache_order2=vert_to_list_pos[new_vert_ids[t1+1]];
      if(!verts_cached[new_vert_ids[t2]]&&!verts_cached[new_vert_ids[t1+1]]&&cache_order1>cache_order2){
       unsigned short temp=new_vert_ids[0];
       new_vert_ids[0]=new_vert_ids[2];
       new_vert_ids[2]=new_vert_ids[1];
       new_vert_ids[1]=temp;
       ++times_rotated;
       rotated=true;
       break;
      }
     }
     if(rotated)break;
    }
    if(!rotated)break;
   }

   dest_poly_ids.push_back(best_poly);
   for(long i=0;i<3;++i){
    dest_polys.push_back(new_vert_ids[i]);
    if(!verts_cached[new_vert_ids[i]])verts_cached[new_vert_ids[i]]=true;
   }
   polys_added[best_poly]=true;
   }else break;
  }
 }
}
