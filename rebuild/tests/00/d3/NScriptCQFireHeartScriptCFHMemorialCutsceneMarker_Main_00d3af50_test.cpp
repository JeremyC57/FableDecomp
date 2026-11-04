// Behaviour test for 0x00d3af50 CFH_MemorialCutsceneMarker::Main
#include <cstdio>
#include <cstring>
class CScriptThing { public: int id; };
class CCharString { public: char m_text[32]; CCharString(const char* t, long) { std::strcpy(m_text, t); } };
class CGame { public:
    float age; int persistent, removed;
    float GetHeroAge() const { return age; }
    void SetHeroAge(float a) { age = a; }
    void SetThingPersistent(const CScriptThing&, bool p) { if (p) ++persistent; }
    void RemoveThing(const CScriptThing&, bool a, bool b) { if (!a && b) ++removed; }
};
struct CQ_FireHeartScript { char played[32]; void PlayCutscene(CCharString name) { std::strcpy(played, name.m_text); } };
struct CFH_MemorialCutsceneMarker {
    CGame* Game; CScriptThing Me; CQ_FireHeartScript* ParentClass;
    void Main()
    {
        Game->SetHeroAge(Game->GetHeroAge() + 1.0f);
        ParentClass->PlayCutscene(CCharString("CS_FIREHEART_PREINTRO", -1));
        Game->SetThingPersistent(Me, true);
        Game->RemoveThing(Me, false, true);
    }
};
int main()
{
    CGame g = { 18.0f, 0, 0 }; CQ_FireHeartScript q; q.played[0] = 0;
    CFH_MemorialCutsceneMarker m; m.Game = &g; m.Me.id = 3; m.ParentClass = &q; m.Main();
    bool ok = g.age == 19.0f && std::strcmp(q.played, "CS_FIREHEART_PREINTRO") == 0 && g.persistent == 1 && g.removed == 1;
    if (!ok) { std::printf("00d3af50_TEST FAIL\n"); return 1; }
    std::printf("00d3af50_TEST PASS\n");
    return 0;
}