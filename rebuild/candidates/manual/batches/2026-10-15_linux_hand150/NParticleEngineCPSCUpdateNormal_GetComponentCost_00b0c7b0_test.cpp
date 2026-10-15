// behaviour test for 0x00B0C7B0 CPSCUpdateNormal::GetComponentCost
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
    void GetComponentCost(CParticleSystemCost& cost, long, long, long)
    {
        for (SNode* n = DecalEmitterList.head->next; n != DecalEmitterList.head; n = n->next)
            if (n->data.Get() && !n->data.Get()->IsDead() && n->data.Get()->Initialised) {
                n->data.Get()->GetEmitterCost(cost);
                cost.LightCost *= 2.0f; cost.RenderCost *= 2.0f; cost.SetupCost *= 2.0f;
            }
    }
};
int main()
{
    CParticleEmitter live; live.Dead = 0; live.Initialised = 1; live.kills = 0;
    CParticleEmitter dead = live; dead.Dead = 1;
    CParticleEmitter fresh = live; fresh.Initialised = 0;
    CPSCUpdateNormal u; u.DecalEmitterList.push(&live); u.DecalEmitterList.push(&dead); u.DecalEmitterList.push(0); u.DecalEmitterList.push(&fresh);
    CParticleSystemCost c = { 0, 0, 0, 1, 1, 1 };
    u.GetComponentCost(c, 0, 0, 0);
    if (c.LightCost != 4.0f || c.RenderCost != 6.0f || c.SetupCost != 8.0f)
    { std::printf("00b0c7b0_TEST FAIL\n"); return 1; }
    u.DecalEmitterList.clear();
    std::printf("00b0c7b0_TEST PASS\n");
    return 0;
}
