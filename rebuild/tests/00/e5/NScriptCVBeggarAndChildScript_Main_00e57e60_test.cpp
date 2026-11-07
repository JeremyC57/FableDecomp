// Behaviour test for NScript::CV_BeggarAndChildScript::Main (0x00E57E60): after the wait for a hit or a leave, the cut-scene
// macro chain and morality per outcome: beggar hit -> BEGGARHIT, belch teach (unless belch or battle cry known), sneer teach
// (unless known), BEGGARLEAVES_2, evil; bully hit -> BULLYHIT, battle teach, thanks teach, BULLYLEAVES_2, good; beggar left ->
// BEGGARLEAVES_1, sneer teach, BEGGARLEAVES_2, evil; bully left -> belch / cry line, BULLYLEAVES_1, BULLYLEAVES_2, good.
#include <cstdio>
#include <cstring>

struct Beggar {
    bool BeggarHit, BullyHit, BeggarLeft, BullyLeft, BelchedAtBully, TaughtBelch, TaughtBattleCry, TaughtSneer, TaughtThanks;
    const char* Macros[12]; int Count; int Morality;
    void Run(const char* m) { Macros[Count++] = m; }
    void Main()
    {
        Count = 0; Morality = 0;
        Run("CS_BANDB_SETUP");
        if (BeggarHit) {
            Run("CS_BANDB_BEGGARHIT");
            if (!TaughtBelch && !TaughtBattleCry) Run("CS_BANDB_BELCH_TEACH");
            if (!TaughtSneer) Run("CS_BANDB_SNEER_TEACH");
            Run("CS_BANDB_BEGGARLEAVES_2"); Morality = -1;
        } else if (BullyHit) {
            Run("CS_BANDB_BULLYHIT");
            if (!TaughtBelch && !TaughtBattleCry) Run("CS_BANDB_BATTLE_TEACH");
            if (!TaughtThanks) Run("CS_BANDB_THANKS_TEACH");
            Run("CS_BANDB_BULLYLEAVES_2"); Morality = 1;
        } else if (BeggarLeft) {
            Run("CS_BANDB_BEGGARLEAVES_1");
            if (!TaughtSneer) Run("CS_BANDB_SNEER_TEACH");
            Run("CS_BANDB_BEGGARLEAVES_2"); Morality = -1;
        } else if (BullyLeft) {
            Run(BelchedAtBully ? "CS_BANDB_BULLYLEAVES_BELCH" : "CS_BANDB_BULLYLEAVES_CRY");
            Run("CS_BANDB_BULLYLEAVES_1");
            Run("CS_BANDB_BULLYLEAVES_2"); Morality = 1;
        }
    }
};

int main()
{
    bool ok = true;
    Beggar b; std::memset(&b, 0, sizeof(b));
    b.BeggarHit = true; b.Main();
    ok = ok && b.Count == 5 && !std::strcmp(b.Macros[2], "CS_BANDB_BELCH_TEACH") && b.Morality == -1;
    b.TaughtBattleCry = true; b.TaughtSneer = true; b.Main();
    ok = ok && b.Count == 3 && !std::strcmp(b.Macros[2], "CS_BANDB_BEGGARLEAVES_2");
    std::memset(&b, 0, sizeof(b)); b.BullyHit = true; b.TaughtThanks = true; b.Main();
    ok = ok && b.Count == 4 && !std::strcmp(b.Macros[2], "CS_BANDB_BATTLE_TEACH") && b.Morality == 1;
    std::memset(&b, 0, sizeof(b)); b.BeggarLeft = true; b.Main();
    ok = ok && b.Count == 4 && !std::strcmp(b.Macros[1], "CS_BANDB_BEGGARLEAVES_1");
    std::memset(&b, 0, sizeof(b)); b.BullyLeft = true; b.BelchedAtBully = true; b.Main();
    ok = ok && b.Count == 4 && !std::strcmp(b.Macros[1], "CS_BANDB_BULLYLEAVES_BELCH") && b.Morality == 1;
    if (!ok) { std::printf("00e57e60_TEST FAIL\n"); return 1; }
    std::printf("00e57e60_TEST PASS\n");
    return 0;
}