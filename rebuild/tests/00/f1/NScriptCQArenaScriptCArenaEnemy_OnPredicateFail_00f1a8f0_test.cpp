// Behaviour test for 0x00F1A8F0 NScript::CQ_ArenaScript::CArenaEnemy::OnPredicateFail: the creature-type count drops by
// one; a hero kill adds Rounds[ArenaRound].Waves[ArenaRoundWave].Creatures[MyCreatureType].DeathScore to NewCrowdPoints;
// a big creature removes its quest-info health bar.
#include <cstdio>
#include <cstring>

class CCharString { public: const char* Text; CCharString(const char* t) : Text(t) { } };
class CScriptThing { public:
    const char* Killer;
    bool MsgIsKilledBy(const CCharString& killer) const { return Killer && std::strcmp(Killer, killer.Text) == 0; }
};
class CGameScriptInterfaceBase { public:
    long Removed; int Calls;
    void RemoveQuestInfoElement(long element) { Removed = element; ++Calls; }
};
class CArenaCreatureDef { public: long DeathScore; };
class CArenaWaveDef { public: CArenaCreatureDef* Creatures; };
class CArenaRoundDef { public: CArenaWaveDef* Waves; };
struct CQ_ArenaScript {
    CArenaRoundDef* Rounds; long ArenaRound; long ArenaRoundWave; long TotalCreatures[3]; long NewCrowdPoints;
};
struct CArenaEnemy {
    CGameScriptInterfaceBase* Game; CScriptThing Me; CQ_ArenaScript* ParentClass;
    long HealthBarIndex; bool BigCreature; long MyCreatureType;
    void OnPredicateFail()
    {
        --ParentClass->TotalCreatures[MyCreatureType];
        if (Me.MsgIsKilledBy("SCRIPT_NAME_HERO"))
            ParentClass->NewCrowdPoints += ParentClass->Rounds[ParentClass->ArenaRound].Waves[ParentClass->ArenaRoundWave].Creatures[MyCreatureType].DeathScore;
        if (BigCreature)
            Game->RemoveQuestInfoElement(HealthBarIndex);
    }
};

int main()
{
    CArenaCreatureDef c0[3] = { { 1 }, { 2 }, { 3 } };
    CArenaCreatureDef c1[3] = { { 10 }, { 20 }, { 30 } };
    CArenaWaveDef w[2] = { { c0 }, { c1 } };
    CArenaRoundDef r[2] = { { 0 }, { w } };
    CQ_ArenaScript q; q.Rounds = r; q.ArenaRound = 1; q.ArenaRoundWave = 1;
    q.TotalCreatures[0] = q.TotalCreatures[1] = q.TotalCreatures[2] = 5; q.NewCrowdPoints = 100;
    CGameScriptInterfaceBase g = { 0, 0 };
    CArenaEnemy e; e.Game = &g; e.ParentClass = &q; e.HealthBarIndex = 7; e.BigCreature = true; e.MyCreatureType = 2;
    e.Me.Killer = "SCRIPT_NAME_HERO";
    e.OnPredicateFail();
    int ok = q.TotalCreatures[2] == 4 && q.TotalCreatures[1] == 5 && q.NewCrowdPoints == 130 && g.Calls == 1 && g.Removed == 7;
    e.Me.Killer = "Other"; e.BigCreature = false;
    e.OnPredicateFail();
    ok = ok && q.TotalCreatures[2] == 3 && q.NewCrowdPoints == 130 && g.Calls == 1;
    if (!ok) { std::printf("00f1a8f0_TEST FAIL\n"); return 1; }
    std::printf("00f1a8f0_TEST PASS\n");
    return 0;
}