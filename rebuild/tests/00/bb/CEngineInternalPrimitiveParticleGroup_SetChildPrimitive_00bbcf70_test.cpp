// behaviour test for 00bbcf70 CEngineInternalPrimitiveParticleGroup::SetChildPrimitive
#include <cstdio>
#include <vector>
#include <list>
struct Prim { int Type; int RefCount; bool Enable; int updates; bool destroyed; };
static Prim g_pool[8]; static int g_next;
static Prim* Create(int type) { Prim* p = &g_pool[g_next++]; p->Type = type; p->RefCount = 0; p->Enable = false; p->updates = 0; p->destroyed = false; return p; }
struct Handle { Prim* Data; };
struct It { unsigned char used; std::list<Handle>::iterator node; };
struct Group {
    std::list<Handle> Primitives; std::vector<It> Ids;
    void SetChildPrimitive(unsigned long id, int type, unsigned long flags)
    {
        if (id >= Ids.size() || !Ids[id].used) {
            Handle h = { 0 };
            Primitives.push_front(h);
            if (Ids.size() <= id) { It blank; blank.used = 0; Ids.resize(id + 1, blank); }
            Ids[id].used = 1; Ids[id].node = Primitives.begin();
        }
        Handle& handle = *Ids[id].node;
        Prim* child = 0;
        if (handle.Data) {
            child = handle.Data;
            if (child->Type != type) { if (--child->RefCount == 0) child->destroyed = true; handle.Data = 0; }
        }
        if (!handle.Data) { child = Create(type); handle.Data = child; ++child->RefCount; }
        ++child->updates;
        child->Enable = (unsigned char)(~(flags >> 5)) & 1;
    }
};
int main()
{
    int bad = 0;
    Group g;
    g.SetChildPrimitive(2, 7, 0);
    if (g.Ids.size() != 3 || !g.Ids[2].used || g.Ids[0].used || g.Primitives.size() != 1) bad |= 1;
    Prim* first = g.Ids[2].node->Data;
    if (!first || first->Type != 7 || !first->Enable || first->updates != 1) bad |= 2;
    g.SetChildPrimitive(2, 7, 0x20);
    if (g.Ids[2].node->Data != first || first->Enable || first->updates != 2 || g.Primitives.size() != 1) bad |= 4;
    g.SetChildPrimitive(2, 9, 0);
    if (!first->destroyed || g.Ids[2].node->Data == first || g.Ids[2].node->Data->Type != 9) bad |= 8;
    g.SetChildPrimitive(0, 3, 0);
    if (g.Primitives.size() != 2 || g.Ids[0].node != g.Primitives.begin()) bad |= 16;
    if (bad) { std::printf("00bbcf70_TEST FAIL\n"); return 1; }
    std::printf("00bbcf70_TEST PASS\n");
    return 0;
}