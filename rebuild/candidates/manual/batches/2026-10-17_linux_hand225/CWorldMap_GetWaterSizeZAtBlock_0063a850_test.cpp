#include <cstdio>
struct P { long x,y; };
struct M { float A(P const&p) const { return (float)p.x; } float B(P const&p) const { return (float)p.y; } };
struct W { bool ok; M m; float F(P const&p) const { if(!ok)return 0; return m.A(p)+m.B(p); } };
int main(){W w={true,{}}; P p={4,7}; if(w.F(p)!=11){std::printf("0063a850_TEST FAIL\n");return 1;} std::printf("0063a850_TEST PASS\n");return 0;}
