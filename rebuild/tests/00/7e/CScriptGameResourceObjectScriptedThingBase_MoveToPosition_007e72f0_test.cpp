#include <cstdio>
static int g_hit=0;
struct Inner { virtual void d0(){}virtual void d1(){}virtual void d2(){}virtual void d3(){} virtual void V(int pos,int a1,int a2,int a3,int a4){ g_hit=1; } };
class CScriptGameResourceObjectScriptedThingBase { public: char pad[8]; Inner* f8; void MoveToPosition(int pos,int a1,int a2,int a3,int a4); };
void CScriptGameResourceObjectScriptedThingBase::MoveToPosition(int pos,int a1,int a2,int a3,int a4){ Inner* p = this->f8; if(p) p->V(pos,a1,a2,a3,a4); }
int main(){ Inner inr; CScriptGameResourceObjectScriptedThingBase o; o.f8=&inr; o.MoveToPosition(1,1,1,1,1); if(!g_hit){ std::printf("007e72f0_TEST FAIL\n"); return 1;}
 CScriptGameResourceObjectScriptedThingBase z; z.f8=0; z.MoveToPosition(1,1,1,1,1);
 std::printf("007e72f0_TEST PASS\n"); return 0;}