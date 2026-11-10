// Behaviour test for 0x005B3AEF CTCInventoryClothing::OnInitialActivate: a save-loaded inventory only runs the base activation;
// otherwise each of the creature def's initial appearance modifiers that is a valid object def with a clothing appearance modifier
// (type 2) is created at the owner's position for the owner's player, flagged as a default item, added to this inventory
// (true, true, 0, true, false) and, when alive, marked as created by its owner; a created thing without an inventory-item component
// is killed; then the base activation runs.
#include <cstdio>
#include <vector>
#include <map>
#include <string>

struct Thing {
    long defIndex; float pos[3]; long player; bool hasItem; bool defaultItem; bool dead; bool createdByOwner; bool killed;
    int addedTo; bool addArgs[4]; long addLong;
};
static std::vector<Thing*> gThings;
struct ModifierDef { bool isModifier; long appearanceType; };
struct Clothing {
    bool LoadedFromSaveGame;
    bool hasCreatureDef;
    std::vector<long> initialModifiers;            // CCreatureDef::InitialAppearanceModifiers
    std::map<long, ModifierDef> defs;              // valid object defs (index > 0) and their appearance sub-def
    std::map<long, bool> createsWithItem;          // does the created thing have a CTCInventoryItem
    std::map<long, bool> createsDead;
    float ownerPos[3]; long ownerPlayer;
    int baseActivations;
    std::vector<std::string> names;

    Thing* Create(long defIndex, const float* pos, long player, const std::string& name)
    {
        Thing* t = new Thing();
        t->defIndex = defIndex; t->pos[0] = pos[0]; t->pos[1] = pos[1]; t->pos[2] = pos[2]; t->player = player;
        t->hasItem = createsWithItem[defIndex]; t->dead = createsDead[defIndex];
        names.push_back(name);
        gThings.push_back(t);
        return t;
    }
    void OnInitialActivate()
    {
        if (!LoadedFromSaveGame) {
            if (hasCreatureDef) {
                for (unsigned long i = 0; i < initialModifiers.size(); ++i) {
                    long defIndex = initialModifiers[i];
                    std::map<long, ModifierDef>::iterator d = defs.find(defIndex);
                    if (defIndex > 0 && d != defs.end()) {
                        if (d->second.isModifier && d->second.appearanceType == 2) {
                            Thing* thing;
                            {
                                std::string name("");
                                thing = Create(defIndex, ownerPos, ownerPlayer, name);
                            }
                            if (thing->hasItem) {
                                thing->defaultItem = true;
                                thing->addedTo = 1;
                                thing->addArgs[0] = true; thing->addArgs[1] = true; thing->addLong = 0;
                                thing->addArgs[2] = true; thing->addArgs[3] = false;
                                if (!thing->dead)
                                    thing->createdByOwner = true;
                            } else {
                                thing->killed = true;
                            }
                        }
                    }
                }
            }
        }
        ++baseActivations;
    }
};

int main()
{
    Clothing c;
    c.LoadedFromSaveGame = false; c.hasCreatureDef = true; c.baseActivations = 0;
    c.ownerPos[0] = 1.0f; c.ownerPos[1] = 2.0f; c.ownerPos[2] = 3.0f; c.ownerPlayer = -1;
    ModifierDef clothing = { true, 2 }, tattoo = { true, 1 }, plain = { false, 2 };
    c.defs[10] = clothing; c.defs[11] = clothing; c.defs[12] = tattoo; c.defs[13] = plain; c.defs[14] = clothing;
    c.createsWithItem[10] = true; c.createsWithItem[11] = false; c.createsWithItem[14] = true; c.createsDead[14] = true;
    c.initialModifiers.push_back(10); c.initialModifiers.push_back(11); c.initialModifiers.push_back(12);
    c.initialModifiers.push_back(13); c.initialModifiers.push_back(0); c.initialModifiers.push_back(99); c.initialModifiers.push_back(14);
    c.OnInitialActivate();
    bool ok = gThings.size() == 3 && c.baseActivations == 1;
    ok = ok && gThings[0]->defIndex == 10 && gThings[0]->player == -1 && gThings[0]->pos[2] == 3.0f && gThings[0]->defaultItem
            && gThings[0]->addedTo == 1 && gThings[0]->addArgs[0] && !gThings[0]->addArgs[3] && gThings[0]->createdByOwner && !gThings[0]->killed;
    ok = ok && gThings[1]->defIndex == 11 && gThings[1]->killed && !gThings[1]->defaultItem && gThings[1]->addedTo == 0;
    ok = ok && gThings[2]->defIndex == 14 && gThings[2]->defaultItem && !gThings[2]->createdByOwner && c.names[0] == "";
    Clothing loaded = c;
    loaded.LoadedFromSaveGame = true; loaded.baseActivations = 0;
    loaded.OnInitialActivate();
    ok = ok && gThings.size() == 3 && loaded.baseActivations == 1;
    Clothing noCreature = c;
    noCreature.hasCreatureDef = false; noCreature.baseActivations = 0;
    noCreature.OnInitialActivate();
    ok = ok && gThings.size() == 3 && noCreature.baseActivations == 1;
    for (unsigned int i = 0; i < gThings.size(); ++i) delete gThings[i];
    if (!ok) { std::printf("005b3aef_TEST FAIL\n"); return 1; }
    std::printf("005b3aef_TEST PASS\n");
    return 0;
}