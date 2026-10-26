#pragma optimize("s",on)
class CPersistContext { public:                       // typed transfer helpers: each starts `push 0x122d70e ; call 0x404500` (CheckCRC)
    void Transfer_004dedda(int* value);             // 0x004DEDDA per-type transfer instantiation, element type not attributed
    void TransferFloat(int* value);                 // 0x00431061 Transfer(float&)
    void TransferLong(int* value);                  // 0x00431102 Transfer(long&)
};

class CTCNoiseDef { public:
    char pad[0x28];
    int a;
    int b;
    int c;
};

void __fastcall CTCNoiseDef_Transfer(CTCNoiseDef* def, void* edx, CPersistContext* ctx)
{
    ctx->Transfer_004dedda(&def->a);
    ctx->TransferFloat(&def->b);
    ctx->TransferLong(&def->c);
}