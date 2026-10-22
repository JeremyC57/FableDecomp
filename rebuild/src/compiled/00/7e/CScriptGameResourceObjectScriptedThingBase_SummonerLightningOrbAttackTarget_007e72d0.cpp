struct Inner { virtual void d0();virtual void d1(); virtual void V(int scriptThing); };
struct CScriptGameResourceObjectScriptedThingBase { char pad[8]; Inner* f8; void SummonerLightningOrbAttackTarget(int scriptThing); };
void CScriptGameResourceObjectScriptedThingBase::SummonerLightningOrbAttackTarget(int scriptThing){ Inner* p = this->f8; if(p) p->V(scriptThing); }