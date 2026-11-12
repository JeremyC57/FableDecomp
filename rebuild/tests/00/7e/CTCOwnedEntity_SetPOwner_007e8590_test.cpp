// Behaviour test for CTCOwnedEntity::SetPOwner (retail 0x007E8590): a copy of the row's logic over small models of the thing,
// its thing-owner component and the owned-entity component.
#include <cstdio>
#include <cstring>
#include <string.h>

struct Thing;
struct ThingOwner { int added; int removed; ThingOwner() : added(0), removed(0) {} };
struct Thing {
    unsigned __int64 uid; bool dead; bool inLimbo; bool drawable; int type; const char* scriptData;
    bool hasThingOwner, hasNav, isBuyableHouse; ThingOwner owner; int addedThingOwnerTC; int messages; int navAdds, navRemoves;
    bool limboSet; bool drawSet;
    Thing() : uid(0), dead(false), inLimbo(false), drawable(false), type(0), scriptData(""), hasThingOwner(false), hasNav(false),
              isBuyableHouse(false), addedThingOwnerTC(0), messages(0), navAdds(0), navRemoves(0), limboSet(false), drawSet(false) {}
};
struct OwnedEntity {
    Thing* parent; bool navAdded; Thing* pOwner; unsigned __int64 ownerUID;
    void SetPOwner(Thing* owner)
    {
        if (pOwner) {
            pOwner->owner.removed++;
        }
        parent->messages++;
        if (navAdded) { parent->hasNav = false; parent->navRemoves++; navAdded = false; }
        pOwner = owner;
        if (pOwner && !pOwner->dead) {
            ownerUID = pOwner->uid;
            if (!pOwner->hasThingOwner) { pOwner->hasThingOwner = true; pOwner->addedThingOwnerTC++; }
            pOwner->owner.added++;
            parent->drawSet = pOwner->drawable;
            parent->limboSet = pOwner->inLimbo;
            if (!parent->hasNav && pOwner->isBuyableHouse && parent->type == 5 &&
                _strnicmp(parent->scriptData, "Dress", strlen("Dress")) == 0) {
                parent->hasNav = true; parent->navAdds++; navAdded = true;
            }
        }
    }
};

int main()
{
    Thing house, item, other;
    house.uid = 0x1122334455667788ULL; house.isBuyableHouse = true; house.drawable = true; house.inLimbo = true;
    item.type = 5; item.scriptData = "dress_lvl1";
    OwnedEntity e = { &item, false, 0, 0 };
    e.SetPOwner(&house);
    if (e.ownerUID != house.uid || house.addedThingOwnerTC != 1 || house.owner.added != 1 || !e.navAdded || item.navAdds != 1 ||
        !item.drawSet || !item.limboSet || item.messages != 1) { std::printf("007e8590_TEST FAIL\n"); return 1; }
    other.dead = true;
    e.SetPOwner(&other);
    if (house.owner.removed != 1 || e.navAdded || item.navRemoves != 1 || other.owner.added != 0 || e.ownerUID != house.uid) {
        std::printf("007e8590_TEST FAIL\n"); return 1;
    }
    Thing plain; item.scriptData = "Chair";
    e.pOwner = 0; e.SetPOwner(&plain);
    if (e.navAdded || plain.addedThingOwnerTC != 1 || plain.owner.added != 1) { std::printf("007e8590_TEST FAIL\n"); return 1; }
    std::printf("007e8590_TEST PASS\n");
    return 0;
}