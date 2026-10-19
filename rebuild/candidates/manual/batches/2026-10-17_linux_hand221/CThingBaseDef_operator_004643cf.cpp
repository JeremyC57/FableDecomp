// CThingBaseDef::operator=(const CThingBaseDef& other) -- copies derived records and packed flags.
// Retail 0x004643cf, 201 bytes; size optimized POD memberwise assignment.
#pragma optimize("s", on)
struct S14 { unsigned short v[7]; };
struct Base { unsigned char pad[0x70]; Base& operator=(const Base&); };
struct CThingBaseDef : Base { long lead[2]; S14 rec; long a[6]; unsigned char f0:1; unsigned char f1:1; unsigned char f2:1; unsigned char f3:1; CThingBaseDef& operator=(const CThingBaseDef&); };
CThingBaseDef& CThingBaseDef::operator=(const CThingBaseDef& o) { Base::operator=(o); lead[0]=o.lead[0]; lead[1]=o.lead[1]; rec=o.rec; a[0]=o.a[0]; a[1]=o.a[1]; a[2]=o.a[2]; a[3]=o.a[3]; a[4]=o.a[4]; a[5]=o.a[5]; f0=o.f0; f1=o.f1; f2=o.f2; f3=o.f3; return *this; }
