#include <cstdio>
struct V { float x,y,z; };
struct Light { unsigned char pad[12]; V direction, position; unsigned char r,g,b,alpha; long f28,f2c,f30; };
struct Colour { float c0,c1,c2,c3; };
struct Primitive { unsigned char pad[12]; void* source; unsigned char pad2[56]; V direction, position; Colour colour; long f70,f74,f78; };
static void update(Primitive& p, Light& l, void* retained) {
 p.source=retained; p.direction=l.direction; p.f70=l.f28; p.colour.c0=l.b/255.0f; p.colour.c1=l.g/255.0f; p.colour.c2=l.r/255.0f; p.colour.c3=l.alpha/255.0f; p.f74=l.f2c; p.position=l.position; p.f78=l.f30;
}
int main(){ Primitive p={}; Light l={}; l.direction.x=1; l.direction.y=2; l.direction.z=3; l.position.x=4; l.position.y=5; l.position.z=6; l.r=255;l.g=128;l.b=64;l.alpha=32;l.f28=7;l.f2c=8;l.f30=9; void* q=(void*)0x1234; update(p,l,q); if(p.source!=q||p.direction.z!=3||p.position.x!=4||p.f70!=7||p.f74!=8||p.f78!=9||p.colour.c0<.25f||p.colour.c0>.26f||p.colour.c2!=1.0f){std::printf("00b45460_TEST FAIL\n");return 1;} std::printf("00b45460_TEST PASS\n");return 0;}