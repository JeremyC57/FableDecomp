// CQ_DragonBossFightScript::SpawnEnemies() -- enable minion spawning; every N dragon fly-bys also enable summoner spawning and restart the fly-by count (unless the script thread is terminating)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D25C20, length 53 bytes.
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940 (landed).
// Layout (Ego_r.pdb CQ_DragonBossFightScript - 0x1c: base -0x14 and two PDB 16-byte vectors -> 12):
// NumFlyBysBetweenSummonerSpawns +0x78, NumFlyBysSinceLastSummonerSpawn +0x7c, MinionSpawningEnabled +0x80,
// SummonerSpawningEnabled +0x81.
// Shape: the pre-increment `++count >= between` keeps the incremented value in edx and compares a copy in
// eax; the MinionSpawningEnabled store is scheduled between the compare and the `jl`.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
struct CGameScriptInterface_Calls {                      // the quest's game script interface (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame();   // slot 7 (+0x1c)
};

class CScriptBase { public:                                        // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                              // +0x04 .. +0x40 (PActiveThread at +0x2c)
    bool IsActiveThreadTerminating();                       // 0x00CB7940
};

struct CQuestScriptBase : CScriptBase {                     // retail 0x48 bytes
    CGameScriptInterface_Calls* GSI;                              // +0x40
    unsigned char _pad44[4];                                // +0x44
};

namespace NScript {
class CQ_DragonBossFightScript : public CQuestScriptBase {
public:
    unsigned char _pad48[0x30];                             // +0x48
    long NumFlyBysBetweenSummonerSpawns;                    // +0x78
    long NumFlyBysSinceLastSummonerSpawn;                   // +0x7c
    bool MinionSpawningEnabled;                             // +0x80
    bool SummonerSpawningEnabled;                           // +0x81
    void SpawnEnemies();
};
} // namespace NScript
using NScript::CQ_DragonBossFightScript;

void NScript::CQ_DragonBossFightScript::SpawnEnemies()
{
    MinionSpawningEnabled = true;
    if (++NumFlyBysSinceLastSummonerSpawn >= NumFlyBysBetweenSummonerSpawns) {
        if (!IsActiveThreadTerminating()) {
            SummonerSpawningEnabled = true;
            NumFlyBysSinceLastSummonerSpawn = 0;
        }
    }
}