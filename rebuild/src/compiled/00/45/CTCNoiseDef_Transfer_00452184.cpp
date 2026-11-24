// Leaf: ?Transfer@CTCNoiseDef@@UAEXAAVCPersistContext@@@Z
#pragma optimize("s",on)
class CPersistContext { public:
    void TransferA(int* v);
    template <class T> void Transfer(T& value);
};

class CTCNoiseDef { public:
    char pad[0x24];
    int a;
    int b;
    int c;
    virtual void Transfer(class CPersistContext&);
};

void CTCNoiseDef::Transfer(class CPersistContext& ctx)
{
    CTCNoiseDef* self = this;
    ctx.TransferA(&self->a);
    ctx.Transfer(*(float*)(&self->b));
    ctx.Transfer(*(float*)(&self->c));
}
// Flatten route (flatten_pilot v4, NF324): CTCNoiseDef_Transfer -> CTCNoiseDef::Transfer, the RTTI-placed PDB member; class model: the row's local class model.
