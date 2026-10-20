// Behaviour test for CQ_MinionCampScript::AddNewHeroAttacker (0x00DA0B30): the attacker goes into slot one when it is
// null or dead, else into slot two when that one is null or dead, else nowhere; the chosen slot shares the attacker's
// counted block (old block released) and gets the hero as its best enemy target; a terminating thread stops it.
#include <cstdio>
#include <cstring>

struct CCPPointerInfo { long RefCount; void (*DeleteFunc)(void*); void* Data; };
static int g_deleted = 0;
static void DeletePointee(void*) { ++g_deleted; }
template <class T> struct CCountedPointer {
    T* Data; CCPPointerInfo* Info;
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info; T* data = other.Data;
        if (Info != info) {
            if (Info) { --Info->RefCount; if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } }
            Data = data; Info = info; if (info) ++info->RefCount;
        }
        return *this;
    }
};
struct CBaseClass { virtual ~CBaseClass() {} };
struct CScriptThing : CBaseClass {
    CCountedPointer<CScriptThing> PImp;
    bool alive;
    CScriptThing() : alive(false) { PImp.Data = 0; PImp.Info = 0; }
    CScriptThing& operator=(const CScriptThing& o) { PImp = o.PImp; return *this; }
    void Assign(const CScriptThing& o) { PImp = o.PImp; }
    virtual bool IsAlive() const { return alive; }
    virtual bool IsNull() const { return PImp.Data == 0; }
};
static CScriptThing g_hero, g_game;
struct CGameScriptInterface {
    const CScriptThing* gaveTo;
    virtual CScriptThing& GetHero() { return g_hero; }
    virtual void GiveThingBestEnemyTarget(const CScriptThing& t, const CScriptThing& e) { gaveTo = &t; if (&e != &g_hero) gaveTo = 0; }
};
struct CScriptBase { bool term; int polls; bool IsActiveThreadTerminating() { ++polls; return term; } };
struct CQ_MinionCampScript : CScriptBase {
    CGameScriptInterface* m_pInterface;
    CScriptThing HeroAttackerOne;
    CScriptThing HeroAttackerTwo;
    void AddNewHeroAttacker(const CScriptThing& attacker);
};
void CQ_MinionCampScript::AddNewHeroAttacker(const CScriptThing& attacker)
{
    if (!HeroAttackerOne.IsNull() && HeroAttackerOne.IsAlive()) {
        if (!HeroAttackerTwo.IsNull() && HeroAttackerTwo.IsAlive()) {
            IsActiveThreadTerminating();
            return;
        }
        if (IsActiveThreadTerminating())
            return;
        HeroAttackerTwo = attacker;
        m_pInterface->GiveThingBestEnemyTarget(HeroAttackerTwo, m_pInterface->GetHero());
    } else {
        if (IsActiveThreadTerminating())
            return;
        HeroAttackerOne.Assign(attacker);
        m_pInterface->GiveThingBestEnemyTarget(HeroAttackerOne, m_pInterface->GetHero());
    }
}
static CCPPointerInfo* Block() { CCPPointerInfo* b = new CCPPointerInfo; b->RefCount = 1; b->DeleteFunc = DeletePointee; b->Data = &g_game; return b; }
int main()
{
    int bad = 0;
    CGameScriptInterface gi;
    CScriptThing attacker; attacker.PImp.Data = &g_game; attacker.PImp.Info = Block();
    // slot one empty
    { CQ_MinionCampScript s; s.term = false; s.polls = 0; s.m_pInterface = &gi; gi.gaveTo = 0;
      s.AddNewHeroAttacker(attacker);
      if (s.HeroAttackerOne.PImp.Info != attacker.PImp.Info || attacker.PImp.Info->RefCount != 2 || gi.gaveTo != &s.HeroAttackerOne) bad = 1;
      s.HeroAttackerOne.PImp.Info->RefCount--; }
    // slot one alive, slot two dead with its own block: the old block is released and deleted
    { CQ_MinionCampScript s; s.term = false; s.polls = 0; s.m_pInterface = &gi; gi.gaveTo = 0;
      s.HeroAttackerOne.PImp.Data = &g_game; s.HeroAttackerOne.alive = true;
      s.HeroAttackerTwo.PImp.Data = &g_game; s.HeroAttackerTwo.PImp.Info = Block(); s.HeroAttackerTwo.alive = false;
      g_deleted = 0;
      s.AddNewHeroAttacker(attacker);
      if (s.HeroAttackerTwo.PImp.Info != attacker.PImp.Info || g_deleted != 1 || gi.gaveTo != &s.HeroAttackerTwo) bad = 1;
      s.HeroAttackerTwo.PImp.Info->RefCount--; }
    // both alive: only the thread poll
    { CQ_MinionCampScript s; s.term = false; s.polls = 0; s.m_pInterface = &gi; gi.gaveTo = 0;
      s.HeroAttackerOne.PImp.Data = &g_game; s.HeroAttackerOne.alive = true;
      s.HeroAttackerTwo.PImp.Data = &g_game; s.HeroAttackerTwo.alive = true;
      s.AddNewHeroAttacker(attacker);
      if (s.polls != 1 || gi.gaveTo != 0 || s.HeroAttackerTwo.PImp.Info != 0) bad = 1; }
    // terminating: nothing assigned
    { CQ_MinionCampScript s; s.term = true; s.polls = 0; s.m_pInterface = &gi; gi.gaveTo = 0;
      s.AddNewHeroAttacker(attacker);
      if (s.HeroAttackerOne.PImp.Info != 0 || gi.gaveTo != 0) bad = 1; }
    if (attacker.PImp.Info->RefCount != 1) bad = 1;
    if (bad) { std::printf("00da0b30_TEST FAIL\n"); return 1; }
    std::printf("00da0b30_TEST PASS\n");
    return 0;
}
