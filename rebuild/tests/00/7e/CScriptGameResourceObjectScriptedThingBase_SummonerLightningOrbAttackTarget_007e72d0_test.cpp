#include <cstdio>
static int g_hit=0;
struct Inner { virtual void d0(){}virtual void d1(){} virtual void V(int scriptThing){ g_hit=1; } };
struct CScriptGameResourceObjectScriptedThingBase { char pad[8]; Inner* f8; void SummonerLightningOrbAttackTarget(int scriptThing); };
void CScriptGameResourceObjectScriptedThingBase::SummonerLightningOrbAttackTarget(int scriptThing){ Inner* p = this->f8; if(p) p->V(scriptThing); }
int main(){ Inner inr; CScriptGameResourceObjectScriptedThingBase o; o.f8=&inr; o.SummonerLightningOrbAttackTarget(1); if(!g_hit){ std::printf("007e72d0_TEST FAIL\n"); return 1;}
 CScriptGameResourceObjectScriptedThingBase z; z.f8=0; z.SummonerLightningOrbAttackTarget(1);
 std::printf("007e72d0_TEST PASS\n"); return 0;}