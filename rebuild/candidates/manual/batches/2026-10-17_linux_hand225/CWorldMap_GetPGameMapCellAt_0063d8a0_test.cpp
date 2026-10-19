#include <cstdio>
struct V { float X,Y,Z; };
struct Grid { unsigned char pad[4]; unsigned char cells[200]; unsigned char pad2[20]; long width,maxY; };
struct M { Grid* g; void* F(V const& p) const { long x=(long)p.X,y=(long)p.Y; return g->cells+((g->maxY-y-1)*g->width+x)*7; } };
int main(){ Grid g={}; g.width=4; g.maxY=10; M m={&g}; V p={2.8f,3.2f,0}; unsigned char* q=(unsigned char*)m.F(p); if(q!=g.cells+((10-3-1)*4+2)*7){std::printf("0063d8a0_TEST FAIL\n");return 1;} std::printf("0063d8a0_TEST PASS\n");return 0; }
