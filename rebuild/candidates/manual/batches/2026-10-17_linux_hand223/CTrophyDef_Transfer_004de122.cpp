#pragma optimize("s",on)
// CTrophyDef::Transfer(CPersistContext&) -- serialize each trophy-definition field in declaration order
// Retail: Fable.exe @ 0x004DE122, 189 bytes; fields include two adjacent byte members at +34/+35.
struct CPersistContext {
 void A(void*); // 0x00431061
 void B(void*); // 0x00431102
 void C(void*); // 0x0043314a
 void D(void*); // 0x0045228f
 void E(void*); // 0x00431143
};
struct CTrophyDef {
 unsigned char pad[0x24]; unsigned long f28,f2c,f30; unsigned char f34,f35; unsigned long f38,f3c,f40,f44,f48,f4c,f50,f54,f58,f5c,f60; virtual void Transfer(CPersistContext&);
};
void CTrophyDef::Transfer(CPersistContext& c)
{ c.A(&f28); c.A(&f2c); c.B(&f30); c.C(&f34); c.C(&f35); c.B(&f38); c.D(&f3c); c.A(&f40); c.A(&f44); c.E(&f48); c.E(&f4c); c.E(&f50); c.E(&f54); c.E(&f58); c.E(&f5c); c.E(&f60); }
