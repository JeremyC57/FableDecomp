#include <cstdio>
// Behaviour of CTCHeroOpinionDeedLog::PlayerTakenOutWeapon (0x006C6E20): an old weapon-out deed is removed from the
// three registries and forgotten; when weapon-out crimes are enabled a new deed (type 1, hero position, post type 2)
// is posted and its handle remembered.
struct C3DVector { float x, y, z; };
struct CThingAICreature; struct CFaction;
static long g_removed[3]; static int g_removeCount = 0; static int g_posts = 0; static const C3DVector* g_postPos = 0; static bool g_ok = true;
struct CThing {
    C3DVector Position;
    const C3DVector& GetPosition() { return Position; }
};
struct CTCHeroOpinionDeedLog_Retail {
    CThing* ParentThing;
    long WeaponOutDeedHandle;
    bool WeaponOutCrimeEnabled;
    void RemoveDeedStillSearchingForWitnesses(long handle) { g_removed[g_removeCount++] = handle; }
    void RemoveDeed(long handle) { g_removed[g_removeCount++] = handle; }
    void RemoveCrimeFromVillage(long handle) { g_removed[g_removeCount++] = handle; }
    long PostDeed(long type, const C3DVector& position, CThing* target, CThingAICreature* victim,
                  long postType, bool a, bool b, CFaction* faction, bool c, long extra)
    {
        ++g_posts; g_postPos = &position;
        if (type != 1 || target || victim || postType != 2 || !a || !b || faction || c || extra) g_ok = false;
        return 77;
    }
    void PlayerTakenOutWeapon()
    {
        if (WeaponOutDeedHandle != -1) {
            RemoveDeedStillSearchingForWitnesses(WeaponOutDeedHandle);
            RemoveDeed(WeaponOutDeedHandle);
            RemoveCrimeFromVillage(WeaponOutDeedHandle);
            WeaponOutDeedHandle = -1;
        }
        if (WeaponOutCrimeEnabled)
            WeaponOutDeedHandle = PostDeed(1, ParentThing->GetPosition(), 0, 0, 2, true, true, 0, false, 0);
    }
};
int main()
{
    CThing hero; hero.Position.x = 1.0f; hero.Position.y = 2.0f; hero.Position.z = 3.0f;
    CTCHeroOpinionDeedLog_Retail log; log.ParentThing = &hero; log.WeaponOutDeedHandle = 12; log.WeaponOutCrimeEnabled = false;
    log.PlayerTakenOutWeapon();
    if (g_removeCount != 3 || g_removed[0] != 12 || g_removed[1] != 12 || g_removed[2] != 12 || log.WeaponOutDeedHandle != -1 || g_posts != 0) {
        std::printf("006c6e20_TEST FAIL\n"); return 1;
    }
    log.WeaponOutCrimeEnabled = true;
    log.PlayerTakenOutWeapon();
    if (g_removeCount != 3 || g_posts != 1 || !g_ok || g_postPos != &hero.Position || log.WeaponOutDeedHandle != 77) {
        std::printf("006c6e20_TEST FAIL\n"); return 1;
    }
    std::printf("006c6e20_TEST PASS\n");
    return 0;
}
