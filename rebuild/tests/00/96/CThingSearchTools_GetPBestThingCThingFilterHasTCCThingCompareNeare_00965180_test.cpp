// behaviour test for 0x00965180 CThingSearchTools::GetPBestThing<CThingFilter_HasTC, ...> (model over std::list; filter and compare are stubs)
#include <cstdio>
#include <list>
class CThing { public: unsigned long ThingTypeFlags; unsigned char StateBits; int dist; bool pass; };
static bool Included(const CThing* t) { unsigned char f = t->StateBits; return (f & 4) && (f & 8) && !(f & 1) && !(f & 0x20); }
static const unsigned long MASK = 0x8007;
static CThing* Best(const std::list<CThing*>& things, CThing** out)
{
    CThing* best = 0;
    for (std::list<CThing*>::const_iterator it = things.begin(); it != things.end(); ++it)
        if (Included(*it) && ((*it)->ThingTypeFlags & MASK) == MASK) {
            CThing* c = *it;
            if (c && c->pass) best = (!best || c->dist < best->dist) ? c : best;
        }
    *out = best;
    return best;
}
int main()
{
    CThing a = { MASK, 0x0C, 5, true }, b = { MASK, 0x0C, 2, true }, dead = { MASK, 0x0D, 1, true }, wrong = { 0, 0x0C, 0, true }, fail = { MASK, 0x0C, 1, false };
    std::list<CThing*> l; l.push_back(&a); l.push_back(&dead); l.push_back(&b); l.push_back(&wrong); l.push_back(&fail);
    CThing* out = 0;
    int ok = Best(l, &out) == &b && out == &b;
    std::list<CThing*> e; ok = ok && Best(e, &out) == 0 && out == 0;
    if (!ok) { std::printf("00965180_TEST FAIL\n"); return 1; }
    std::printf("00965180_TEST PASS\n");
    return 0;
}