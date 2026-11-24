// Fixture opens private access only; complete selected production source follows unchanged.
#define private public
#define FABLE_RETAIL_STLP_VECTOR
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

#undef private

#include <cstdio>
static int failures=0;
#define CHECK(x) do{if(!(x)){++failures;std::printf("FAIL %d\n",__LINE__);}}while(0)
CBaseClass::CBaseClass(){} CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){} CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
void CTriangleStripifier2::GenerateTriangleListAndStrip(const CArray<C3DTriangle2>&,CArray<C3DTriangle2>&,CArray<unsigned short>&){}
static void Run(const CArray<unsigned short>&polys,const CArray<unsigned short>&section,int n){CArray<CArray<long> > map(n);for(unsigned p=0;p<polys.size()/3;++p)for(int v=0;v<3;++v)map[polys[p*3+v]].push_back(p);CArray<bool> mask(30,true);CTriangleStripifier2 strip;long actual=strip.CalcSectionPolyMask(polys,map,section,mask);CHECK(mask.size()==polys.size()/3);long expected=0;for(unsigned p=0;p<polys.size()/3;++p){int stamps[3]={-1,-1,-1};for(unsigned s=0;s<section.size();++s)for(int v=0;v<3;++v)if(section[s]==polys[p*3+v])stamps[v]=s;bool yes=stamps[0]>=0&&stamps[1]>=0&&stamps[2]>=0;int lo=stamps[0],hi=stamps[0];for(int v=1;v<3;++v){if(stamps[v]<lo)lo=stamps[v];if(stamps[v]>hi)hi=stamps[v];}yes=yes&&hi-lo<15;CHECK(mask[p]==yes);expected+=yes;}CHECK(actual==expected);}
int main(){for(int run=0;run<40;++run){CArray<unsigned short>polys,section;for(int p=0;p<12;++p){polys.push_back((run+p*3)%20);polys.push_back((run+p*5+1)%20);polys.push_back((run+p*7+2)%20);}for(int v=0;v<20;++v)if((v+run)%4) section.push_back((v*7+run)%20);Run(polys,section,20);}
 for(int end=14;end<=16;++end){CArray<unsigned short>polys,section;polys.push_back(0);polys.push_back(1);polys.push_back(end);for(int v=0;v<=end;++v)section.push_back(v);Run(polys,section,20);section.push_back(0);Run(polys,section,20);}
 {CArray<unsigned short>p,s;Run(p,s,0);}std::puts(failures?"00f3cf50_TEST FAIL":"00f3cf50_TEST PASS");return failures?1:0;}