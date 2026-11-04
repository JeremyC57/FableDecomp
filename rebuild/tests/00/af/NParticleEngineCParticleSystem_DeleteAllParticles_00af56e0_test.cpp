// Behaviour test for NParticleEngine::CParticleSystem::DeleteAllParticles (0x00AF56E0): a model of the row over a
// three-particle list -- every particle is cleared once and deleted, every node freed, the list left empty and the count 0.
#include <stdio.h>
#include <stdlib.h>
static int g_cleared = 0, g_deleted = 0, g_freed = 0;
struct CParticle { int id; virtual ~CParticle() { ++g_deleted; } };
struct SNode { SNode* next; SNode* prev; CParticle* data; };
static void node_free(void* p) { ++g_freed; free(p); }
struct CList {
    SNode* head;
    SNode* begin() { return head->next; }
    SNode* end() { return head; }
    SNode* erase(SNode* pos) { SNode* n = pos->next; SNode* p = pos->prev; p->next = n; n->prev = p; node_free(pos); return n; }
    void clear() { SNode* c = head->next; while (c != head) { SNode* t = c; c = c->next; node_free(t); } head->next = head; head->prev = head; }
};
struct CParticleSystem {
    CList Particles; long NoActiveParticles;
    void ClearParticle(SNode*& it) { if (it->data) ++g_cleared; }
    void DeleteAllParticles() {
        SNode* it = Particles.begin();
        while (it != Particles.end()) { ClearParticle(it); delete it->data; it = Particles.erase(it); --NoActiveParticles; }
        Particles.clear(); NoActiveParticles = 0;
    }
};
int main()
{
    CParticleSystem s; SNode* h = (SNode*)malloc(sizeof(SNode)); h->next = h; h->prev = h; s.Particles.head = h; s.NoActiveParticles = 0;
    for (int i = 0; i < 3; ++i) {
        SNode* n = (SNode*)malloc(sizeof(SNode)); n->data = new CParticle; n->data->id = i;
        n->prev = h->prev; n->next = h; h->prev->next = n; h->prev = n; ++s.NoActiveParticles;
    }
    s.NoActiveParticles = 5;   // the loop counts down, then the count is zeroed
    s.DeleteAllParticles();
    int ok = g_cleared == 3 && g_deleted == 3 && g_freed == 3 && h->next == h && h->prev == h && s.NoActiveParticles == 0;
    free(h);
    printf(ok ? "00af56e0_TEST PASS\n" : "00af56e0_TEST FAIL\n");
    return ok ? 0 : 1;
}