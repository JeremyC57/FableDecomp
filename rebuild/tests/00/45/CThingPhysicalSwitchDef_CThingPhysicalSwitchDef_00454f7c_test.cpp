// Behaviour test for 00454f7c CThingPhysicalSwitchDef::CThingPhysicalSwitchDef (copy of the model; the
// CThingBaseDef stand-in fills its bytes with a marker so untouched members are visible).
#include <cstdio>
#include <cstring>
#include <new>
struct CThingBaseDef {
    CThingBaseDef() { std::memset(_body, 0x5A, sizeof(_body)); }
    virtual ~CThingBaseDef() {}
    unsigned char _body[0x6c];
};
#pragma pack(push, 2)
struct CEngineGraphic {
    unsigned long BankIndex;
    float         AnimStep;
    float         RenderSizeX;
    bool          AdditiveAlpha;
    unsigned char Type;
    CEngineGraphic() : BankIndex(0), AnimStep(0), RenderSizeX(0), AdditiveAlpha(false), Type(0) {}
};
#pragma pack(pop)
#pragma pack(push, 2)
struct CThingPhysicalSwitchDef : CThingBaseDef {
    long           TriggerType;
    CEngineGraphic UntriggeredGraphic;
    CEngineGraphic TriggeredGraphic;
    long           TriggerSound;
    CThingPhysicalSwitchDef();
};
#pragma pack(pop)
CThingPhysicalSwitchDef::CThingPhysicalSwitchDef()
{
}
int main()
{
    static unsigned char raw[sizeof(CThingPhysicalSwitchDef) + 8];
    std::memset(raw, 0x77, sizeof(raw));
    CThingPhysicalSwitchDef* d = new (raw) CThingPhysicalSwitchDef;
    int ok = 1;
    if (d->UntriggeredGraphic.BankIndex != 0 || d->UntriggeredGraphic.AnimStep != 0.0f || d->UntriggeredGraphic.RenderSizeX != 0.0f) ok = 0;
    if (d->UntriggeredGraphic.AdditiveAlpha || d->UntriggeredGraphic.Type != 0) ok = 0;
    if (d->TriggeredGraphic.BankIndex != 0 || d->TriggeredGraphic.AnimStep != 0.0f || d->TriggeredGraphic.AdditiveAlpha || d->TriggeredGraphic.Type != 0) ok = 0;
    const unsigned char* tt = (const unsigned char*)&d->TriggerType;
    const unsigned char* ts = (const unsigned char*)&d->TriggerSound;
    for (unsigned i = 0; i < sizeof(long); ++i) if (tt[i] != 0x77 || ts[i] != 0x77) ok = 0;   // left alone
    if (d->_body[0] != 0x5A) ok = 0;
    d->~CThingPhysicalSwitchDef();
    if (!ok) { std::printf("00454f7c_TEST FAIL\n"); return 1; }
    std::printf("00454f7c_TEST PASS\n");
    return 0;
}