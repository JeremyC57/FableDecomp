#include <cstdio>
// Behaviour test for 0x00454FC3: the reconstructed body (pragmas stripped) driven through recording stubs;
// checks the transfer order, the helper chosen per member (kind) and the member address.
static const void* g_p[32]; static int g_k[32]; static int g_n = 0;
static void Rec(int k, const void* p) { if (g_n < 32) { g_k[g_n] = k; g_p[g_n] = p; } ++g_n; }
enum EPSwitchTriggerType { };
class CEngineGraphic { public: unsigned char _body[0x0e]; };   // retail 14 bytes
class CPersistContext { public:
    template <class T> void Transfer(T& value);       // one-argument transfer helpers (namepass TR2)
};
class CThingBaseDef { public:
    virtual void Transfer(CPersistContext& context);   // 0x00676BE0
    unsigned char _body04[0x70 - 4];                   // retail def base (Ego_r.pdb CThingBaseDef 0x74 less 4)
};
class CThingPhysicalSwitchDef : public CThingBaseDef { public:
    EPSwitchTriggerType TriggerType; // +0x70
    CEngineGraphic UntriggeredGraphic; // +0x74
    CEngineGraphic TriggeredGraphic; // +0x82
    long TriggerSound;              // +0x90
    virtual void Transfer(CPersistContext& context);
};
template <> void CPersistContext::Transfer<CEngineGraphic>(CEngineGraphic& value);
template <> void CPersistContext::Transfer<EPSwitchTriggerType>(EPSwitchTriggerType& value);
template <> void CPersistContext::Transfer<long>(long& value);

void CThingPhysicalSwitchDef::Transfer(CPersistContext& context)
{
    CThingBaseDef::Transfer(context);
    context.Transfer(TriggerType);
    context.Transfer(UntriggeredGraphic);
    context.Transfer(TriggeredGraphic);
    context.Transfer(TriggerSound);
}
template <> void CPersistContext::Transfer<CEngineGraphic>(CEngineGraphic& v) { Rec(6, &v); }
template <> void CPersistContext::Transfer<EPSwitchTriggerType>(EPSwitchTriggerType& v) { Rec(11, &v); }
template <> void CPersistContext::Transfer<long>(long& v) { Rec(3, &v); }
void CThingBaseDef::Transfer(CPersistContext& context) { Rec(1, this); }
static CThingPhysicalSwitchDef d;
int main()
{
    CPersistContext c;
    d.Transfer(c);
    bool ok = g_n == 5;
    ok = ok && g_k[0] == 1 && g_p[0] == (const void*)&d;
    ok = ok && g_k[1] == 11 && g_p[1] == (const void*)&d.TriggerType;
    ok = ok && g_k[2] == 6 && g_p[2] == (const void*)&d.UntriggeredGraphic;
    ok = ok && g_k[3] == 6 && g_p[3] == (const void*)&d.TriggeredGraphic;
    ok = ok && g_k[4] == 3 && g_p[4] == (const void*)&d.TriggerSound;
    if (ok) { std::printf("CThingPhysicalSwitchDef_00454fc3_TEST PASS\n"); return 0; }
    std::printf("FAIL %d\n", g_n);
    return 1;
}
