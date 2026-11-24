// Use the shared timer declaration, actual vector type and complete production method.
// The fixture supplies only the unused creature dependency of the timer constructor.
#include <stdio.h>
class TimerFixtureCreature {};
#define CThing TimerFixtureCreature
#define FABLE_RETAIL_CWILLBUTTONHELPERTIMER_GET_ACCUMULATED_LEVEL_PASS26
#include "engine/CWillButtonHelperTimer.h"
#undef CThing

CBaseIntelligentPointer::~CBaseIntelligentPointer() {}
CWillButtonHelperTimer::~CWillButtonHelperTimer() {}
static long startLevel;
static long aiLimit;
static float playerLimit;
static bool aiCasting;
CWillButtonHelperTimer::CWillButtonHelperTimer(EHeroAbility ability, TimerFixtureCreature&)
    : ButtonHeldDownTimer(-19), WillAccumulatedLevel(startLevel), MyAbility(ability),
      CurrentState(RELEASED), WillPowerUpAccumulatedTimings(4U, 0.0f),
      MaxNumberLevels(4.0f), CurMaxLevels(playerLimit), WillIncrementedFlag(false),
      UpdateAccLevel(false), IsAICasting(aiCasting), IsAIHoldingDown(false), LevelToCast(aiLimit) {
    WillPowerUpAccumulatedTimings[0] = 7.75f;
    WillPowerUpAccumulatedTimings[1] = -4.5f;
    WillPowerUpAccumulatedTimings[2] = 12.25f;
    WillPowerUpAccumulatedTimings[3] = 2.0f;
}
long CWillButtonHelperTimer::GetButtonHeldDownForTime() const { return ButtonHeldDownTimer; }

void CWillButtonHelperTimer::ForceToNextLevel() {
    long cur_level = GetWillAccumulatedLevel();
    if (IsAICasting) {
        if (cur_level < LevelToCast) {
            ++WillAccumulatedLevel;
            ButtonHeldDownTimer = (long)WillPowerUpAccumulatedTimings[WillAccumulatedLevel - 1];
        }
    } else if (cur_level < CurMaxLevels) {
        ++WillAccumulatedLevel;
        ButtonHeldDownTimer = (long)WillPowerUpAccumulatedTimings[WillAccumulatedLevel - 1];
    }
}

static bool Check(bool ai, long start, long aiMax, float playerMax, long level, long time) {
    aiCasting = ai; startLevel = start; aiLimit = aiMax; playerLimit = playerMax;
    TimerFixtureCreature creature;
    CWillButtonHelperTimer timer(HERO_ABILITY_FIREBALL_SPELL, creature);
    timer.ForceToNextLevel();
    return timer.GetWillAccumulatedLevel() == level && timer.GetButtonHeldDownForTime() == time;
}
int main() {
    if (!Check(true, 0, 2, 0.0f, 1, 7) || !Check(true, 1, 2, 0.0f, 2, -4) ||
        !Check(true, 2, 2, 4.0f, 2, -19) || !Check(true, 3, 2, 4.0f, 3, -19) ||
        !Check(false, 2, 0, 2.5f, 3, 12) || !Check(false, 3, 4, 3.0f, 3, -19) ||
        !Check(false, 3, 4, 2.5f, 3, -19))
        return 1;
    puts("008edc70_TEST PASS");
    return 0;
}