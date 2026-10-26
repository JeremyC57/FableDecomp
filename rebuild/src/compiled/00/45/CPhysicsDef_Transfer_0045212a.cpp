#pragma optimize("s",on)
class CPersistContext { public:
    template <class T> void Transfer(T& value);
};

class CPhysicsDef { public:
    unsigned char _pad[0x28];
    float Diameter;
    float Friction;
    float Elasticity;
    int   pad34;
    int   pad38;
    int   pad3c;
    unsigned char sub[4];
};

void __fastcall CPhysicsDef_Transfer(CPhysicsDef* def, int edx, CPersistContext* ctx)
{
    ctx->Transfer(*(float*)(&def->Diameter));
    ctx->Transfer(*(float*)(&def->Friction));
    ctx->Transfer(*(float*)(&def->Elasticity));
    ctx->Transfer(*(long*)(&def->pad3c));
    ctx->Transfer(*(float*)((float*)&def->pad34));
    ctx->Transfer(*(float*)((float*)&def->pad38));
    ctx->Transfer(*(bool*)(&def->sub[0]));
}