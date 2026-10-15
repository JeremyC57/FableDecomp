#include <cstdio>
struct CTrapDef { long TrapType; long OwnerValue; };
struct CDefPointerTrapDef { CTrapDef* Object; };
static CTrapDef g_def;
struct CDefClassBase { bool GetSubDef(CDefPointerTrapDef* out) { out->Object = &g_def; return true; } };
struct COwnerObject60 { long received; int calls; virtual ~COwnerObject60() {} virtual void Receive(long v) { received = v; ++calls; } };
struct CComponentFlags { unsigned char Flag0 : 1; unsigned char Flag1 : 1; };
struct CThing { COwnerObject60* m_p60; CComponentFlags* m_p64; CDefClassBase* Def; };
struct CTCTrapBase {
    int initCalls;
    CThing* Thing;
    CDefPointerTrapDef PTrapDef;
    long TrapType;
    virtual ~CTCTrapBase() {}
    virtual void InitTrap() { ++initCalls; }
    const CTrapDef& GetTrapDef() const { return *PTrapDef.Object; }
    void OnInitialActivate()
    {
        Thing->Def->GetSubDef(&PTrapDef);
        TrapType = GetTrapDef().TrapType;
        if (GetTrapDef().OwnerValue != 0)
            Thing->m_p60->Receive(GetTrapDef().OwnerValue);
        InitTrap();
        Thing->m_p64->Flag1 = 1;
    }
};
int main()
{
    CDefClassBase defs; COwnerObject60 sink; sink.received = 0; sink.calls = 0;
    CComponentFlags flags; flags.Flag0 = 1; flags.Flag1 = 0;
    CThing thing; thing.m_p60 = &sink; thing.m_p64 = &flags; thing.Def = &defs;
    CTCTrapBase trap; trap.initCalls = 0; trap.Thing = &thing; trap.PTrapDef.Object = 0; trap.TrapType = -1;
    g_def.TrapType = 4; g_def.OwnerValue = 0;
    trap.OnInitialActivate();
    if (trap.PTrapDef.Object != &g_def || trap.TrapType != 4 || sink.calls != 0 || trap.initCalls != 1 || flags.Flag1 != 1 || flags.Flag0 != 1) { std::printf("00787d4f_TEST FAIL\n"); return 1; }
    g_def.OwnerValue = 77; g_def.TrapType = 2;
    trap.OnInitialActivate();
    if (trap.TrapType != 2 || sink.calls != 1 || sink.received != 77 || trap.initCalls != 2) { std::printf("00787d4f_TEST FAIL\n"); return 1; }
    std::printf("00787d4f_TEST PASS\n");
    return 0;
}
