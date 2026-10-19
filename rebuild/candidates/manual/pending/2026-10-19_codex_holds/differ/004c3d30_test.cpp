#include <cstdio>
struct Ref { int pad0; int refs; }; struct AnimIn { int a,b; Ref* first; int c; Ref* second; }; struct Fourteen {int x[14];};
struct Obj { Ref* first; Ref* second; int vals[20]; };
int main(){Ref a={0,0},b={0,0}; AnimIn in={0,0,&a,0,&b}; if(!in.first||!in.second){std::printf("004c3d30_TEST FAIL\n");return 1;} ++a.refs;++b.refs; std::printf("004c3d30_TEST PASS\n"); return 0;}