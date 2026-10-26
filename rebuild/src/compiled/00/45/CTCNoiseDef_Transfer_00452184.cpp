#pragma optimize("s",on)
class CPersistContext { public:
    void TransferA(int* v);
    template <class T> void Transfer(T& value);
};

class CTCNoiseDef { public:
    char pad[0x28];
    int a;
    int b;
    int c;
};

void __fastcall CTCNoiseDef_Transfer(CTCNoiseDef* self, void* edx, CPersistContext* ctx)
{
    ctx->TransferA(&self->a);
    ctx->Transfer(*(float*)(&self->b));
    ctx->Transfer(*(float*)(&self->c));
}