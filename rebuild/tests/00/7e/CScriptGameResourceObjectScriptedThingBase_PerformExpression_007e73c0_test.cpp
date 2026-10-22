#include <cstdio>
static int g_hit=0;
struct Inner { virtual void d0(){}virtual void d1(){}virtual void d2(){}virtual void d3(){}virtual void d4(){}virtual void d5(){}virtual void d6(){}virtual void d7(){}virtual void d8(){}virtual void d9(){}virtual void d10(){}virtual void d11(){}virtual void d12(){}virtual void d13(){}virtual void d14(){}virtual void d15(){}virtual void d16(){} virtual void V(int scriptThing,int name){ g_hit=1; } };
struct CScriptGameResourceObjectScriptedThingBase { char pad[8]; Inner* f8; void PerformExpression(int scriptThing,int name); };
void CScriptGameResourceObjectScriptedThingBase::PerformExpression(int scriptThing,int name){ Inner* p = this->f8; if(p) p->V(scriptThing,name); }
int main(){ Inner inr; CScriptGameResourceObjectScriptedThingBase o; o.f8=&inr; o.PerformExpression(1,1); if(!g_hit){ std::printf("007e73c0_TEST FAIL\n"); return 1;}
 CScriptGameResourceObjectScriptedThingBase z; z.f8=0; z.PerformExpression(1,1);
 std::printf("007e73c0_TEST PASS\n"); return 0;}