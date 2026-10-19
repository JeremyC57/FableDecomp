#include <cstdio>
struct Def { unsigned char p[0x25]; unsigned char a,b,c,d,e,f,g; };
static unsigned mask(const Def& x){ return (x.a&1)|((x.b&1)<<1)|((x.c&1)<<2)|((x.d&1)<<3)|((x.f&1)<<4)|((x.g&1)<<5); }
int main(){ Def d={}; d.a=1;d.b=1;d.c=0;d.d=1;d.f=1;d.g=0; if(mask(d)!=0x1b){std::printf("007e3c70_TEST FAIL\n");return 1;} std::printf("007e3c70_TEST PASS\n");return 0; }
