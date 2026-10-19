#include <cstdio>
struct V { float X,Y,Z; };
struct P { long x,y; };
struct M { float Get(P const& p) const { return (float)(p.x*10+p.y); } };
struct W { bool inside; long map; bool loaded; M m; float F(V const& p) const { if (!inside) return 0.0f; P b={(long)p.X,(long)p.Y}; if (map==0 || !loaded) return 0.0f; return m.Get(b); } };
int main(){ W w={true,2,true,{}}; V p={3.7f,4.2f,0}; if(w.F(p)!=34.0f){std::printf("0063a540_TEST FAIL\n");return 1;} w.inside=false; if(w.F(p)!=0.0f){std::printf("0063a540_TEST FAIL\n");return 1;} std::printf("0063a540_TEST PASS\n"); return 0; }
