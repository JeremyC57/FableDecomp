#include <cstdio>
int g_destroyed = 0;
void Destroy(void*) { ++g_destroyed; }
struct Info { long refs; void (*destroy)(void*); void* obj; };
struct ScriptThing {
    void* data; Info* info;
    ScriptThing() : data(0), info(0) {}
    ~ScriptThing() { if (info) { if (--info->refs == 0) { info->destroy(info->obj); delete info; } } data = 0; info = 0; }
    ScriptThing& operator=(const ScriptThing& o) {
        Info* i = o.info; void* d = o.data;
        if (info != i) { if (info) { if (--info->refs == 0) { info->destroy(info->obj); delete info; } } data = d; info = i; if (i) ++i->refs; }
        return *this; }
};
struct Club { long RelevantRegion; bool c1, c2, c3, c4; ScriptThing Village;
    void Reset() { RelevantRegion = 6; Village = ScriptThing(); c1 = c2 = c3 = c4 = false; } };
int main()
{
    Club c; c.RelevantRegion = 2; c.c1 = c.c2 = c.c3 = c.c4 = true;
    int x; Info* i = new Info; i->refs = 1; i->destroy = Destroy; i->obj = &x;
    c.Village.data = &x; c.Village.info = i;
    c.Reset();
    bool ok = c.RelevantRegion == 6 && !c.c1 && !c.c4 && c.Village.data == 0 && c.Village.info == 0 && g_destroyed == 1;
    c.Reset();
    ok = ok && g_destroyed == 1;
    if (!ok) { std::printf("00e82010_TEST FAIL\n"); return 1; }
    std::printf("00e82010_TEST PASS\n");
    return 0;
}
