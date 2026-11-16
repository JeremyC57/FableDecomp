#include <cstdio>
struct Sub;
static char* g_arg=0;
struct S { char pad[24]; void* ff; };   // CRenderTarget: ZBufferSurface +0x14, its PD3DSurface +0x18 (NF37: 0x00A10E20 is CRenderTarget::DetachZBuffer)
void __fastcall sub(Sub* p){ g_arg=(char*)p; }
void __fastcall DetachZBuffer(S* self){ if(self->ff) sub((Sub*)((char*)self+20)); }
int main(){ S o; o.ff=(void*)1; DetachZBuffer(&o);
 if(g_arg!=(char*)&o+20){ std::printf("00a10e20_TEST FAIL\n"); return 1;}
 std::printf("00a10e20_TEST PASS\n"); return 0;}