struct I_007eac00 { char pad[0x30]; unsigned long f; };
// Leaf: ?M@C_007eac00@@QAEKXZ
struct C_007eac00 { char pad[0xc]; I_007eac00* p; unsigned long M(); };
unsigned long C_007eac00::M(){ return this->p->f; }