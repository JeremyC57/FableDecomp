#pragma optimize("s",on)
struct CMember { int x; };

class CPersistContext { public:                       // typed transfer helpers: each starts `push 0x122d70e ; call 0x404500` (CheckCRC)
    void TransferFloat(CMember* value);             // 0x00431061 Transfer(float&)
    void TransferLong(CMember* value);              // 0x00431102 Transfer(long&)
};

class CGuildMasterDef { public:
    unsigned char pad0[0x28];
    CMember m28;
    CMember m2c;
};

void __fastcall CGuildMasterDef_Transfer(CGuildMasterDef* def, void* edx, CPersistContext* ctx)
{
    ctx->TransferFloat(&def->m28);
    ctx->TransferLong(&def->m2c);
}