#include <cstdio>
static int g_hit=0;
struct Inner { virtual void d0(){}virtual void d1(){}virtual void d2(){}virtual void d3(){}virtual void d4(){}virtual void d5(){}virtual void d6(){} virtual void V(int scriptThing,int a1,int a2){ g_hit=1; } };
struct CScriptGameResourceObjectScriptedThingBase { char pad[8]; Inner* f8; void FollowThing(int scriptThing,int a1,int a2); };
void CScriptGameResourceObjectScriptedThingBase::FollowThing(int scriptThing,int a1,int a2){ Inner* p = this->f8; if(p) p->V(scriptThing,a1,a2); }
int main(){ Inner inr; CScriptGameResourceObjectScriptedThingBase o; o.f8=&inr; o.FollowThing(1,1,1); if(!g_hit){ std::printf("007e7320_TEST FAIL\n"); return 1;}
 CScriptGameResourceObjectScriptedThingBase z; z.f8=0; z.FollowThing(1,1,1);
 std::printf("007e7320_TEST PASS\n"); return 0;}