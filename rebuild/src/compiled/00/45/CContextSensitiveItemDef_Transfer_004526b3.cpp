#pragma optimize("s",on)
struct Field { unsigned char b[4]; };
class CPersistContext { public:
    void PersistA_004526f7(void* p);
    template <class T> void Transfer(T& value);
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
    ctx.PersistA_004526f7(&f0);
    ctx.Transfer(*(long*)(&f1));
    ctx.Transfer(*(long*)(&f2));
    ctx.Transfer(*(long*)(&f3));
    ctx.Transfer(*(long*)(&f4));
}