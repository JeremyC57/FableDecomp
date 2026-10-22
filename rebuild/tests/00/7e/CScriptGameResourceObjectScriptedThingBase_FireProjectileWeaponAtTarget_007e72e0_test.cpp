#include <cstdio>
static int g_hit=0;
struct Inner { virtual void d0(){}virtual void d1(){}virtual void d2(){} virtual void V(int scriptThing){ g_hit=1; } };
struct S { char pad[8]; Inner* f8; void FireProjectileWeaponAtTarget(int scriptThing); };
void S::FireProjectileWeaponAtTarget(int scriptThing){ Inner* p = this->f8; if(p) p->V(scriptThing); }
int main(){ Inner inr; S o; o.f8=&inr; o.FireProjectileWeaponAtTarget(1); if(!g_hit){ std::printf("007e72e0_TEST FAIL\n"); return 1;}
 S z; z.f8=0; z.FireProjectileWeaponAtTarget(1);
 std::printf("007e72e0_TEST PASS\n"); return 0;}