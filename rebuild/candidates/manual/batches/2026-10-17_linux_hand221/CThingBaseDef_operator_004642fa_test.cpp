#include <cstdio>
struct S14 { unsigned short v[7]; }; struct Base { unsigned char pad[0x70]; }; struct CThingBaseDef:Base { S14 rec; long a[6]; unsigned char f0:1; unsigned char f1:1; unsigned char f2:1; unsigned char f3:1; unsigned char f4:1; };
int main(){CThingBaseDef a; CThingBaseDef b; for(int i=0;i<6;i++) b.a[i]=i+2; b.f0=1;b.f2=1;b.f4=1;a.rec=b.rec;for(int i=0;i<6;i++)a.a[i]=b.a[i];a.f0=b.f0;a.f1=b.f1;a.f2=b.f2;a.f3=b.f3;a.f4=b.f4;if(a.a[3]!=b.a[3]||a.f0!=b.f0||a.f2!=b.f2||a.f4!=b.f4){std::printf("004642fa_TEST FAIL\n");return 1;}std::printf("004642fa_TEST PASS\n");return 0;}
