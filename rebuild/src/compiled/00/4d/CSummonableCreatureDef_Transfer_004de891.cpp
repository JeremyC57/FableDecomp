#pragma optimize("s",on)
struct CMember { int x; };

struct CPersistContext {                       // typed transfer helpers: each starts `push 0x122d70e ; call 0x404500` (CheckCRC)
    void TransferFloat(CMember* value);             // 0x00431061 Transfer(float&)
    void TransferDefString(CMember* value);         // 0x0045228F Transfer(CDefString&): dword write / CDefString read 0x0044FC00
};

struct CGuildMasterDef {
    unsigned char pad0[0x28];
    CMember m28;
    CMember m2c;
};

void __fastcall CGuildMasterDef_Transfer(CGuildMasterDef* def, void* edx, CPersistContext* ctx)
{
    ctx->TransferFloat(&def->m28);
    ctx->TransferDefString(&def->m2c);
}