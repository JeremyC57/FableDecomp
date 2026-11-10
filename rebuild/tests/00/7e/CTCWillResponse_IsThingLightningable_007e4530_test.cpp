// Behaviour test for CTCWillResponse::IsThingLightningable (0x007E4530): the will-response component decides when present (false
// when not bloody-willable at all, else its lightningable bit); otherwise a smashable thing, a targeted thing whose targeting flags
// have 0x08 or 0x10, or a thing with type flag 8 can be hit.
#include <cstdio>

class CTCWillResponse { public: bool IsLightningable, IsNotBloodyWillable; };
class CTCTargeted { public: unsigned long TargetingFlags; };
class CThing { public:
    const CTCWillResponse* will; bool smashable; const CTCTargeted* targeted; unsigned long typeFlags;
};
static bool IsThingLightningable(const CThing* thing)
{
    if (thing->will) {
        if (thing->will->IsNotBloodyWillable) return false;
        return thing->will->IsLightningable;
    }
    if (thing->smashable) return true;
    if (thing->targeted) {
        if (thing->targeted->TargetingFlags & 0x18) return true;
    }
    if (thing->typeFlags & 8) return true;
    return false;
}
static CThing Make(const CTCWillResponse* w, bool s, const CTCTargeted* t, unsigned long f)
{
    CThing c; c.will = w; c.smashable = s; c.targeted = t; c.typeFlags = f; return c;
}
int main()
{
    CTCWillResponse yes = { true, false }, no = { false, false }, blocked = { true, true };
    CTCTargeted t8 = { 0x08 }, t4 = { 0x04 }, t10 = { 0x10 };
    bool ok = true;
    CThing c;
    c = Make(&yes, true, &t8, 8);      if (!IsThingLightningable(&c)) ok = false;
    c = Make(&no, true, &t8, 8);       if (IsThingLightningable(&c)) ok = false;    // the component wins
    c = Make(&blocked, false, 0, 0);   if (IsThingLightningable(&c)) ok = false;
    c = Make(0, true, 0, 0);           if (!IsThingLightningable(&c)) ok = false;
    c = Make(0, false, &t10, 0);       if (!IsThingLightningable(&c)) ok = false;
    c = Make(0, false, &t4, 0);        if (IsThingLightningable(&c)) ok = false;
    c = Make(0, false, &t4, 8);        if (!IsThingLightningable(&c)) ok = false;
    c = Make(0, false, 0, 0x37);       if (IsThingLightningable(&c)) ok = false;
    if (!ok) { std::printf("007e4530_TEST FAIL\n"); return 1; }
    std::printf("007e4530_TEST PASS\n");
    return 0;
}