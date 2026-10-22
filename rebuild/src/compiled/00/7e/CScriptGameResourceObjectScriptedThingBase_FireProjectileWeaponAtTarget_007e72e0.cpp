struct Inner { virtual void d0();virtual void d1();virtual void d2(); virtual void V(int scriptThing); };
struct S { char pad[8]; Inner* f8; void FireProjectileWeaponAtTarget(int scriptThing); };
void S::FireProjectileWeaponAtTarget(int scriptThing){ Inner* p = this->f8; if(p) p->V(scriptThing); }