// behaviour test for 0x00B0C730 CPSCUpdateNormal::KillAllDecalEmitters
#include <new>
#include <cstdio>
#include <cstdlib>
static int g_dtors = 0;
struct CParticleSystemCost { float PolyCount, PixelCount, LightCount, RenderCost, SetupCost, LightCost; };
struct CParticleEmitter {
    unsigned short Dead : 1; unsigned short Initialised : 1; int kills;
    bool IsDead() const { return Dead; }
    void Kill() { ++kills; }
    void GetEmitterCost(CParticleSystemCost& c) { c.LightCost += 1; c.RenderCost += 2; c.SetupCost += 3; }
};
struct CEmitterPointer {
    CParticleEmitter* item;
    virtual ~CEmitterPointer() { ++g_dtors; }
    CParticleEmitter* Get() const { return item; }
};
struct SNode { SNode* next; SNode* prev; CEmitterPointer data; };
struct CList {
    SNode* head;
    CList() { head = (SNode*)std::malloc(sizeof(SNode)); head->next = head->prev = head; }
    void push(CParticleEmitter* e) { SNode* n = (SNode*)std::malloc(sizeof(SNode)); new (&n->data) CEmitterPointer(); n->data.item = e; n->next = head; n->prev = head->prev; head->prev->next = n; head->prev = n; }
    void clear() {
        SNode* cur = head->next;
        while (cur != head) { SNode* tmp = cur; cur = cur->next; tmp->data.~CEmitterPointer(); std::free(tmp); }
        head->next = head; head->prev = head;
    }
};
struct CPSCUpdateNormal {
    CList DecalEmitterList;
    void KillAllDecalEmitters()
    {
        for (SNode* n = DecalEmitterList.head->next; n != DecalEmitterList.head; n = n->next)
            if (n->data.Get()) n->data.Get()->Kill();
        DecalEmitterList.clear();
    }
};
int main()
{
    CParticleEmitter a; a.Dead = 0; a.Initialised = 1; a.kills = 0;
    CParticleEmitter b = a;
    CPSCUpdateNormal u; u.DecalEmitterList.push(&a); u.DecalEmitterList.push(0); u.DecalEmitterList.push(&b);
    u.KillAllDecalEmitters();
    if (a.kills != 1 || b.kills != 1 || g_dtors != 3 || u.DecalEmitterList.head->next != u.DecalEmitterList.head)
    { std::printf("00b0c730_TEST FAIL\n"); return 1; }
    std::printf("00b0c730_TEST PASS\n");
    return 0;
}
