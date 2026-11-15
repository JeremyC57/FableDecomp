// Complete selected production method follows. Mesh topology is independently constructed with known open/closed vertices.
#include <stdio.h>
#include <string.h>
#include <vector>
#include <algorithm>
static int liveArrays;static bool bad;
template<class T>class CArray:public std::vector<T>{public:CArray(){++liveArrays;}CArray(const CArray&o):std::vector<T>(o){++liveArrays;}~CArray(){--liveArrays;}CArray&operator=(const CArray&o){std::vector<T>::operator=(o);return*this;}};
class CTriangleStripifier2{public:void BuildRings(long,const CArray<unsigned short>&,const CArray<CArray<long> >&,const CArray<CArray<unsigned short> >&,CArray<unsigned short>&,CArray<unsigned short>&,CArray<CArray<unsigned short> >&);void BuildCacheOptimizedVertexList(const CArray<unsigned short>&,const CArray<CArray<long> >&,const CArray<CArray<unsigned short> >&,const CArray<unsigned short>&,const CArray<CArray<unsigned short> >&,CArray<unsigned short>&);float CalcSectionScore(const CArray<unsigned short>&,const CArray<unsigned short>&,const CArray<CArray<long> >&);long FindBestStartVertex(const CArray<unsigned short>&,const CArray<CArray<long> >&,const CArray<CArray<unsigned short> >&);};
static CTriangleStripifier2*owner;static const CArray<unsigned short>*polys;static const CArray<CArray<long> >*incidence;static const CArray<CArray<unsigned short> >*links;static std::vector<bool>closed;static std::vector<long>called;static std::vector<int>visits;static int stage,scoreMode,seed;static long pending;
static float Bits(unsigned int x){float f;memcpy(&f,&x,4);return f;}
static float Weight(long v,int visit){switch(scoreMode){case 0:return 0.0f;case 1:return -1.0f;case 2:return(float)(v+1);case 3:return(float)(100-v);case 4:return 2.0f;case 5:return v%5==seed%5?1e30f:1.0f;case 6:return closed[v]?4.0f:0.0f;case 7:return Bits(0x7fc12345u);case 8:return v%3==0?Bits(0x7f800000u):1.0f;case 9:return Bits(0xff800000u);case 10:return visit?(float)(v+1):0.0f;case 11:return visit?3.0f:Bits(0x7fc54321u);case 12:return(float)((v+seed)%7)*0.25f;case 13:return v%2?0.0f:-1.0f;case 14:return v==0?1e30f:2.0f;case 15:return closed[v]?1e30f:0.0f;default:{unsigned int a=(unsigned int)v*1103515245u+(unsigned int)seed*12345u+(unsigned int)visit*7919u;switch((a>>(scoreMode-16))%7){case 0:return 0;case 1:return -2;case 2:return 1;case 3:return 3;case 4:return 1e30f;case 5:return Bits(0x7fc00001u);default:return(float)(a&15u)*0.125f;}}}}
static void Context(const CArray<unsigned short>&p,const CArray<CArray<long> >&m){if(owner==0||&p!=polys||&m!=incidence)bad=true;}
void CTriangleStripifier2::BuildRings(long v,const CArray<unsigned short>&p,const CArray<CArray<long> >&m,const CArray<CArray<unsigned short> >&l,CArray<unsigned short>&map,CArray<unsigned short>&pos,CArray<CArray<unsigned short> >&rings){Context(p,m);if(this!=owner||&l!=links||stage||v<0||(unsigned long)v>=m.size()||m[v].empty()||!map.empty()||!pos.empty()||!rings.empty())bad=true;pending=v;called.push_back(v);stage=1;map.push_back((unsigned short)v);pos.push_back((unsigned short)(v+11));rings.resize(2);rings[0].push_back((unsigned short)(v+7));rings[1].push_back((unsigned short)(v+13));}
void CTriangleStripifier2::BuildCacheOptimizedVertexList(const CArray<unsigned short>&p,const CArray<CArray<long> >&m,const CArray<CArray<unsigned short> >&l,const CArray<unsigned short>&map,const CArray<CArray<unsigned short> >&rings,CArray<unsigned short>&out){Context(p,m);if(this!=owner||&l!=links||stage!=1||map.size()!=1||map[0]!=pending||rings.size()!=2||rings[0].size()!=1||rings[1].size()!=1||rings[0][0]!=pending+7||rings[1][0]!=pending+13||!out.empty())bad=true;out.push_back((unsigned short)pending);stage=2;}
float CTriangleStripifier2::CalcSectionScore(const CArray<unsigned short>&out,const CArray<unsigned short>&p,const CArray<CArray<long> >&m){Context(p,m);if(this!=owner||stage!=2||out.size()!=1||out[0]!=pending)bad=true;stage=0;return Weight(pending,visits[pending]++);}

long CTriangleStripifier2::FindBestStartVertex(const CArray<unsigned short>& poly_buffer,
    const CArray<CArray<long> >& vertex_to_poly_map, const CArray<CArray<unsigned short> >& vertex_link_map)
{
    long vertex_count = vertex_to_poly_map.size();
    float best_score = 0.0f;
    long best_start_vert = -1;
    for (long start_vert = 0; start_vert < vertex_count; ++start_vert) {
        if (vertex_to_poly_map[start_vert].size() == 0)
            continue;
        long surround_vert = vertex_link_map[start_vert][0];
        long start_surround_vert = surround_vert;
        for (long verts_searched = 0; verts_searched < vertex_link_map[start_vert].size(); ++verts_searched) {
            for (long pl = 0; pl < vertex_to_poly_map[surround_vert].size(); ++pl) {
                long poly = vertex_to_poly_map[surround_vert][pl];
                long id = 0;
                for (; id < 3; ++id)
                    if (poly_buffer[poly*3+id] == surround_vert)
                        break;
                long id1 = (id+1)%3;
                long id2 = (id1+1)%3;
                if (poly_buffer[poly*3+id1] == start_vert) {
                    surround_vert = poly_buffer[poly*3+id2];
                    if (surround_vert == start_surround_vert)
                        goto skip_to_next_start_vert;
                    break;
                }
            }
        }
        {
            CArray<unsigned short> vertex_to_ring_map;
            CArray<unsigned short> vertex_to_ring_pos_map;
            CArray<CArray<unsigned short> > rings;
            BuildRings(start_vert, poly_buffer, vertex_to_poly_map, vertex_link_map, vertex_to_ring_map, vertex_to_ring_pos_map, rings);
            CArray<unsigned short> vertex_list;
            BuildCacheOptimizedVertexList(poly_buffer, vertex_to_poly_map, vertex_link_map, vertex_to_ring_map, rings, vertex_list);
            float score = CalcSectionScore(vertex_list, poly_buffer, vertex_to_poly_map);
            if (score > best_score) {
                best_score = score;
                best_start_vert = start_vert;
            }
            if (best_score == 1e30f)
                break;
        }
    skip_to_next_start_vert:
        continue;
    }
    if (best_score == 0.0f) {
        for (long start_vert = 0; start_vert < vertex_count; ++start_vert) {
            if (vertex_to_poly_map[start_vert].size() == 0)
                continue;
                CArray<unsigned short> vertex_to_ring_map;
                CArray<unsigned short> vertex_to_ring_pos_map;
                CArray<CArray<unsigned short> > rings;
                CArray<unsigned short> vertex_list;
                BuildRings(start_vert, poly_buffer, vertex_to_poly_map, vertex_link_map, vertex_to_ring_map, vertex_to_ring_pos_map, rings);
                BuildCacheOptimizedVertexList(poly_buffer, vertex_to_poly_map, vertex_link_map, vertex_to_ring_map, rings, vertex_list);
                float score = CalcSectionScore(vertex_list, poly_buffer, vertex_to_poly_map);
                if (score > best_score) {
                    best_score = score;
                    best_start_vert = start_vert;
                }
                if (best_score == 1e30f)
                    break;
        }
    }
        if (best_score == 0.0f) {
            for (long start_vert = 0; start_vert < vertex_count; ++start_vert) {
                if (vertex_to_poly_map[start_vert].size() == 0)
                    continue;
                return start_vert;
            }
        }
    return best_start_vert;
}


struct Mesh{std::vector<unsigned short>tri;std::vector<bool>closed;void Vert(bool c){closed.push_back(c);}void Tri(int a,int b,int c){tri.push_back((unsigned short)a);tri.push_back((unsigned short)b);tri.push_back((unsigned short)c);}void Fan(int n,bool complete){int z=(int)closed.size();Vert(complete);for(int i=0;i<n;++i)Vert(false);for(int i=0;i<n-(complete?0:1);++i)Tri(z,z+1+i,z+1+(i+1)%n);}void Tetra(){int z=(int)closed.size();for(int i=0;i<4;++i)Vert(true);Tri(z,z+2,z+1);Tri(z,z+1,z+3);Tri(z,z+3,z+2);Tri(z+1,z+2,z+3);}void Grid(int n){int z=(int)closed.size();for(int y=0;y<n;++y)for(int x=0;x<n;++x)Vert(x>0&&y>0&&x<n-1&&y<n-1);for(int y=0;y<n-1;++y)for(int x=0;x<n-1;++x){int a=z+y*n+x;Tri(a,a+1,a+n+1);Tri(a,a+n+1,a+n);}}};
static Mesh Make(int kind,int n){Mesh m;switch(kind){case 0:for(int i=0;i<n;++i)m.Vert(false);break;case 1:m.Fan(n,false);break;case 2:m.Fan(n,true);break;case 3:m.Tetra();break;case 4:m.Vert(false);m.Fan(n,false);m.Fan(n,true);m.Tetra();break;case 5:m.Grid(n);break;case 6:m.Tetra();m.Vert(false);m.Tetra();break;}return m;}
static long Expected(const CArray<CArray<long> >&m,std::vector<long>&order){float best=0;long vertex=-1;std::vector<int>v(m.size(),0);for(int pass=0;pass<2;++pass){for(unsigned int i=0;i<m.size();++i){if(m[i].empty()||(pass==0&&closed[i]))continue;order.push_back(i);float w=Weight(i,v[i]++);if(w>best){best=w;vertex=i;}if(best==1e30f)break;}if(best!=0)break;}if(best==0){for(unsigned int i=0;i<m.size();++i)if(!m[i].empty())return i;}return vertex;}
static int Fail(int c,const char*s){printf("00f3e120_TEST FAIL %d %s\n",c,s);return 1;}
int main(){const int sizes[]={3,4,5,7,9};int cases=0,totalScores=0,emptyCases=0,fallbackCases=0;
for(int kind=0;kind<7;++kind)for(int sz=0;sz<5;++sz)for(int reverse=0;reverse<2;++reverse)for(int perm=0;perm<4;++perm)for(int rotate=0;rotate<3;++rotate)for(scoreMode=0;scoreMode<20;++scoreMode,++cases){
 if(liveArrays)return Fail(cases,"prior array lifetime");
 bad=false;stage=0;seed=sz*71+perm*13+rotate;called.clear();
 {
  Mesh mesh=Make(kind,sizes[sz]);closed=mesh.closed;CArray<unsigned short>p;p.assign(mesh.tri.begin(),mesh.tri.end());CArray<CArray<long> >m;CArray<CArray<unsigned short> >l;m.resize(closed.size());l.resize(closed.size());
  if(reverse){for(unsigned int q=0;q<p.size();q+=3)std::swap(p[q+1],p[q+2]);}
  if(p.size()){unsigned int shift=((unsigned int)perm*3)%p.size();std::rotate(p.begin(),p.begin()+shift,p.end());if(perm==3){for(unsigned int a=0,b=(unsigned int)p.size()-3;a<b;a+=3,b-=3)for(unsigned int j=0;j<3;++j)std::swap(p[a+j],p[b+j]);}}
  for(unsigned int q=0;q<p.size()/3;++q)for(unsigned int j=0;j<3;++j){unsigned short v=p[q*3+j];m[v].push_back(q);for(unsigned int k=0;k<3;++k)if(j!=k){unsigned short b=p[q*3+k];if(std::find(l[v].begin(),l[v].end(),b)==l[v].end())l[v].push_back(b);}}
  for(unsigned int v=0;v<m.size();++v){if(rotate==1)std::reverse(m[v].begin(),m[v].end());if(rotate&&l[v].size())std::rotate(l[v].begin(),l[v].begin()+(v+rotate)%l[v].size(),l[v].end());}
  CTriangleStripifier2 strip;owner=&strip;polys=&p;incidence=&m;links=&l;visits.assign(m.size(),0);std::vector<long>expectedOrder;long want=Expected(m,expectedOrder);int baseline=liveArrays;long got=strip.FindBestStartVertex(p,m,l);
  if(bad||stage||got!=want||called!=expectedOrder||liveArrays!=baseline)return Fail(cases,"whole selection/helper protocol/lifetime");
  totalScores+=(int)called.size();if(p.empty())++emptyCases;if(kind==3||kind==6)++fallbackCases;
 }
 if(liveArrays)return Fail(cases,"input cleanup");
}
if(cases!=16800||!totalScores||!emptyCases||!fallbackCases)return Fail(cases,"coverage");
 printf("cases=%d scores=%d empty=%d closed_mesh=%d\n",cases,totalScores,emptyCases,fallbackCases);puts("00f3e120_TEST PASS");return 0;}
