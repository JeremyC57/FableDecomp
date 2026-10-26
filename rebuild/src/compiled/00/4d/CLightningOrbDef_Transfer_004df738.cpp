#pragma optimize("s",on)
struct Field { unsigned char b[4]; };
class CPersistContext { public:                       // typed transfer helpers: each starts `push 0x122d70e ; call 0x404500` (CheckCRC)
    template <class T> void Transfer(T& value);                 // 0x00431102 Transfer(long&)
};
class CContextSensitiveItemDef { public:
    unsigned char _pad[0x28];
    Field f0; // 0x28
    Field f1; // 0x2c
    Field f2; // 0x30
    Field f3; // 0x34
    Field f4; // 0x38
    void Transfer(CPersistContext& ctx);
};

void CContextSensitiveItemDef::Transfer(CPersistContext& ctx)
{
    ctx.Transfer(*(long*)(&f0));
    ctx.Transfer(*(float*)(&f1));
    ctx.Transfer(*(float*)(&f2));
    ctx.Transfer(*(float*)(&f3));
    ctx.Transfer(*(long*)(&f4));
}