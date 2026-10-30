// Behaviour test for NScript::CV_TalentlessBardScript::IsHeroDressedAsAWoman 0x00EDA5E0
#include <cstdio>
#include <cstring>
class CScriptThing { public: int id; };
class CGame { public:
    CScriptThing Hero;
    const char* Worn[8];
    int nWorn;
    int Calls;
    CScriptThing& GetHero() { return Hero; }
    bool IsWearingClothingItem(const CScriptThing& t, const char* item)
    {
        ++Calls;
        if (&t != &Hero) return false;
        for (int i = 0; i < nWorn; ++i)
            if (std::strcmp(Worn[i], item) == 0) return true;
        return false;
    }
};
struct CV_TalentlessBardScript {
    CGame* Game;
    bool IsHeroDressedAsAWoman()
    {
        bool plain = Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_SHIRT_DRESS")
            && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_GLOVES_DRESS")
            && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_TROUSERS_DRESS");
        bool good = Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_SHIRT_DRESS_GOOD")
            && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_GLOVES_DRESS_GOOD")
            && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_TROUSERS_DRESS_GOOD");
        bool evil = Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_SHIRT_DRESS_EVIL")
            && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_GLOVES_DRESS_EVIL")
            && Game->IsWearingClothingItem(Game->GetHero(), "OBJECT_HERO_TROUSERS_DRESS_EVIL");
        return plain || good || evil;
    }
};
static bool Dressed(const char* a, const char* b, const char* c, int* calls)
{
    CGame g;
    g.Hero.id = 1; g.nWorn = 0; g.Calls = 0;
    if (a) g.Worn[g.nWorn++] = a;
    if (b) g.Worn[g.nWorn++] = b;
    if (c) g.Worn[g.nWorn++] = c;
    CV_TalentlessBardScript s;
    s.Game = &g;
    bool r = s.IsHeroDressedAsAWoman();
    if (calls) *calls = g.Calls;
    return r;
}
int main()
{
    int calls = 0;
    if (!Dressed("OBJECT_HERO_SHIRT_DRESS", "OBJECT_HERO_GLOVES_DRESS", "OBJECT_HERO_TROUSERS_DRESS", &calls) || calls != 5) {
        std::printf("00eda5e0_TEST FAIL\n"); return 1;
    }
    if (!Dressed("OBJECT_HERO_SHIRT_DRESS_EVIL", "OBJECT_HERO_GLOVES_DRESS_EVIL", "OBJECT_HERO_TROUSERS_DRESS_EVIL", 0)
        || !Dressed("OBJECT_HERO_SHIRT_DRESS_GOOD", "OBJECT_HERO_GLOVES_DRESS_GOOD", "OBJECT_HERO_TROUSERS_DRESS_GOOD", 0)) {
        std::printf("00eda5e0_TEST FAIL\n"); return 1;
    }
    if (Dressed("OBJECT_HERO_SHIRT_DRESS", "OBJECT_HERO_GLOVES_DRESS_GOOD", "OBJECT_HERO_TROUSERS_DRESS_EVIL", 0)
        || Dressed(0, 0, 0, &calls) || calls != 3) {
        std::printf("00eda5e0_TEST FAIL\n"); return 1;
    }
    std::printf("00eda5e0_TEST PASS\n");
    return 0;
}