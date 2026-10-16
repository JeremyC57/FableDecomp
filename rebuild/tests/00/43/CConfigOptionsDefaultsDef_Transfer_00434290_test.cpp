#include <cstdio>
#include <cstddef>
// Behaviour test for CConfigOptionsDefaultsDef::Transfer @ 0x00434290: fourteen transfers in persist-list order.
static const void* g_order[32]; static int g_kind[32]; static int g_n = 0;
struct Def;
static const Def* g_def = 0;
struct CPersistContext {
    void TransferValue(long* p);
    void TransferValue(float* p);
    void TransferValue(unsigned long* p);
};
struct Def {
    long m_3c; float m_40, m_44, m_48, m_4c, m_50;
    unsigned long m_54, m_58, m_5c, m_60, m_64, m_68, m_6c, m_70;
    void Transfer(CPersistContext& context)
    {
        context.TransferValue(&m_3c);
        context.TransferValue(&m_40);
        context.TransferValue(&m_44);
        context.TransferValue(&m_48);
        context.TransferValue(&m_54);
        context.TransferValue(&m_58);
        context.TransferValue(&m_5c);
        context.TransferValue(&m_60);
        context.TransferValue(&m_64);
        context.TransferValue(&m_68);
        context.TransferValue(&m_6c);
        context.TransferValue(&m_70);
        context.TransferValue(&m_4c);
        context.TransferValue(&m_50);
    }
};
static const void* Off(const void* p) { return p; }
void CPersistContext::TransferValue(long* p) { g_kind[g_n] = 0; g_order[g_n++] = Off(p); }
void CPersistContext::TransferValue(float* p) { g_kind[g_n] = 1; g_order[g_n++] = Off(p); }
void CPersistContext::TransferValue(unsigned long* p) { g_kind[g_n] = 2; g_order[g_n++] = Off(p); }
int main()
{
    Def d; CPersistContext c; g_def = &d;
    d.Transfer(c);
    const void* exp[14] = {&d.m_3c,&d.m_40,&d.m_44,&d.m_48,&d.m_54,&d.m_58,&d.m_5c,&d.m_60,&d.m_64,&d.m_68,&d.m_6c,&d.m_70,&d.m_4c,&d.m_50};
    static const int kind[14] = {0,1,1,1,2,2,2,2,2,2,2,2,1,1};
    if (g_n != 14) { std::printf("00434290_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 14; ++i)
        if (g_kind[i] != kind[i] || g_order[i] != exp[i]) { std::printf("00434290_TEST FAIL\n"); return 1; }
    std::printf("00434290_TEST PASS\n");
    return 0;
}