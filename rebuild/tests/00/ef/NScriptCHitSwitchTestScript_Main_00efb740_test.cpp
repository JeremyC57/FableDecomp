// behaviour test for 0x00EFB740 CHitSwitchTestScript::Main() (model: one binding named "HitSwitch" with the allocator, flags 0, then both post-adds)
#include <cstdio>
#include <cstring>
#include <string>
struct Binding { std::string name; void* parent; void* alloc; bool valid; unsigned long flags; };
static Binding* g_b = 0; static int g_post = 0, g_gamePost = 0;
static void Alloc() { }
struct Script {
    void AddEntityScriptBinding(Binding* b) { g_b = b; }
    void PostAddScriptedEntities() { ++g_post; }
    void GamePost() { ++g_gamePost; }
    void Main()
    {
        Binding* b = new Binding; b->name = "HitSwitch"; b->parent = this; b->alloc = (void*)&Alloc; b->valid = true; b->flags = 0;
        AddEntityScriptBinding(b);
        PostAddScriptedEntities();
        GamePost();
    }
};
int main()
{
    Script s; s.Main();
    int ok = g_b && g_b->name == "HitSwitch" && g_b->parent == &s && g_b->valid && g_b->flags == 0 && g_post == 1 && g_gamePost == 1;
    delete g_b;
    if (!ok) { std::printf("00efb740_TEST FAIL\n"); return 1; }
    std::printf("00efb740_TEST PASS\n");
    return 0;
}