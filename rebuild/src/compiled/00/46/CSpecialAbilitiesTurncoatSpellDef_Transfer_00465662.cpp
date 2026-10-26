#pragma optimize("s",on)
struct Field { unsigned char b[0xc]; };
class CPersistContext { public:                       // typed transfer helpers: each starts `push 0x122d70e ; call 0x404500` (CheckCRC)
    void Transfer_00464cbf(void* value);            // 0x00464CBF per-type transfer instantiation, element type not attributed
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
    ctx.Transfer_00464cbf(&f0);
    ctx.Transfer_00464cbf(&f1);
    ctx.Transfer_00464cbf(&f2);
    ctx.Transfer_00464cbf(&f3);
    ctx.Transfer_00464cbf(&f4);
}