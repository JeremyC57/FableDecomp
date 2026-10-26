#pragma optimize("s",on)
struct Field { unsigned char b[4]; };
class CPersistContext { public:                       // typed transfer helpers: each starts `push 0x122d70e ; call 0x404500` (CheckCRC)
    void TransferLong(void* value);                 // 0x00431102 Transfer(long&)
    void TransferFloat(void* value);                // 0x00431061 Transfer(float&)
    void Transfer_004e59e1(void* value);            // 0x004E59E1 per-type transfer instantiation, element type not attributed
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
    ctx.TransferLong(&f0);
    ctx.TransferFloat(&f1);
    ctx.TransferFloat(&f2);
    ctx.TransferFloat(&f3);
    ctx.Transfer_004e59e1(&f4);
}