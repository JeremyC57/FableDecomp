#include "rebuild_abi.h"
struct Sub { void Notify(void* a); };
struct C { char p[0x1e0]; Sub s1e0; void Notify(void* a, void* array); };
void C::Notify(void* a, void* array){ this->s1e0.Notify(a); }