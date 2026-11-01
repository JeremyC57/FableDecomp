// Behaviour test for CTCHeroMorph::KillParticles (0x0071C260): pop every particle emitter from the back, killing the live
// ones, then the idle emitters the same way (KillIdleParticles 0x0071C1F0).
#include <cstdio>
#include <cstdlib>
static int g_order[16];
static int g_kills = 0;
static int g_ptrDtors = 0;
class CThing { public: int id; bool killed; void Kill(bool) { killed = true; g_order[g_kills++] = id; } };
class CIntelligentPointer { public:
    CThing* item;
    virtual ~CIntelligentPointer() { ++g_ptrDtors; }
    CThing* Get() const { return item; }
};
struct Node { Node* next; Node* prev; CIntelligentPointer data; Node() { } };
static void PushBack(Node* head, CThing* t)
{
    Node* n = new Node;
    n->data.item = t;
    n->next = head; n->prev = head->prev; head->prev->next = n; head->prev = n;
}
static void PopBack(Node* head)
{
    Node* n = head->prev;
    n->prev->next = n->next;
    n->next->prev = n->prev;
    delete n;                                            // element dtor + free
}
static void KillAll(Node* head)
{
    while (head->next != head) {
        Node* it = head->prev;
        if (it->data.Get())
            head->prev->data.Get()->Kill(true);
        PopBack(head);
    }
}
class CTCHeroMorph { public:
    Node ParticleEmitters, IdleParticleEmitters;
    void KillIdleParticles() { KillAll(&IdleParticleEmitters); }
    void KillParticles() { KillAll(&ParticleEmitters); KillIdleParticles(); }
};
int main()
{
    CThing t[5];
    for (int i = 0; i < 5; ++i) { t[i].id = i; t[i].killed = false; }
    CTCHeroMorph m;
    m.ParticleEmitters.next = m.ParticleEmitters.prev = &m.ParticleEmitters;
    m.IdleParticleEmitters.next = m.IdleParticleEmitters.prev = &m.IdleParticleEmitters;
    PushBack(&m.ParticleEmitters, &t[0]);
    PushBack(&m.ParticleEmitters, 0);                    // dead emitter: dropped, no Kill
    PushBack(&m.ParticleEmitters, &t[1]);
    PushBack(&m.IdleParticleEmitters, &t[2]);
    PushBack(&m.IdleParticleEmitters, &t[3]);
    m.KillParticles();
    int want[4] = { 1, 0, 3, 2 };
    if (g_kills != 4 || g_ptrDtors != 5 || m.ParticleEmitters.next != &m.ParticleEmitters || m.IdleParticleEmitters.next != &m.IdleParticleEmitters) { std::printf("0071c260_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 4; ++i) if (g_order[i] != want[i]) { std::printf("0071c260_TEST FAIL\n"); return 1; }
    if (t[4].killed) { std::printf("0071c260_TEST FAIL\n"); return 1; }
    std::printf("0071c260_TEST PASS\n");
    return 0;
}