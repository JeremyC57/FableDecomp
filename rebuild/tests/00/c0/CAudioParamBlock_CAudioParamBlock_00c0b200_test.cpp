#include <cstdio>
#include <new>
#include <cstdlib>
#include <cstring>
static char g_nil[1] = { 0 };
struct Cs { char* p; Cs() : p(g_nil) {} Cs(const char* t) : p(g_nil) { Set(t); } Cs(const Cs& o) : p(g_nil) { Set(o.p); } ~Cs() { if (p != g_nil) std::free(p); } void Set(const char* t) { char* n = (char*)std::malloc(std::strlen(t) + 1); std::strcpy(n, t); if (p != g_nil) std::free(p); p = n; } const Cs& operator=(const char* t) { Set(t); return *this; } const Cs& operator=(const Cs& o) { Set(o.p); return *this; } Cs* Append(const char* t) { char* n = (char*)std::malloc(std::strlen(p) + std::strlen(t) + 1); std::strcpy(n, p); std::strcat(n, t); if (p != g_nil) std::free(p); p = n; return this; } Cs* AppendNumber(unsigned long v) { char n[16]; std::sprintf(n, "%lu", v); return Append(n); } };
static int g_fin = 0;
struct Slot { float gain, pitch; long m8, mc, m10, m14, m18; float vol; long m20, m24, m28, m2c; bool act; Slot() : act(false) {} void Reset() { pitch = 1.0f; gain = 1.0f; vol = 1.0f; m18 = 0; m14 = 0; m2c = 0; m28 = 0; m10 = 0; m24 = 0; mc = 0; m20 = 0; m8 = 0; } };
struct Ab { virtual void v0() {} Slot slots[16]; float gains[8]; Cs sn[16]; Cs gn[8]; void* owner; void Finish() { ++g_fin; } Ab(void* o) : owner(o) { for (unsigned long i = 0; i < 16; ++i) { slots[i].Reset(); sn[i] = *Cs("G").AppendNumber(i); } for (unsigned long j = 0; j < 8; ++j) { gains[j] = 1.0f; gn[j] = *Cs("P").AppendNumber(j); } Finish(); } };
static bool Check() { int o; Ab* a = new Ab(&o); bool ok = a->owner == &o && g_fin == 1 && std::strcmp(a->sn[0].p, "G0") == 0 && std::strcmp(a->sn[15].p, "G15") == 0 && std::strcmp(a->gn[7].p, "P7") == 0 && a->gains[3] == 1.0f && a->slots[5].gain == 1.0f && a->slots[5].vol == 1.0f && a->slots[5].m2c == 0 && !a->slots[5].act; delete a; return ok; }

int main()
{
    if (!Check()) { std::printf("00c0b200_TEST FAIL\n"); return 1; }
    std::printf("00c0b200_TEST PASS\n");
    return 0;
}