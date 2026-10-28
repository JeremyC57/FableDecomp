extern "C" int printf(const char*, ...);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL line %d\n", __LINE__); } } while (0)
// Behaviour test for 0x00890990 CGameScriptInterface::EntityGetOpinionAttitudeToPlayer: the opinion reaction manager's
// attitude of the entity's thing; 0 when the script thing has no thing.
class CThing { public: long attitude; };
class COpinionReactionManager { public: int calls; long GetThingAttitudeToPlayer(const CThing& t) { ++calls; return t.attitude; } };
static long EntityGetOpinionAttitudeToPlayer(COpinionReactionManager& m, CThing* t) { if (t) return m.GetThingAttitudeToPlayer(*t); return 0; }
int main()
{
    COpinionReactionManager m; m.calls = 0; CThing t = { 3 };
    CHECK(EntityGetOpinionAttitudeToPlayer(m, &t) == 3 && m.calls == 1);
    CHECK(EntityGetOpinionAttitudeToPlayer(m, 0) == 0 && m.calls == 1);
    if (fails == 0) printf("00890990_TEST PASS\n"); else printf("00890990_TEST FAIL\n");
    return fails;
}