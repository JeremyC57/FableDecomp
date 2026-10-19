#pragma optimize("s",on)
struct Field { unsigned char b[4]; };
struct CPersistContext {                       // typed transfer helpers: each starts `push 0x122d70e ; call 0x404500` (CheckCRC)
    void TransferCharString(void* value);           // 0x00431143 Transfer(CCharString&)
    void TransferULong(void* value);                // 0x00431020 Transfer(unsigned long&): PDB-typed ulong members
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
    ctx.TransferCharString(&f0);
    ctx.TransferCharString(&f1);
    ctx.TransferULong(&f2);
    ctx.TransferULong(&f3);
    ctx.TransferULong(&f4);
}