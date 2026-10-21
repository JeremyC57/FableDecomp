// Behaviour test for CAIStateGroup_MinionGuardReactToCriminal::IsRunnable (0x00932990): model copy with stub callees.
#include <cstdio>
#include <cstring>

struct CThing { unsigned char m_flags91; };
static bool IS_THING_ALIVE(CThing* t) { return t != 0 && !(t->m_flags91 & 1); }
struct CIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
    CIntelligentPointer& operator=(CThing* t) { CThing* item = t; m_pItem = item; return *this; }
};
static bool operator==(const CThing* thing, const CIntelligentPointer& p) { return thing == p.GetPItem(); }
struct CCharString {
    const char* text;
    bool operator==(const char* t) const { if (text == 0) return std::strcmp(t, "") == 0; return std::strcmp(text, t) == 0; }
};
struct CTCVillage {
    bool criminal; const char* warning; CIntelligentPointer PCriminalHandler;
    bool IsHeroCriminal() { return criminal; }
    CCharString GetWarningStringForCurrentCrimes(long, bool& f, long& a) const { f = false; a = 0; CCharString s; s.text = warning; return s; }
};
struct CTCPerceiveThing {
    CThing* perceived; bool canSee;
    CThing* GetPerceivedThing() { return perceived; }
    bool CanSeeOrSmellPerceivedThing() const { return canSee; }
};
struct CCreature { CTCVillage* village; CTCPerceiveThing* perceive; };
static CThing* g_hero;
struct CAIBrain { CCreature* PCreature; };

struct CAIStateGroup_MinionGuardReactToCriminal {
    CAIBrain* PBrain; CIntelligentPointer PHero;
    CThing* GetHero() { return g_hero; }
    bool IsRunnable();
};

bool CAIStateGroup_MinionGuardReactToCriminal::IsRunnable()
{
    CTCVillage* village = PBrain->PCreature->village;
    if (village != 0 && village->IsHeroCriminal()) {
        const CCreature* creature = PBrain->PCreature;
        if (creature->perceive != 0) {
            CTCPerceiveThing* perceive = creature->perceive;
            PHero = GetHero();
            if (PHero.GetPItem() != 0 && perceive->GetPerceivedThing() != 0
                && perceive->GetPerceivedThing() == PHero && perceive->CanSeeOrSmellPerceivedThing()) {
                bool isFine;
                long amount;
                bool noWarning = village->GetWarningStringForCurrentCrimes(0, isFine, amount) == "";
                if (noWarning)
                    return true;
                return !IS_THING_ALIVE(village->PCriminalHandler.GetPItem());
            }
        }
    }
    return false;
}

int main()
{
    int ok = 1;
    CThing hero = { 0 }, other = { 0 }, handler = { 0 };
    CTCVillage village = { true, 0, { 0 } };
    CTCPerceiveThing perceive = { &hero, true };
    CCreature guard = { &village, &perceive };
    CAIBrain brain = { &guard };
    CAIStateGroup_MinionGuardReactToCriminal s; s.PBrain = &brain; s.PHero.m_pItem = 0;
    g_hero = &hero;
    // no warning string (null and empty): runnable
    if (!s.IsRunnable() || s.PHero.m_pItem != &hero) ok = 0;
    village.warning = ""; if (!s.IsRunnable()) ok = 0;
    // a warning: runnable only while nobody living handles the criminal
    village.warning = "Stop!";
    if (!s.IsRunnable()) ok = 0;
    village.PCriminalHandler.m_pItem = &handler; if (s.IsRunnable()) ok = 0;
    handler.m_flags91 = 1; if (!s.IsRunnable()) ok = 0;
    // cannot see, perceives someone else, nothing perceived, no hero, no perceiver, not a criminal, no village
    perceive.canSee = false; if (s.IsRunnable()) ok = 0; perceive.canSee = true;
    perceive.perceived = &other; if (s.IsRunnable()) ok = 0;
    perceive.perceived = 0; if (s.IsRunnable()) ok = 0; perceive.perceived = &hero;
    g_hero = 0; if (s.IsRunnable()) ok = 0; g_hero = &hero;
    guard.perceive = 0; if (s.IsRunnable()) ok = 0; guard.perceive = &perceive;
    village.criminal = false; if (s.IsRunnable()) ok = 0; village.criminal = true;
    guard.village = 0; if (s.IsRunnable()) ok = 0; guard.village = &village;
    if (!s.IsRunnable()) ok = 0;
    if (!ok) { std::printf("00932990_TEST FAIL\n"); return 1; }
    std::printf("00932990_TEST PASS\n");
    return 0;
}