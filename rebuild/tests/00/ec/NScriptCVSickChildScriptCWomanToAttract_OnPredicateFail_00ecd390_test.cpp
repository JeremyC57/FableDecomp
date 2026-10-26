// Behaviour test for 0x00ECD390 CWomanToAttract::OnPredicateFail: reward + morality + quest flag only when she was
// killed and had not yet handed over her object.
#include <cstdio>
#include <cstring>
class CCharString { public: char text[32]; CCharString(const char* t, long length = -1) { std::strcpy(text, t); (void)length; } };
class CDefString { public: const char* name; operator CCharString() const { return CCharString(name); } };
class CScriptDef { public: CDefString IngredientObject; float KilledSomeoneForMushroom; };
static CScriptDef g_def = { { "OBJECT_MUSHROOM" }, -5.0f };
static CScriptDef* g_pScriptDef = &g_def;
class CScriptThing { public: bool killed; char asked[32]; bool MsgIsKilledBy(const CCharString& k) { std::strcpy(asked, k.text); return killed; } };
class CGameScriptInterface { public:
    int gives; char object[32]; long count; bool show; float morality;
    void GiveHeroObject(const CCharString& o, long c, bool s) { ++gives; std::strcpy(object, o.text); count = c; show = s; }
    void GiveHeroMorality(float m) { morality = m; }
};
struct CV_SickChildScript { bool LaughingWomanKilled; };
struct CWomanToAttract {
    CGameScriptInterface* Game; CScriptThing Me; CV_SickChildScript* ParentClass; bool GivenObject;
    void OnPredicateFail()
    {
        bool killed = !GivenObject && Me.MsgIsKilledBy("");
        if (killed) {
            Game->GiveHeroObject(g_pScriptDef->IngredientObject, -1, false);
            Game->GiveHeroMorality(g_pScriptDef->KilledSomeoneForMushroom);
            ParentClass->LaughingWomanKilled = true;
        }
    }
};
int main()
{
    CGameScriptInterface g; g.gives = 0; g.morality = 0.0f;
    CV_SickChildScript q; q.LaughingWomanKilled = false;
    CWomanToAttract w; w.Game = &g; w.ParentClass = &q; w.GivenObject = false; w.Me.killed = false; w.Me.asked[0] = 'x';
    w.OnPredicateFail();
    bool ok = g.gives == 0 && !q.LaughingWomanKilled && w.Me.asked[0] == 0;
    w.Me.killed = true; w.GivenObject = true; w.Me.asked[0] = 'x';
    w.OnPredicateFail();
    ok = ok && g.gives == 0 && w.Me.asked[0] == 'x';
    w.GivenObject = false;
    w.OnPredicateFail();
    float expect = -5.0f;
    ok = ok && g.gives == 1 && std::strcmp(g.object, "OBJECT_MUSHROOM") == 0 && g.count == -1 && !g.show
         && g.morality == expect && q.LaughingWomanKilled;
    if (!ok) { std::printf("00ecd390_TEST FAIL\n"); return 1; }
    std::printf("00ecd390_TEST PASS\n");
    return 0;
}