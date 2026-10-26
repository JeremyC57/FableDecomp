#pragma optimize("s",on)
class CPersistContext { public:                       // typed transfer helpers: each starts `push 0x122d70e ; call 0x404500` (CheckCRC)
    void Transfer_004dedda(int* value);             // 0x004DEDDA per-type transfer instantiation, element type not attributed
    template <class T> void Transfer(T& value);                 // 0x00431061 Transfer(float&)
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
    ctx->Transfer(*(float*)(&def->b));
    ctx->Transfer(*(long*)(&def->c));
}