// Behaviour test for 0x007447A0 CTCPerceiveThing::SetThingToPerceive(const CThing*)
#include <cstdio>

class CThing { public: int id; };

static int g_gets = 0, g_sets = 0;
static const CThing* g_lastSet = (const CThing*)1;
class CIntelligentPointer {
public:
    const void* item;
    const void* GetPItem() const { ++g_gets; return item; }       // stand-in for 0x00A01B50
};

class CTCPerceiveThing {
public:
    CIntelligentPointer PerceivedThing;
    bool Perceiving;
    const CThing* GetPerceivedThing() const { return (const CThing*)PerceivedThing.GetPItem(); }
    void SetPerceivedThing(const CThing* thing) { ++g_sets; g_lastSet = thing; PerceivedThing.item = thing; }   // stand-in for 0x007446B0
    void SetThingToPerceive(const CThing* thing)
    {
        if (thing) {
            GetPerceivedThing();
            Perceiving = true;
            SetPerceivedThing(thing);
        }
        else {
            Perceiving = false;
            SetPerceivedThing(0);
        }
    }
};

int main()
{
    CThing a, b;
    CTCPerceiveThing p;
    p.PerceivedThing.item = 0;
    p.Perceiving = false;
    int ok = 1;
    p.SetThingToPerceive(&a);
    if (!p.Perceiving || g_lastSet != &a || p.PerceivedThing.item != &a || g_sets != 1 || g_gets != 1) ok = 0;
    p.SetThingToPerceive(&b);
    if (!p.Perceiving || g_lastSet != &b || g_sets != 2 || g_gets != 2) ok = 0;
    p.SetThingToPerceive(0);
    if (p.Perceiving || g_lastSet != 0 || p.PerceivedThing.item != 0 || g_sets != 3 || g_gets != 2) ok = 0;
    if (!ok) { std::printf("007447a0_TEST FAIL\n"); return 1; }
    std::printf("007447a0_TEST PASS\n");
    return 0;
}