#include "rebuild_abi.h"
struct C {
 virtual void s0(); virtual void s1(); virtual void s2(); virtual void s3(); virtual void s4(); virtual void s5(); virtual void s6(); virtual void s7(); virtual void s8(); virtual void s9(); virtual void s10(); virtual void s11(); virtual void s12(); virtual void s13(); virtual void s14(); virtual void s15(); virtual void s16(); virtual void s17(); virtual void s18();
 char Running; char pad[0xc-5]; int CurrentActionIndex;
 void Stop();
};
void C::Stop(){ this->Running=0; this->CurrentActionIndex=0; this->s18(); }