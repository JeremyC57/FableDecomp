struct S { char pad[4]; int f4; char pad2[0xa0]; unsigned char b_a8; };
void __fastcall Init(S* woman){ woman->b_a8 = 0; woman->f4 = 2; }