#include <cstdio>
#include <cstdlib>
// Behaviour test for NParticleEngine::CParticleEmitterList::FindVisible (0x00AEB2D0): dead emitters are erased,
// uninitialised ones initialise, visible ones are inserted into the caller's list which is trimmed back to maxCount from
// the far end (clearing decals and particles), invisible die-offscreen emitters are killed and erased, and other
// invisible emitters just clear.
struct Info { float alwaysUpdateDistance; float distance; int engine; };
struct Emitter {
    int id; bool dead, initialised, alwaysUpdate, continuous, dieOffscreen; float range;
    int inits, kills, clears; float lastDistance;
    bool Initialise(int) { ++inits; initialised = true; return true; }
    void Kill() { ++kills; }
    bool IsVisible(float d, const Info*) { lastDistance = d; return range <= d; }
    void ClearAllDecals(int) { ++clears; }
    void ClearAllParticles(int) { ++clears; }
};
struct Node { Node* next; Node* prev; Emitter* data; };
struct List {
    Node head;
    List() { head.next = head.prev = &head; }
    void push_back(Emitter* e) { Node* n = (Node*)std::malloc(sizeof(Node)); n->data = e; n->next = &head; n->prev = head.prev; head.prev->next = n; head.prev = n; }
    unsigned long size() const { unsigned long c = 0; for (Node* p = head.next; p != &head; p = p->next) ++c; return c; }
    Node* erase(Node* n) { Node* next = n->next; n->prev->next = next; next->prev = n->prev; std::free(n); return next; }
    void pop_back() { erase(head.prev); }
};
struct EmitterList {
    List emitters;
    bool InsertSorted(List& visible, Emitter* e, long) { visible.push_back(e); return true; }
    void FindVisible(List& visible, long maxCount, const Info& info)
    {
        float alwaysUpdateDistance = info.alwaysUpdateDistance;
        float distance = info.distance;
        Node* it = emitters.head.next;
        while (it != &emitters.head) {
            Emitter* emitter = it->data;
            if (emitter->dead) {
                it = emitters.erase(it);
            } else if (emitter->initialised) {
                bool isVisible;
                if (emitter->alwaysUpdate) isVisible = emitter->IsVisible(alwaysUpdateDistance, &info);
                else isVisible = emitter->IsVisible(distance, &info);
                if (isVisible && InsertSorted(visible, emitter, maxCount)) {
                    while (visible.size() > (unsigned long)maxCount) {
                        Emitter* last = visible.head.prev->data;
                        last->ClearAllDecals(info.engine);
                        last->ClearAllParticles(info.engine);
                        visible.pop_back();
                    }
                    it = it->next;
                } else {
                    Emitter* hidden = emitter;
                    if (!hidden->continuous && hidden->dieOffscreen) {
                        hidden->Kill();
                        it = emitters.erase(it);
                    } else {
                        hidden->ClearAllDecals(info.engine);
                        hidden->ClearAllParticles(info.engine);
                        it = it->next;
                    }
                }
            } else {
                emitter->Initialise(info.engine);
                it = it->next;
            }
        }
    }
};
int main()
{
    Emitter e[6];
    for (int i = 0; i < 6; ++i) { Emitter z = { i, false, true, false, false, false, 5.0f, 0, 0, 0, 0.0f }; e[i] = z; }
    e[0].dead = true;                                   // erased
    e[1].initialised = false;                           // initialised, kept
    e[2].range = 50.0f; e[2].alwaysUpdate = true;       // visible through the always-update distance
    e[3].range = 50.0f; e[3].dieOffscreen = true;       // invisible + dies offscreen: killed and erased
    e[4].range = 50.0f;                                 // invisible: cleared, kept
    e[5].range = 1.0f;                                  // visible, but trimmed off the list end
    EmitterList list; for (int i = 0; i < 6; ++i) list.emitters.push_back(&e[i]);
    List visible;
    Info info = { 100.0f, 10.0f, 7 };
    list.FindVisible(visible, 1, info);
    int bad = 0;
    bad |= list.emitters.size() != 4;
    bad |= e[1].inits != 1 || e[3].kills != 1 || e[4].clears != 2 || e[5].clears != 2;
    bad |= e[2].lastDistance != 100.0f || e[5].lastDistance != 10.0f;
    bad |= visible.size() != 1 || visible.head.next->data != &e[2];
    if (bad) { std::printf("00aeb2d0_TEST FAIL\n"); return 1; }
    std::printf("00aeb2d0_TEST PASS\n");
    return 0;
}
