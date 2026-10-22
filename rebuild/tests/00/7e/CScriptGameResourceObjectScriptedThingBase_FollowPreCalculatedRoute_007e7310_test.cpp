#include <cstdio>
static int g_hit=0;
struct Inner { virtual void d0(){}virtual void d1(){}virtual void d2(){}virtual void d3(){}virtual void d4(){}virtual void d5(){} virtual void V(int scriptThing,int a1,int a2,int a3){ g_hit=1; } };
struct CScriptGameResourceObjectScriptedThingBase { char pad[8]; Inner* f8; void FollowPreCalculatedRoute(int scriptThing,int a1,int a2,int a3); };
void CScriptGameResourceObjectScriptedThingBase::FollowPreCalculatedRoute(int scriptThing,int a1,int a2,int a3){ Inner* p = this->f8; if(p) p->V(scriptThing,a1,a2,a3); }
int main(){ Inner inr; CScriptGameResourceObjectScriptedThingBase o; o.f8=&inr; o.FollowPreCalculatedRoute(1,1,1,1); if(!g_hit){ std::printf("007e7310_TEST FAIL\n"); return 1;}
 CScriptGameResourceObjectScriptedThingBase z; z.f8=0; z.FollowPreCalculatedRoute(1,1,1,1);
 std::printf("007e7310_TEST PASS\n"); return 0;}