#include <cstdio>
#include <cstring>
// Behaviour test for CTCSpecialAbilities::SetupBerserk (0x0072CBA0): the body of src/ over stand-ins -- false when the
// creature is flagged; with a live held primary weapon, a sheathe action that follows up with a berserk carrying the weapon;
// otherwise a plain berserk cast.
class CThing;
struct CBaseIntelligentPointer {
    CThing* item;
    CThing* GetPItem() const { return item; }
};
struct CTCCarrying;
struct CTCHeldItem;
class CThing {
public:
    unsigned char m_flags30;
    unsigned char m_flags91;
    CTCCarrying* carrying;
    CTCHeldItem* held;
    bool GetInterface(CTCCarrying** ppOut, long type) const { if (type != 0x46 || !carrying) return false; *ppOut = carrying; return true; }
    bool GetInterface(CTCHeldItem** ppOut, long type) const { if (type != 0x13 || !held) return false; *ppOut = held; return true; }
};
template <class T> struct CIntelligentPointer {
    T* item;
    CIntelligentPointer(T* i) : item(i) {}
};
struct CTCCarrying {
    CThing* primary;
    CThing* GetThingInPrimarySlot() { return primary; }
};
struct CTCHeldItem { CBaseIntelligentPointer HeldItem; };
class CThingCreatureBase;
class CCreatureActionBase {
public:
    int kind;                       // 1 berserk, 2 sheathe
    const CThing* item;
    const CCreatureActionBase* followUp;
    const char* name;
};
class CCreatureAction_CastStrengthBerserk : public CCreatureActionBase {
public:
    CCreatureAction_CastStrengthBerserk(CThingCreatureBase* creature) { kind = 1; item = 0; followUp = 0; name = 0; }
    void SetItem(CIntelligentPointer<const CThing> i) { item = i.item; }
};
struct CCharString { const char* text; };
static const CCharString g_sheatheForAbilityName = { "SHEATHE_FOR_ABILITY" };
class CCreatureAction_SheatheItemToInventory : public CCreatureActionBase {
public:
    CCreatureAction_SheatheItemToInventory(CThingCreatureBase* creature, const CCharString& n, CCreatureActionBase* f, long flags)
    {
        kind = 2; item = 0; followUp = f; name = n.text;
    }
};
static CCreatureActionBase g_set;
static int g_sets = 0;
class CThingCreatureBase : public CThing {
public:
    bool result;
    bool SetCurrentAction(const CCreatureActionBase& action) { g_set = action; ++g_sets; return result; }
};
class CTCSpecialAbilities {
public:
    CThingCreatureBase* Creature;
    bool SetupBerserk();
};

bool CTCSpecialAbilities::SetupBerserk()
{
    if (!(Creature->m_flags30 & 0x10)) {
        CTCCarrying* carrying;
        if (Creature->GetInterface(&carrying, 0x46)) {
            CThing* weapon = carrying->GetThingInPrimarySlot();
            if (weapon && !(weapon->m_flags91 & 1)) {
                CTCHeldItem* held;
                Creature->GetInterface(&held, 0x13);
                CThing* item = (CThing*)held->HeldItem.GetPItem();
                if (weapon == item) {
                    CCreatureAction_CastStrengthBerserk* berserk = new CCreatureAction_CastStrengthBerserk(Creature);
                    berserk->SetItem(item);
                    CCreatureAction_SheatheItemToInventory action(Creature, g_sheatheForAbilityName, berserk, 0);
                    return Creature->SetCurrentAction(action);
                }
            }
        }
        return Creature->SetCurrentAction(CCreatureAction_CastStrengthBerserk(Creature));
    }
    return false;
}

static int fail(int line) { printf("0072cba0_TEST FAIL (line %d)\n", line); return 1; }
#define CHECK(c) do { if (!(c)) return fail(__LINE__); } while (0)

int main()
{
    CThing sword; sword.m_flags91 = 0;
    CTCCarrying carrying = { &sword };
    CTCHeldItem held = { { &sword } };
    CThingCreatureBase hero; hero.m_flags30 = 0; hero.carrying = &carrying; hero.held = &held; hero.result = true;
    CTCSpecialAbilities sa; sa.Creature = &hero;
    // held primary weapon: sheathe first, berserk carries the weapon
    CHECK(sa.SetupBerserk() && g_sets == 1 && g_set.kind == 2 && strcmp(g_set.name, "SHEATHE_FOR_ABILITY") == 0);
    CHECK(g_set.followUp && g_set.followUp->kind == 1 && g_set.followUp->item == &sword);
    delete (CCreatureAction_CastStrengthBerserk*)g_set.followUp;
    // weapon not the held item: plain berserk
    CThing other; held.HeldItem.item = &other;
    hero.result = false;
    CHECK(!sa.SetupBerserk() && g_sets == 2 && g_set.kind == 1 && g_set.item == 0);
    // dead weapon / no weapon / no carrying: plain berserk
    held.HeldItem.item = &sword; sword.m_flags91 = 1; hero.result = true;
    CHECK(sa.SetupBerserk() && g_sets == 3 && g_set.kind == 1);
    carrying.primary = 0;
    CHECK(sa.SetupBerserk() && g_sets == 4 && g_set.kind == 1);
    hero.carrying = 0;
    CHECK(sa.SetupBerserk() && g_sets == 5 && g_set.kind == 1);
    // flagged creature: false, no action
    hero.m_flags30 = 0x10;
    CHECK(!sa.SetupBerserk() && g_sets == 5);
    printf("0072cba0_TEST PASS\n");
    return 0;
}
