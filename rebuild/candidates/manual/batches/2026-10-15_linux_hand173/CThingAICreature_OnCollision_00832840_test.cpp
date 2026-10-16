// behaviour test for CThingAICreature::OnCollision (0x00832840)
#include <cstdio>
static int g_base = 0, g_aware = 0, g_world = 0;
struct CThing;
struct CTCPerceiveThing { CThing* perceived; CThing* GetPerceivedThing() { return perceived; } void SetAsDirectlyAwareOfPerceivedThing() { ++g_aware; } };
struct CThing { bool Dead; bool hasPerceive; CTCPerceiveThing* perceive; void PeekWorld() { ++g_world; } };
struct CThingAICreature : CThing {
    void BaseOnCollision(int, CThing*) { ++g_base; }
    void OnCollision(int type, CThing* other)
    {
        BaseOnCollision(type, other);
        if (!Dead && other && !other->Dead) {
            if (hasPerceive) { CTCPerceiveThing* p = perceive; if (p->GetPerceivedThing() == other) p->SetAsDirectlyAwareOfPerceivedThing(); }
            if (type == 1) PeekWorld();
        }
    }
};
int main()
{
    CThing other; other.Dead = false; CTCPerceiveThing p; p.perceived = &other;
    CThingAICreature c; c.Dead = false; c.hasPerceive = true; c.perceive = &p;
    c.OnCollision(1, &other);
    int bad = g_base != 1 || g_aware != 1 || g_world != 1;
    c.OnCollision(0, 0); if (g_base != 2 || g_aware != 1 || g_world != 1) bad = 1;
    other.Dead = true; c.OnCollision(1, &other); if (g_aware != 1 || g_world != 1) bad = 1;
    other.Dead = false; p.perceived = 0; c.OnCollision(2, &other); if (g_aware != 1 || g_world != 1) bad = 1;
    c.Dead = true; c.OnCollision(1, &other); if (g_base != 5 || g_world != 1) bad = 1;
    if (bad) { std::printf("00832840_TEST FAIL\n"); return 1; }
    std::printf("00832840_TEST PASS\n");
    return 0;
}
