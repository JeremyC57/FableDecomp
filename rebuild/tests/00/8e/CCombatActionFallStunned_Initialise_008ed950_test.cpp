#include <cstdio>
struct Block { long refs; void* obj; };
struct Mode { bool Started; void* creature; Mode(void* c) : Started(true), creature(c) {} };
struct Ptr { Mode* obj; Block* count; Ptr(Mode* m) : obj(m) { count = m ? new Block() : 0; if (count) { count->refs = 1; count->obj = m; } } };
struct Manager { int added; Mode* last; long refs; void AddMode(Ptr p) { ++added; last = p.obj; refs = p.count->refs; delete p.count; } };
static int g_baseInit = 0;
struct Action {
    void* creature; Manager* manager;
    void Initialise()
    {
        Mode* mode = new Mode(creature);
        mode->Started = false;
        manager->AddMode(Ptr(mode));
        ++g_baseInit;
    }
};
int main()
{
    Manager m = { 0, 0, 0 };
    int creature = 5;
    Action a = { &creature, &m };
    a.Initialise();
    bool ok = m.added == 1 && m.last && !m.last->Started && m.last->creature == &creature && m.refs == 1 && g_baseInit == 1;
    delete m.last;
    if (!ok) { std::printf("008ed950_TEST FAIL\n"); return 1; }
    std::printf("008ed950_TEST PASS\n");
    return 0;
}