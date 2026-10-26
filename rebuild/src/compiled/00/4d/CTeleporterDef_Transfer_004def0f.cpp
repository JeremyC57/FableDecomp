#pragma optimize("s",on)
class CPersistContext { public:                       // typed transfer helpers: each starts `push 0x122d70e ; call 0x404500` (CheckCRC)
    void TransferFloat(int* value);                 // 0x00431061 Transfer(float&)
    void TransferLong(int* value);                  // 0x00431102 Transfer(long&)
    void TransferBool(int* value);                  // 0x0043314A Transfer(bool&)
};

class CTCNoiseDef { public:
    char pad[0x28];
    int a;
    int b;
    int c;
};

void __fastcall CTCNoiseDef_Transfer(CTCNoiseDef* def, void* edx, CPersistContext* ctx)
{
    ctx->TransferFloat(&def->a);
    ctx->TransferLong(&def->b);
    ctx->TransferBool(&def->c);
}