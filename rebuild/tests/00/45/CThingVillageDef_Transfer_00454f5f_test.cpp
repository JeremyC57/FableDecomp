#include <cstdio>
// Behaviour test for 0x00454F5F: the reconstructed body (pragmas stripped) driven through recording stubs;
// checks the transfer order, the helper chosen per member (kind) and the member address.
static const void* g_p[32]; static int g_k[32]; static int g_n = 0;
static void Rec(int k, const void* p) { if (g_n < 32) { g_k[g_n] = k; g_p[g_n] = p; } ++g_n; }
class CEngineGraphic { public: unsigned char _body[0x0e]; };   // retail 14 bytes
class CPersistContext { public:
    template <class T> void Transfer(T& value);       // one-argument transfer helpers (namepass TR2)
};
class CThingBaseDef { public:
    virtual void Transfer(CPersistContext& context);   // 0x00676BE0
    unsigned char _body04[0x70 - 4];                   // retail def base (Ego_r.pdb CThingBaseDef 0x74 less 4)
};
class CThingVillageDef : public CThingBaseDef { public:
    CEngineGraphic Graphic;         // +0x70
    virtual void Transfer(CPersistContext& context);
};
template <> void CPersistContext::Transfer<CEngineGraphic>(CEngineGraphic& value);

void CThingVillageDef::Transfer(CPersistContext& context)
{
    CThingBaseDef::Transfer(context);
    context.Transfer(Graphic);
}
template <> void CPersistContext::Transfer<CEngineGraphic>(CEngineGraphic& v) { Rec(6, &v); }
void CThingBaseDef::Transfer(CPersistContext& context) { Rec(1, this); }
static CThingVillageDef d;
int main()
{
    CPersistContext c;
    d.Transfer(c);
    bool ok = g_n == 2;
    ok = ok && g_k[0] == 1 && g_p[0] == (const void*)&d;
    ok = ok && g_k[1] == 6 && g_p[1] == (const void*)&d.Graphic;
    if (ok) { std::printf("CThingVillageDef_00454f5f_TEST PASS\n"); return 0; }
    std::printf("FAIL %d\n", g_n);
    return 1;
}
