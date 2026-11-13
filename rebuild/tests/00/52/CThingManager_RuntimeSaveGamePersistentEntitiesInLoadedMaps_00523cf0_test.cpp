// Behaviour test for CThingManager::RuntimeSaveGamePersistentEntitiesInLoadedMaps (0x00523CF0): a copy of the row's logic over a
// small model (maps with loaded flags, a class list of things, a hero, the runtime persistence slots per map).
#include <cstdio>
#include <vector>
#include <list>
#include <string>

struct Thing { bool persisted; bool dead; long map; int id; bool IsGameStatePersisted() const { return persisted; } };
struct Slot { std::string script; std::vector<int> data; long uncompressed; bool valid; Slot() : uncompressed(-1), valid(false) {} };
struct Model {
    std::vector<bool> loaded;          // index = map
    std::list<Thing*> classList;
    Thing* hero;
    bool heroState;
    bool serialising;
    std::vector<Slot> slots;
    long GetNoMaps() const { return (long)loaded.size(); }
    void SetGamePersistentSaveData(long map, const std::vector<int>& data) {
        unsigned long needed = map + 1;
        slots.resize(needed > slots.size() ? needed : slots.size());
        slots[map].uncompressed = (long)data.size();
        slots[map].data = data;
        slots[map].valid = true;
    }
    void SetGamePersistentScript(long map, const std::string& s) {
        unsigned long needed = map + 1;
        slots.resize(needed > slots.size() ? needed : slots.size());
        slots[map].script = s;
        slots[map].valid = true;
    }
    void Run(bool binary) {
        for (long map = 1; map < GetNoMaps(); ++map) {
            if (!loaded[map]) continue;
            std::vector<Thing*> things;
            things.clear();
            if (heroState) {
                if (hero && !hero->dead && hero->map == map) things.push_back(hero);
            } else {
                for (std::list<Thing*>::iterator it = classList.begin(); it != classList.end(); ++it) {
                    Thing* t = *it;
                    if (t->IsGameStatePersisted() && t && !t->dead && t->map == map) things.push_back(t);
                }
            }
            serialising = true;
            if (binary) {
                std::vector<int> stream;
                for (size_t i = 0; i < things.size(); ++i) stream.push_back(things[i]->id);
                SetGamePersistentSaveData(map, stream);
                SetGamePersistentScript(map, "");
            } else {
                std::string text;
                for (size_t i = 0; i < things.size(); ++i) { char b[16]; std::sprintf(b, "T%d;", things[i]->id); text += b; }
                SetGamePersistentScript(map, text);
                SetGamePersistentSaveData(map, std::vector<int>());
            }
            serialising = false;
        }
    }
};

int main()
{
    Thing a = { true, false, 1, 10 }, b = { true, true, 1, 11 }, c = { false, false, 1, 12 }, d = { true, false, 2, 13 }, e = { true, false, 3, 14 };
    Thing h = { true, false, 2, 99 };
    Model m;
    m.loaded.push_back(true); m.loaded.push_back(true); m.loaded.push_back(true); m.loaded.push_back(false);
    m.classList.push_back(&a); m.classList.push_back(&b); m.classList.push_back(&c); m.classList.push_back(&d); m.classList.push_back(&e);
    m.hero = &h; m.heroState = false; m.serialising = false;
    m.Run(false);
    bool ok = m.slots.size() == 3 && m.slots[1].script == "T10;" && m.slots[2].script == "T13;" && m.slots[1].uncompressed == 0
           && m.slots[1].valid && !m.slots[0].valid && !m.serialising;
    Model n = m; n.slots.clear(); n.heroState = true;
    n.Run(true);
    ok = ok && n.slots.size() == 3 && n.slots[1].data.empty() && n.slots[2].data.size() == 1 && n.slots[2].data[0] == 99
            && n.slots[2].script == "" && n.slots[2].uncompressed == 1;
    h.dead = true; Model o = n; o.slots.clear(); o.Run(true);
    ok = ok && o.slots.size() == 3 && o.slots[2].data.empty();
    if (!ok) { std::printf("00523cf0_TEST FAIL\n"); return 1; }
    std::printf("00523cf0_TEST PASS\n");
    return 0;
}