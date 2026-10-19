// CSpecialAbilitiesDivineWrathDef::CSpecialAbilitiesDivineWrathDef() -- initialise inherited definition, scalar defaults, and nine empty vector members at their padded retail offsets; retail @00458033 length 188.
// The quarantined class header claims 16-byte vectors from +0x40 and size 0x138; retail instead writes 12-byte vector triples from +0x78. This local retail model follows constructor bytes.
// Grade: RELOCATION_MATCH -- VC7.1 byte match, flags=[]; volatile scalar stores retain retail vtable-store order.
#pragma optimize("s", on)
struct CParentDefClassBase { unsigned char base[0x3c]; CParentDefClassBase(); }; // 0x00430370
struct Vec { unsigned int first,last,end; };
struct CSpecialAbilitiesDivineWrathDef : CParentDefClassBase { unsigned char pad0[0x3c]; Vec v0; unsigned char gap0[4]; Vec v1,v2,v3,v4,v5,v6; unsigned char gap1[12]; Vec v7; Vec v8; CSpecialAbilitiesDivineWrathDef(); };
CSpecialAbilitiesDivineWrathDef::CSpecialAbilitiesDivineWrathDef() {
    *(volatile unsigned int*)this = 0x0123487c; volatile unsigned int* z=(volatile unsigned int*)this;
    z[15]=0;z[16]=0;z[17]=0;z[18]=0;z[19]=0;z[20]=0;z[21]=0;z[22]=0;z[23]=0;z[24]=0;z[25]=0;z[26]=0;z[27]=0;z[28]=0;z[29]=0;
    Vec& a=v0; a.first=0;a.last=0;a.end=0; Vec& b=v1; b.first=0;b.last=0;b.end=0;
    Vec& c=v2; c.first=0;c.last=0;c.end=0; Vec& d=v3; d.first=0;d.last=0;d.end=0;
    Vec& e=v4; e.first=0;e.last=0;e.end=0; Vec& f=v5; f.first=0;f.last=0;f.end=0;
    Vec& g=v6; g.first=0;g.last=0;g.end=0; Vec& h=v7; h.first=0;h.last=0;h.end=0;
    Vec& i=v8; i.first=0;i.last=0;i.end=0;
}