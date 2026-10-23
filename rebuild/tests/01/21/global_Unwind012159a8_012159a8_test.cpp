#include <cstdio>
#include <new>
static int g_ip = 0, g_set = 0; static void* g_item = 0; struct Ip { unsigned char b[8]; Ip() { ++g_ip; } ~Ip() { --g_ip; } void Set(void* i) { ++g_set; g_item = i; } }; struct V { float x, y, z; V() {} V(const V& o) : x(o.x), y(o.y), z(o.z) {} }; struct Fd { void* th; V pos; Ip ip; void* tg; }; struct Rq { long t; Fd food; long m20; Rq(long T) : t(T), food() { food.pos.x = 0; food.pos.y = 0; food.pos.z = 0; } Fd& Food() { return food; } };
static long g_type = 0; static void* g_th = 0; static float g_z = 0; static void* g_tg = 0; struct X { void Issue(Rq* r) { g_type = r->t; g_th = r->food.th; g_z = r->food.pos.z; g_tg = r->food.tg; } void Update(void* th, void* it, const V* p, void* tg) { Rq r(1); Fd& f = r.Food(); f.th = th; f.ip.Set(it); f.pos = *p; f.tg = tg; Issue(&r); } };
static bool Check() { X x; int a, b, c; V v; v.x = 1; v.y = 2; v.z = 3; x.Update(&a, &b, &v, &c); return g_type == 1 && g_th == &a && g_item == &b && g_set == 1 && g_z == 3 && g_tg == &c && g_ip == 0; }

int main()
{
    if (!Check()) { std::printf("00c484b0_TEST FAIL\n"); return 1; }
    std::printf("012159a8_TEST PASS\n");
    return 0;
}