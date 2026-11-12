// Behaviour test for 0x0093C030 CAIStateGroup_SeekScriptTarget::Update(): the row's logic over stub classes (C++03).
#include <stdio.h>
#include <stdlib.h>
#include <string>
static std::string trace;
static void check(bool v, int line) { if (!v) { printf("0093c030_TEST FAIL line %d\n", line); exit(1); } }
enum EAIActionReturn { AI_ACTION_NULL = 0, AI_ACTION_TERMINATE_PROCESS = 1, AI_ACTION_FINISHED = 2, AI_ACTION_FAILED = 3, AI_ACTION_IN_PROGRESS = 4 };
enum EAIActionRepeat { ACTION_REPEAT_UNTIL_FINISHED = 1, ACTION_REPEAT_CALLBACK = 2 };
enum EMovementType { AI_MOVEMENT_RUNNING = 5 };
enum EUnsheatheWeaponType { UNSHEATHE_MELEE_WEAPON = 4 };
namespace NStealth { enum EDangerStatus { };}
static unsigned long GFROR13(unsigned long v) { return (v >> 13) | (v << 19); }
static unsigned long GFRandom(unsigned long range, unsigned long* seed)
{ *seed *= 0x24a1; *seed += 0x24df; *seed = GFROR13(*seed); if (range) return *seed % range; return 0; }
class CThingAICreature;
class CTCStealth { public: int Status; void SetAsInDanger(const CThingAICreature&, NStealth::EDangerStatus s) { Status = (int)s; trace += 'D'; } };
class CThing { public: bool Dead; CTCStealth* Stealth; bool IsDead() const { return Dead; }
    bool GetTC(CTCStealth** out) { if (!Stealth) return false; *out = Stealth; return true; } };
template <class T> class CIntelligentPointer { public: T* Item; T* operator->() const { return Item; } };
template <class T> static bool IS_THING_ALIVE(const CIntelligentPointer<T>& p) { if (p.Item != 0) { if (!p->IsDead()) return true; } return false; }
class CCombatDialogueDef { public: long EnemySighted; };
class CombatDialogueManager { public: CCombatDialogueDef Def; bool Cry;
    const CCombatDialogueDef* GetPDialogueDef() { return &Def; }
    bool SaySpeech(CThingAICreature&, unsigned long s) { check(s == 77, __LINE__); trace += 'S'; return true; }
    bool PlayBattleCry(CThingAICreature&) { trace += 'B'; return Cry; } };
class CCombatManager { public: CombatDialogueManager Dialogue; CombatDialogueManager& GetCombatDialogueManager() { return Dialogue; }
    void CreatureUnsheathWeapon(CThingAICreature&, EUnsheatheWeaponType t) { check(t == UNSHEATHE_MELEE_WEAPON, __LINE__); trace += 'U'; } };
class CWorld { public: unsigned long Seed; CCombatManager Combat;
    unsigned long* GetWorldSeed() const { return const_cast<unsigned long*>(&Seed); } CCombatManager& GetCombatManager() { return Combat; } };
class CThingAICreature { public: bool Busy, Free; CCombatManager* Combat;
    bool IsPerformingAnyAction() const { return Busy; } bool IsFreeToFinishAction() const { return Free; }
    void FinishCurrentAction() { trace += 'F'; } CCombatManager& GetCombatManager() { return *Combat; } };
class CAIStateGroup_SeekScriptTarget { public:
    CThingAICreature creature; CWorld world; CIntelligentPointer<CThing> PEnemy;
    int runnable_left; const EAIActionReturn* moves;
    CThingAICreature& GetCreature() const { return const_cast<CThingAICreature&>(creature); }
    CWorld& GetWorld() const { return const_cast<CWorld&>(world); }
    bool IsStillRunnable() const { return const_cast<CAIStateGroup_SeekScriptTarget*>(this)->runnable_left-- > 0; }
    void Terminate() { trace += 'X'; }
    EAIActionReturn AIAction_MoveToThingProximity(EAIActionRepeat repeat, CIntelligentPointer<CThing> thing, float proximity, bool& reached,
        EMovementType movement, bool a, bool b, bool c, bool d, float speed)
    { check(repeat == ACTION_REPEAT_CALLBACK && thing.Item == PEnemy.Item && proximity == 1.0f && !reached && movement == AI_MOVEMENT_RUNNING
            && a && !b && c && d && speed == 0.0f, __LINE__); trace += 'M'; return *moves++; }
    void Update();
};
void CAIStateGroup_SeekScriptTarget::Update()
{
    if (IsStillRunnable()) {
        if (GetCreature().IsPerformingAnyAction() && GetCreature().IsFreeToFinishAction())
            GetCreature().FinishCurrentAction();
        bool reached = false;
        bool called_out = false;
        EAIActionReturn ret;
        do {
            if (!IS_THING_ALIVE(PEnemy)) {
                Terminate();
                return;
            }
            if (!called_out) {
                if (GFRandom(100, GetWorld().GetWorldSeed()) > 50) {
                    unsigned long speech = GetWorld().GetCombatManager().GetCombatDialogueManager().GetPDialogueDef()->EnemySighted;
                    GetWorld().GetCombatManager().GetCombatDialogueManager().SaySpeech(GetCreature(), speech);
                    called_out = true;
                } else {
                    called_out = GetWorld().GetCombatManager().GetCombatDialogueManager().PlayBattleCry(GetCreature());
                }
            }
            CTCStealth* stealth;
            if (PEnemy->GetTC(&stealth))
                stealth->SetAsInDanger(GetCreature(), (NStealth::EDangerStatus)2);
            GetCreature().GetCombatManager().CreatureUnsheathWeapon(GetCreature(), UNSHEATHE_MELEE_WEAPON);
            ret = AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK, PEnemy, 1.0f, reached, AI_MOVEMENT_RUNNING, true, false, true, true, 0.0f);
            if (!IsStillRunnable()) {
                Terminate();
                return;
            }
        } while (ret == AI_ACTION_IN_PROGRESS);
        if (ret == AI_ACTION_FINISHED)
            return;
    }
    Terminate();
}

static std::string run(CAIStateGroup_SeekScriptTarget& g, unsigned long seed, int runnable, const EAIActionReturn* moves)
{ g.world.Seed = seed; g.runnable_left = runnable; g.moves = moves; trace = ""; g.Update(); return trace; }
int main()
{
    CTCStealth stealth; stealth.Status = 0;
    CThing enemy; enemy.Dead = false; enemy.Stealth = &stealth;
    CAIStateGroup_SeekScriptTarget g;
    g.PEnemy.Item = &enemy; g.creature.Busy = true; g.creature.Free = true; g.creature.Combat = &g.world.Combat;
    g.world.Combat.Dialogue.Def.EnemySighted = 77; g.world.Combat.Dialogue.Cry = false;
    unsigned long hi = 0, lo = 0;                                           // seeds whose first roll is > 50 / <= 50
    for (unsigned long s = 1; s < 1000 && (!hi || !lo); ++s) { unsigned long t = s; if (GFRandom(100, &t) > 50) { if (!hi) hi = s; } else if (!lo) lo = s; }
    static const EAIActionReturn m1[] = { AI_ACTION_IN_PROGRESS, AI_ACTION_FINISHED };
    check(run(g, hi, 10, m1) == "FSDUMDUM" && stealth.Status == 2, __LINE__);    // speech once, then reached: no terminate
    static const EAIActionReturn m2[] = { AI_ACTION_IN_PROGRESS, AI_ACTION_FAILED };
    std::string t = run(g, lo, 10, m2);
    check(t.substr(0, 5) == "FBDUM" && t[t.size() - 1] == 'X', __LINE__);       // failed battle cry is retried; failure terminates
    g.creature.Free = false; enemy.Stealth = 0;
    check(run(g, hi, 1, m1) == "SUMX", __LINE__);                                 // no longer runnable after the move
    check(run(g, hi, 0, m1) == "X", __LINE__);                                    // not runnable at all
    enemy.Dead = true;
    check(run(g, hi, 10, m1) == "X", __LINE__);                                   // dead enemy
    printf("0093c030_TEST PASS\n");
    return 0;
}