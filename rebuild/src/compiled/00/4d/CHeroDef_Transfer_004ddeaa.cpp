#pragma optimize("s",on)
struct CMember { int x; };

struct CPersistContext {                       // typed transfer helpers: each starts `push 0x122d70e ; call 0x404500` (CheckCRC)
    void TransferLong(CMember* value);              // 0x00431102 Transfer(long&)
    void Transfer_004ddecb(CMember* value);         // 0x004DDECB per-type transfer instantiation, element type not attributed
};

struct CGuildMasterDef {
    unsigned char pad0[0x28];
    CMember m28;
    CMember m2c;
};

void __fastcall CGuildMasterDef_Transfer(CGuildMasterDef* def, void* edx, CPersistContext* ctx)
{
    ctx->TransferLong(&def->m28);
    ctx->Transfer_004ddecb(&def->m2c);
}