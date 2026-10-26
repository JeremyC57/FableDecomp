#include <cstdio>
// Behaviour test for 0x00452B04: the reconstructed body (pragmas stripped) driven through recording stubs;
// checks the transfer order, the helper chosen per member (kind) and the member address.
static const void* g_p[32]; static int g_k[32]; static int g_n = 0;
static void Rec(int k, const void* p) { if (g_n < 32) { g_k[g_n] = k; g_p[g_n] = p; } ++g_n; }
#include "engine/CHairCardDef.h"  // retyped onto the PDB layout; byte parity re-verified
class CPersistContext { public:
    template <class T> void Transfer(T& value);       // one-argument transfer helpers (namepass TR2)
};
template <> void CPersistContext::Transfer<long>(long& value);

void __fastcall CHairCardDef_Transfer(CHairCardDef* self, int /*edx*/, CPersistContext& context)
{
    context.Transfer(self->HairObject);
}
template <> void CPersistContext::Transfer<long>(long& v) { Rec(3, &v); }
static CHairCardDef d;
int main()
{
    CPersistContext c;
    CHairCardDef_Transfer(&d, 0, c);
    bool ok = g_n == 1;
    ok = ok && g_k[0] == 3 && g_p[0] == (const void*)&d.HairObject;
    if (ok) { std::printf("CHairCardDef_00452b04_TEST PASS\n"); return 0; }
    std::printf("FAIL %d\n", g_n);
    return 1;
}
