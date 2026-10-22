#pragma optimize("s",on)
struct CPersistContext {
    void PersistFloat(void* p);
    void PersistOther(void* p);
    void PersistSub(void* p);
};

struct CPhysicsDef {
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
    ctx->PersistFloat(&def->Diameter);
    ctx->PersistFloat(&def->Friction);
    ctx->PersistFloat(&def->Elasticity);
    ctx->PersistOther(&def->pad3c);
    ctx->PersistFloat((float*)&def->pad34);
    ctx->PersistFloat((float*)&def->pad38);
    ctx->PersistSub(&def->sub[0]);
}