#pragma optimize("s",on)
struct Field { unsigned char b[4]; };
struct CPersistContext {                       // typed transfer helpers: each starts `push 0x122d70e ; call 0x404500` (CheckCRC)
    void Transfer_004522f4(void* value);            // 0x004522F4 per-type transfer instantiation, element type not attributed
    void TransferBool(void* value);                 // 0x0043314A Transfer(bool&)
    void TransferLong(void* value);                 // 0x00431102 Transfer(long&)
};
struct CContextSensitiveItemDef {
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
    ctx.Transfer_004522f4(&f0);
    ctx.TransferBool(&f1);
    ctx.TransferLong(&f2);
    ctx.TransferLong(&f3);
    ctx.TransferLong(&f4);
}