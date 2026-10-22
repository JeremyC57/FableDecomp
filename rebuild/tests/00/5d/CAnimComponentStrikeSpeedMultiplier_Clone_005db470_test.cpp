#include <cstdio>
struct CAnimComponentStrikeSpeedMultiplier; typedef CAnimComponentStrikeSpeedMultiplier* (__fastcall* VFn)(CAnimComponentStrikeSpeedMultiplier*);
struct CAnimComponentStrikeSpeedMultiplier { VFn* vtbl; char pad[4]; void* f8; };
static CAnimComponentStrikeSpeedMultiplier g_new;
static CAnimComponentStrikeSpeedMultiplier* __fastcall vm(CAnimComponentStrikeSpeedMultiplier* p){ (void)p; return &g_new; }
static CAnimComponentStrikeSpeedMultiplier* f(CAnimComponentStrikeSpeedMultiplier* component){ CAnimComponentStrikeSpeedMultiplier* p=component->vtbl[0x10/4](component); p->f8=component->f8; return p; }
int main(){ VFn t[8]; for(int i=0;i<8;i++)t[i]=0; t[0x10/4]=vm;
 int m; CAnimComponentStrikeSpeedMultiplier o; o.vtbl=t; o.f8=&m; CAnimComponentStrikeSpeedMultiplier* r=f(&o);
 if(r!=&g_new||r->f8!=&m){ std::printf("005db470_TEST FAIL\n"); return 1;}
 std::printf("005db470_TEST PASS\n"); return 0;}