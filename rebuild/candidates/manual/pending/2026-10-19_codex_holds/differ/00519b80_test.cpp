#include <cstdio>
struct R { int x[8]; unsigned char a,b,c; };
int main(){ R a={{1,2,3,4,5,6,7,8},1,0,1}, b={{0}}; for(int i=0;i<8;++i)b.x[i]=a.x[i]; b.a=a.a;b.b=a.b;b.c=a.c; if(b.x[6]!=7||b.a!=1||b.c!=1){std::printf("00519b80_TEST FAIL\n");return 1;} std::printf("00519b80_TEST PASS\n");return 0; }