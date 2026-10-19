// CSpecialAbilitiesBulletTimeDef::CSpecialAbilitiesBulletTimeDef() -- initialise inherited definition, sound table positions, and seven empty screen-filter vectors; retail @00457537 length 163.
#pragma optimize("s", on)
struct CParentDefClassBase { unsigned char base[0x3c]; CParentDefClassBase(); }; // 0x00430370
struct Vec { unsigned long first,last,end; };
struct CSpecialAbilitiesBulletTimeDef : CParentDefClassBase { unsigned char pad[0x7c]; long sounds[7]; unsigned char gap[0x5c]; Vec filters[7]; CSpecialAbilitiesBulletTimeDef(); };
CSpecialAbilitiesBulletTimeDef::CSpecialAbilitiesBulletTimeDef() {
    *(unsigned long*)this = 0x01234264;
    sounds[0]=-1;sounds[1]=-1;sounds[2]=-1;sounds[3]=-1;sounds[4]=-1;sounds[5]=-1;sounds[6]=-1;
    Vec& a=filters[0]; a.first=0;a.last=0;a.end=0;
    Vec& b=filters[1]; b.first=0;b.last=0;b.end=0;
    Vec& c=filters[2]; c.first=0;c.last=0;c.end=0;
    Vec& d=filters[3]; d.first=0;d.last=0;d.end=0;
    Vec& e=filters[4]; e.first=0;e.last=0;e.end=0;
    Vec& f=filters[5]; f.first=0;f.last=0;f.end=0;
    Vec& g=filters[6]; g.first=0;g.last=0;g.end=0;
}
