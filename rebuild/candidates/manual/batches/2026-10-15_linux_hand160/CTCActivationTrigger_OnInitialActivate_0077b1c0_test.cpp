// Behaviour test for CTCActivationTrigger::OnInitialActivate (0x0077B1C0): with a UID, the receptor thing's interface 0x65
// component is taken (when present) and told about the trigger; nothing happens for UID 0.
#include <cstdio>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
struct Trigger;
static Trigger* g_registered; static int g_calls;
struct Receptor { void RegisterTrigger(Trigger* t) { g_registered = t; ++g_calls; } };
struct Thing {
    unsigned int bits[9]; Receptor* comp;
    bool GetInterface(Receptor** out, int type) { if (bits[type >> 5] & (1u << (type & 31))) { *out = comp; return true; } return false; }
};
static Thing* g_found; static u64 g_uid;
static Thing* GetThingByUID(u64 uid) { g_uid = uid; return g_found; }
struct Trigger {
    Receptor* PTCReceptor; u64 ReceptorUID;
    void OnInitialActivate()
    {
        if (ReceptorUID > 0) {
            Thing* receptor = GetThingByUID(ReceptorUID);
            receptor->GetInterface(&PTCReceptor, 0x65);
            PTCReceptor->RegisterTrigger(this);
        }
    }
};
int main()
{
    Receptor r; Thing t; for (int i = 0; i < 9; ++i) t.bits[i] = 0; t.bits[3] = 1u << 5; t.comp = &r; g_found = &t;
    Trigger tr; tr.PTCReceptor = 0; tr.ReceptorUID = (((u64)1 << 32) | 5);
    tr.OnInitialActivate();
    if (tr.PTCReceptor != &r || g_registered != &tr || g_uid != (((u64)1 << 32) | 5)) { std::printf("0077b1c0_TEST FAIL\n"); return 1; }
    tr.ReceptorUID = 0; tr.OnInitialActivate();
    if (g_calls != 1) { std::printf("0077b1c0_TEST FAIL\n"); return 1; }
    std::printf("0077b1c0_TEST PASS\n");
    return 0;
}
