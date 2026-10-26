// Behaviour test for CSFindThing (0x00CBF9DE): "HERO" is the hero; a name in the cutscene actor map whose resource
// object is scripting yields that object's thing; anything else is looked up by script name.
#include <cstdio>
#include <cstring>

class CCharString { public:
    const char* text;                           // 0 = the empty (null-data) string
    bool operator==(const char* other) const
    {
        if (text == 0)
            return std::strcmp(other, "") == 0;
        return std::strcmp(text, other) == 0;
    }
};
class CScriptThing { public: int id; };
class CScriptGameResourceObjectScriptedThingBase { public:
    int thingId;
    bool scripting;
    bool IsScripting() const { return scripting; }
    CScriptThing GetScriptThing() const { CScriptThing t; t.id = thingId; return t; }
};
struct CActorEntry {
    const char* first;
    CScriptGameResourceObjectScriptedThingBase second;
};
struct CActorMap {                              // find() returns the entry or end()
    CActorEntry entries[4];
    int count;
    CActorEntry* find(const CCharString& key)
    {
        for (int i = 0; i < count; ++i)
            if (key.text && std::strcmp(entries[i].first, key.text) == 0) return &entries[i];
        return end();
    }
    CActorEntry* end() { return &entries[count]; }
};
class CGameScriptInterface { public:
    CScriptThing hero;
    int byNameQueries;
    CScriptThing& GetHero() { return hero; }
    CScriptThing GetThingWithScriptName(const CCharString& name) { ++byNameQueries; CScriptThing t; t.id = 100 + (int)std::strlen(name.text); return t; }
};
CGameScriptInterface* g_pGameScriptInterface = 0;

static bool IsScriptingActor(const CScriptGameResourceObjectScriptedThingBase* actor)
{
    return actor && actor->IsScripting();
}

CScriptThing CSFindThing(const CCharString& name, CActorMap& actors)
{
    CScriptThing thing;
    thing.id = 0;
    if (name == "HERO") {
        thing = g_pGameScriptInterface->GetHero();
    } else {
        CActorEntry* it = actors.find(name);
        if (it != actors.end() && it->second.IsScripting() && IsScriptingActor(&it->second))
            thing = it->second.GetScriptThing();
        else
            thing = g_pGameScriptInterface->GetThingWithScriptName(name);
    }
    return thing;
}

int main()
{
    CGameScriptInterface gsi;
    gsi.hero.id = 1; gsi.byNameQueries = 0;
    g_pGameScriptInterface = &gsi;
    CActorMap actors;
    actors.count = 2;
    actors.entries[0].first = "Briar"; actors.entries[0].second.thingId = 20; actors.entries[0].second.scripting = true;
    actors.entries[1].first = "Maze";  actors.entries[1].second.thingId = 30; actors.entries[1].second.scripting = false;
    CCharString hero; hero.text = "HERO";
    CCharString briar; briar.text = "Briar";
    CCharString maze; maze.text = "Maze";
    CCharString other; other.text = "Guard";
    int a = CSFindThing(hero, actors).id;
    int b = CSFindThing(briar, actors).id;
    int c = CSFindThing(maze, actors).id;
    int d = CSFindThing(other, actors).id;
    if (a != 1 || b != 20 || c != 104 || d != 105 || gsi.byNameQueries != 2) {
        std::printf("00cbf9de_TEST FAIL\n");
        return 1;
    }
    std::printf("00cbf9de_TEST PASS\n");
    return 0;
}