// Behaviour test for CThingPhysical::WorldUpdate (0x006A6880): each health-over-time helper adds HealthLeftToAdd / frames
// left, finished helpers (frames <= 0) are unlinked and freed, then the base CThing::WorldUpdate result is returned.
#include <cstdio>
#include <cstdlib>
static float g_health = 0.0f;
static int g_baseCalls = 0;
static int g_freed = 0;
struct Helper { long FramesLeftForAddingHealth; float HealthLeftToAdd; };
struct Node { Node* next; Node* prev; Helper data; };
class CThing { public: bool WorldUpdateBase() { ++g_baseCalls; return true; } };
class CThingPhysical : public CThing {
public:
    Node* head;
    void InternalModifyHealth(float amount, bool) { g_health += amount; }
    void Update(Helper& h)
    {
        float health = h.HealthLeftToAdd / h.FramesLeftForAddingHealth;
        InternalModifyHealth(health, true);
        h.HealthLeftToAdd -= health;
        --h.FramesLeftForAddingHealth;
    }
    bool WorldUpdate()
    {
        Node* it = head->next;
        while (it != head) {
            Update(it->data);
            if (it->data.FramesLeftForAddingHealth <= 0) {
                Node* next = it->next;
                it->prev->next = next;
                next->prev = it->prev;
                std::free(it);
                ++g_freed;
                it = next;
            } else
                it = it->next;
        }
        return WorldUpdateBase();
    }
};
static void PushBack(Node* head, long frames, float health)
{
    Node* n = (Node*)std::malloc(sizeof(Node));
    n->data.FramesLeftForAddingHealth = frames;
    n->data.HealthLeftToAdd = health;
    n->next = head;
    n->prev = head->prev;
    head->prev->next = n;
    head->prev = n;
}
static int Count(Node* head) { int c = 0; for (Node* n = head->next; n != head; n = n->next) ++c; return c; }
static bool Near(float a, float b) { return a - b < 0.001f && b - a < 0.001f; }
int main()
{
    Node header;
    header.next = header.prev = &header;
    CThingPhysical thing;
    thing.head = &header;
    PushBack(&header, 1, 10.0f);     // finishes this frame
    PushBack(&header, 4, 8.0f);      // 2 per frame
    PushBack(&header, 2, -6.0f);     // -3 per frame
    bool r = thing.WorldUpdate();
    if (!r || g_baseCalls != 1 || !Near(g_health, 10.0f + 2.0f - 3.0f) || Count(&header) != 2 || g_freed != 1) { std::printf("006a6880_TEST FAIL\n"); return 1; }
    thing.WorldUpdate();
    if (!Near(g_health, 9.0f + 2.0f - 3.0f) || Count(&header) != 1 || g_freed != 2) { std::printf("006a6880_TEST FAIL\n"); return 1; }
    thing.WorldUpdate();
    thing.WorldUpdate();
    if (!Near(g_health, 8.0f + 4.0f) || Count(&header) != 0 || g_freed != 3 || g_baseCalls != 4) { std::printf("006a6880_TEST FAIL\n"); return 1; }
    std::printf("006a6880_TEST PASS\n");
    return 0;
}