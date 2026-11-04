#pragma optimize("s",on)
// Leaf: ?M@S@@QAEPAU1@XZ
extern "C" void* __cdecl sub(void* x);
struct S { void* f0; S* M(); };
S* S::M(){ this->f0 = sub(this->f0); return this; }