#include <cstdio>
// Behaviour test for 0x00455F1A: the reconstructed body (pragmas stripped) driven through recording stubs;
// checks the transfer order, the helper chosen per member (kind) and the member address.
static const void* g_p[32]; static int g_k[32]; static int g_n = 0;
static void Rec(int k, const void* p) { if (g_n < 32) { g_k[g_n] = k; g_p[g_n] = p; } ++g_n; }
class CTCNoiseDef { public: unsigned char _body[0x34]; };
class CPersistContext { public:
    template <class T> void TransferSubComponent(const char* name, T& value);
};
class CThingBaseDef { public:
    virtual void Transfer(CPersistContext& context);   // 0x00676BE0
    unsigned char _body04[0x70 - 4];                   // retail def base (Ego_r.pdb CThingBaseDef 0x74 less 4)
};
class CThingNoiseDef : public CThingBaseDef { public:
    CTCNoiseDef Noise;                 // +0x70
    virtual void Transfer(CPersistContext& context);
};
template <> void CPersistContext::TransferSubComponent<CTCNoiseDef>(const char* name, CTCNoiseDef& value);

void CThingNoiseDef::Transfer(CPersistContext& context)
{
    CThingBaseDef::Transfer(context);
    context.TransferSubComponent("Noise", Noise);
}
void CThingBaseDef::Transfer(CPersistContext& context) { Rec(1, this); }
template <> void CPersistContext::TransferSubComponent<CTCNoiseDef>(const char* name, CTCNoiseDef& v)
{ if (name[0] == 'N' && name[4] == 'e') Rec(15, &v); else Rec(99, &v); }
static CThingNoiseDef d;
int main()
{
    CPersistContext c;
    d.Transfer(c);
    bool ok = g_n == 2;
    ok = ok && g_k[0] == 1 && g_p[0] == (const void*)&d;
    ok = ok && g_k[1] == 15 && g_p[1] == (const void*)&d.Noise;
    if (ok) { std::printf("CThingNoiseDef_00455f1a_TEST PASS\n"); return 0; }
    std::printf("FAIL %d\n", g_n);
    return 1;
}
