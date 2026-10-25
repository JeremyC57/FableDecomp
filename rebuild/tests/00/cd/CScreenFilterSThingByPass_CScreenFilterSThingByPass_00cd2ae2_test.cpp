// Behaviour test for 00cd2ae2: CScreenFilterSThingByPass copy ctor -- the thing is copy-constructed (handle reference taken) and the bypass flag copied
#include <cstdio>
#include <new>
static int g_copies = 0;
struct Info { int refs; };
struct Thing {
    void* vt; void* data; Info* info;
    Thing() : vt(0), data(0), info(0) { }
    Thing(const Thing& o) : vt(o.vt), data(o.data), info(o.info) { ++g_copies; if (info) ++info->refs; }
};
struct ThingByPass { Thing thing; bool bypassSet; ThingByPass() : bypassSet(false) { } ThingByPass(const ThingByPass& o) : thing(o.thing), bypassSet(o.bypassSet) { } };
int main()
{
    Info inf = { 1 }; int obj = 0;
    ThingByPass a; a.thing.data = &obj; a.thing.info = &inf; a.bypassSet = true;
    unsigned char raw[sizeof(ThingByPass)];
    ThingByPass* b = new (raw) ThingByPass(a);
    int ok = sizeof(ThingByPass) == 16 && b->thing.data == &obj && b->thing.info == &inf && inf.refs == 2 && b->bypassSet && g_copies == 1;
    std::printf(ok ? "00cd2ae2_TEST PASS\n" : "00cd2ae2_TEST FAIL\n");
    return ok ? 0 : 1;
}
