#pragma optimize("s",on)
struct Field { unsigned char b[4]; };
struct CPersistContext {                       // typed transfer helpers: each starts `push 0x122d70e ; call 0x404500` (CheckCRC)
    void TransferDefString(void* value);            // 0x0045228F Transfer(CDefString&): dword write / CDefString read 0x0044FC00
    void TransferBool(void* value);                 // 0x0043314A Transfer(bool&)
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
    ctx.TransferDefString(&f0);
    ctx.TransferDefString(&f1);
    ctx.TransferDefString(&f2);
    ctx.TransferDefString(&f3);
    ctx.TransferBool(&f4);
}