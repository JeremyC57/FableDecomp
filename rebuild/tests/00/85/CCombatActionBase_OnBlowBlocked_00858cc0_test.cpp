// Behaviour test for 00858cc0: src/00858cc0.cpp copied verbatim; the creature/target things, the carrying component,
// the combat-type def and the action plumbing stubbed so the base call, the alive gates, the held-weapon gate, the
// state/blocked-response test and the recoil action (finish, then a STRIKE_RECOIL interrupted attack) are checked.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CCombatActionBase::OnBlowBlocked(bool flag) -- after the creature-action handling, when both the target and the attacker are alive, the attacker's primary-slot weapon (if it carries anything) is alive, and the attacker is in state 2 or its combat type does not ignore blocks, finish this action and make the attacker recoil (CCreatureAction_GeneralInterruptedAttack "STRIKE_RECOIL")
// Retry 10b (chain-split sweep, FIXES B-16): the `GetPItem()` result is a named local `item` before it is dereferenced, which gives retail's register colouring/order (the chained call gives the parked difference).
// Leaf: ?OnBlowBlocked@CCombatActionBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00858CC0, length 317 bytes (`ret 4`).
// Callees: CCreatureActionBase::OnBlowBlocked(bool) 0x006927F0, CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap::
//          LowerBound const 0x004365B0 (interface 0x46 = CTCCarrying), CTCCarrying::GetThingInPrimarySlot 0x006A1440,
//          CThingCreatureBase::GetCombatTypeDef 0x00666D10, this's slot 4 (+0x10, FinishAction), CCharString 0x0099EBF0,
//          CCreatureAction_GeneralInterruptedAttack(CThingCreatureBase*, CThing*, CCharString) 0x0087CF90 (landed),
//          CThingCreatureBase::SetCurrentAction 0x006644F0, ~CCombatActionBase 0x0062E900; literal 0x012764F0.
// Layout: the retail CCombatActionBase (PCreature +8, PTarget +0xa8, 0x114 bytes); the creature's state byte +0x10;
//          the combat type def's block response +0x98.
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* end;
    SEntry* LowerBound(const long* type) const;  // 0x004365B0
};
class CThing { public:
    unsigned char _pad0[0x10];
    unsigned char m_state;                       // +0x10
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];              // +0x20
    CInterfaceMap m_interfaces;                  // +0x44
    unsigned char _pad50[0x91 - 0x50];
    unsigned char m_flags91;                     // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
class CTCCarrying { public:
    CThing* GetThingInPrimarySlot();             // 0x006A1440
};
class CCombatTypeDef { public:
    unsigned char _pad[0x98];
    long          BlockedResponse;               // +0x98
};
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                              // 0x0099EAE0
};
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                             // +0x04
    CThing* GetPItem() const;                    // 0x00A01B50
};
class CSoundPair { public: long FirstSound, LastSound; };
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
class CThingCreatureBase;
class CCombatActionBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishAction();                 // slot 4 (+0x10)
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;           // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CBaseIntelligentPointer PTarget;             // +0xa8
    unsigned char           _padb0[0x114 - 0xb0];
    ~CCombatActionBase();                        // 0x0062E900
    void CreatureOnBlowBlocked(bool flag);       // 0x006927F0 (CCreatureActionBase::OnBlowBlocked)
    void OnBlowBlocked(bool flag);
};
class CCreatureAction_GeneralInterruptedAttack : public CCombatActionBase { public:   // vtable 0x01277A14
    CCreatureAction_GeneralInterruptedAttack(CThingCreatureBase* owner, CThing* target, CCharString animName);   // 0x0087CF90
};
class CThingCreatureBase : public CThing { public:
    CCombatTypeDef* GetCombatTypeDef();          // 0x00666D10
    bool SetCurrentAction(const CCombatActionBase& action);   // 0x006644F0
};

void CCombatActionBase::OnBlowBlocked(bool flag)
{
    CreatureOnBlowBlocked(flag);
    if (IS_THING_ALIVE(PTarget) && IS_THING_ALIVE(PCreature)) {
        bool canRecoil = true;
        CTCCarrying* carrying;
        if (PCreature.GetPItem()->GetInterface(&carrying, 0x46)) {
            CThing* held = carrying->GetThingInPrimarySlot();
            if (!held || (held->m_flags91 & 1))
                canRecoil = false;
        }
        CThing* item = PCreature.GetPItem();
        if ((item->m_state == 2 || ((CThingCreatureBase*)PCreature.GetPItem())->GetCombatTypeDef()->BlockedResponse != 2) && canRecoil) {
            CThing* target = PTarget.GetPItem();
            CThingCreatureBase* creature = (CThingCreatureBase*)PCreature.GetPItem();
            FinishAction();
            creature->SetCurrentAction(CCreatureAction_GeneralInterruptedAttack(creature, target, "STRIKE_RECOIL"));
        }
    }
}
// ---- end source ----
void CCombatActionBase::v00() {} void CCombatActionBase::v01() {} void CCombatActionBase::v02() {} void CCombatActionBase::v03() {}
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
static CTCCarrying g_carrying; static CThing* g_held = 0;
CThing* CTCCarrying::GetThingInPrimarySlot() { return g_held; }
static CInterfaceMap::SEntry g_entries[1] = { { 0x46, (CTCBase*)&g_carrying } };
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const { return *type <= 0x46 ? first : last; }
static CCombatTypeDef g_ctd;
CCombatTypeDef* CThingCreatureBase::GetCombatTypeDef() { return &g_ctd; }
static int g_sets = 0;
bool CThingCreatureBase::SetCurrentAction(const CCombatActionBase&) { ++g_sets; return true; }
static int g_finishes = 0, g_baseCalls = 0, g_dtors = 0;
void CCombatActionBase::FinishAction() { ++g_finishes; }
void CCombatActionBase::CreatureOnBlowBlocked(bool) { ++g_baseCalls; }
CCombatActionBase::~CCombatActionBase() { ++g_dtors; }
static char g_anim[32]; static CThingCreatureBase* g_owner = 0; static CThing* g_actTarget = 0;
CCharString::CCharString(const char* t, long) : m_pData((void*)t) {}
CCharString::~CCharString() {}
CCreatureAction_GeneralInterruptedAttack::CCreatureAction_GeneralInterruptedAttack(CThingCreatureBase* owner, CThing* target, CCharString animName)
{ g_owner = owner; g_actTarget = target; strcpy(g_anim, (const char*)animName.m_pData); }
int main()
{
    static unsigned char cRaw[0x100], tRaw[0x100], hRaw[0x100];
    memset(cRaw, 0, sizeof(cRaw)); memset(tRaw, 0, sizeof(tRaw)); memset(hRaw, 0, sizeof(hRaw));
    CThing* creature = (CThing*)cRaw; CThing* target = (CThing*)tRaw; CThing* held = (CThing*)hRaw;
    creature->m_interfaces.first = g_entries; creature->m_interfaces.last = g_entries + 1;
    static CCombatActionBase a; a.PCreature.m_pItem = creature; a.PTarget.m_pItem = target;
    g_ctd.BlockedResponse = 1;
    a.OnBlowBlocked(true);                                    // no carrying component: recoil
    CHECK(g_baseCalls == 1 && g_finishes == 1 && g_sets == 1 && strcmp(g_anim, "STRIKE_RECOIL") == 0 && g_owner == (CThingCreatureBase*)creature && g_actTarget == target);
    creature->m_presentBits[0x46 >> 5] |= 1UL << (0x46 & 31);
    a.OnBlowBlocked(true); CHECK(g_sets == 1 && g_baseCalls == 2);   // carrying but nothing held
    g_held = held; a.OnBlowBlocked(true); CHECK(g_sets == 2);
    g_ctd.BlockedResponse = 2; a.OnBlowBlocked(true); CHECK(g_sets == 2);   // response 2 and not state 2
    creature->m_state = 2; a.OnBlowBlocked(true); CHECK(g_sets == 3);
    target->m_flags91 = 1; a.OnBlowBlocked(true); CHECK(g_sets == 3 && g_baseCalls == 6);   // dead target
    if (fails == 0) printf("00858cc0_TEST PASS\n"); else printf("00858cc0_TEST FAIL\n");
    return fails;
}