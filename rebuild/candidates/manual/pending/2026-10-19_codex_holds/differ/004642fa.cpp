// Grade: HELD(bytes match retail) -- test exercises a hand-written stand-in, not the candidate | Codex source 00/46/CThingBaseDef_operator_004642fa.cpp, flags , pass pattern 004642fa_TEST PASS
// CThingBaseDef::operator=(const CThingBaseDef& other) -- copies the base payload, records and bit flags at +0x98.
// Retail 0x004642fa, 208 bytes. The base assignment is an out-of-line call at 0x00463950; record copy is 14-byte POD.
#pragma optimize("s", on)
struct S14 { unsigned short v[7]; };
struct Base { unsigned char pad[0x70]; Base& operator=(const Base&); };
struct CThingBaseDef : Base { S14 rec; long a[6]; unsigned char f0:1; unsigned char f1:1; unsigned char f2:1; unsigned char f3:1; unsigned char f4:1; CThingBaseDef& operator=(const CThingBaseDef&); };
CThingBaseDef& CThingBaseDef::operator=(const CThingBaseDef& o) { Base::operator=(o); rec=o.rec; a[0]=o.a[0]; a[1]=o.a[1]; a[2]=o.a[2]; a[3]=o.a[3]; a[4]=o.a[4]; a[5]=o.a[5]; f0=o.f0; f1=o.f1; f2=o.f2; f3=o.f3; f4=o.f4; return *this; }