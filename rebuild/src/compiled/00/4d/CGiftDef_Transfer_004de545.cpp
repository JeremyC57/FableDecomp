#pragma optimize("s",on)
struct CMember { int x; };

struct CPersistContext {                       // typed transfer helpers: each starts `push 0x122d70e ; call 0x404500` (CheckCRC)
    void Transfer_004de566(CMember* value);         // 0x004DE566 per-type transfer instantiation, element type not attributed
    void TransferBool(CMember* value);              // 0x0043314A Transfer(bool&)
};

struct CGuildMasterDef {
    unsigned char pad0[0x28];
    CMember m28;
    CMember m2c;
};

void __fastcall CGuildMasterDef_Transfer(CGuildMasterDef* def, void* edx, CPersistContext* ctx)
{
    ctx->Transfer_004de566(&def->m28);
    ctx->TransferBool(&def->m2c);
}