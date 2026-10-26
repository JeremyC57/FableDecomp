class CPersistContext;

struct CThingNoiseDef_Base
{
    char base_bytes[0x0c];
    void __fastcall Transfer_00c3a520(CPersistContext* ctx);
};

struct CThingNoiseMember
{
    void DoIt_00c39990(void* owner, int flag);
};

class CThingNoiseDef : public CThingNoiseDef_Base
{ public:
    CThingNoiseMember m0c;
    CThingNoiseDef* __fastcall Transfer(CPersistContext* ctx);
};

CThingNoiseDef* __fastcall CThingNoiseDef::Transfer(CPersistContext* ctx)
{
    this->CThingNoiseDef_Base::Transfer_00c3a520(ctx);
    this->m0c.DoIt_00c39990(this, 1);
    return this;
}