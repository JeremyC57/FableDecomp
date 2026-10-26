#pragma optimize("s",on)
struct Field { unsigned char b[0xc]; };
class CPersistContext { public:
    void PersistA_00464cbf(void* p);
    void PersistB_00466a47(void* p);
    void PersistC(void* p);
    void PersistD(void* p);
};
class CSpecialAbilitiesDrainLifeDef { public:
    unsigned char _pad[0x3c];
    Field f0; // 0x3c
    Field f1; // 0x48
    Field f2; // 0x54
    Field f3; // 0x60
    Field f4; // 0x6c
    void Transfer(CPersistContext& ctx);
};

void CSpecialAbilitiesDrainLifeDef::Transfer(CPersistContext& ctx)
{
    ctx.PersistA_00464cbf(&f0);
    ctx.PersistB_00466a47(&f1);
    ctx.PersistA_00464cbf(&f2);
    ctx.PersistC(&f3);
    ctx.PersistD(&f4);
}