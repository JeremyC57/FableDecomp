// CCreatureAction_CastSkillMultiArrow::Initialise() -- base Initialise, then make sure the owner has a CTCMultiArrow component (adding "CTCMultiArrow" and, for an AI-state owner, setting its AI cast level) before looking it up
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CDC40, length 168 bytes (+ int3 padding), plain `ret` (virtual, no arguments).
// Callees: CCombatActionBase::Initialise 0x00857520 (direct), CBaseIntelligentPointer::GetPItem 0x00A01B50, CCharString ctor 0x0099EBF0 /
// dtor 0x0099EAE0 ("CTCMultiArrow" @ 0x0123D364), CThing::AddTC(const CCharString&, bool, CTCParamsBase*) 0x004C9D60,
// CTCMultiArrow::SetAICast(long) 0x007CBF20.
// Shape: an unrotated top-test loop (the __forceinline early-return LacksTC condition stops VC rotating it) re-testing
// presence bit 0x88 (dword +0x30, bit 8) of a freshly fetched owner; the final lookup is an inline
// std::lower_bound over the {type, component} vector at +0x44/+0x48 (8-byte entries) whose result is unused (the loop keeps it).
// Layout: Level at retail +0x114 (PDB +0x12c); thing byte +0x10 = state (1: AI controlled).
class CTCBase;
class CTCParamsBase;

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};

class CTCMultiArrow { public:
    void SetAICast(long level);                 // 0x007CBF20
};

struct STCEntry { long type; CTCBase* component; };

class CThing { public:
    unsigned char _pad00[0x10];
    unsigned char m_state;                      // +0x10
    unsigned char _pad11[0x0f];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    STCEntry*     m_tcFirst;                    // +0x44
    STCEntry*     m_tcLast;                     // +0x48
    CTCBase* AddTC(const CCharString& name, bool a2, CTCParamsBase* params);   // 0x004C9D60
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    __forceinline bool LacksTC(long interfaceType) const
    {
        if (HasTC(interfaceType)) return false;
        return true;
    }
    __forceinline STCEntry* LowerBoundTC(long interfaceType)
    {
        STCEntry* first = m_tcFirst;
        long count = m_tcLast - m_tcFirst;
        while (count > 0) {
            long half = count / 2;
            if (first[half].type < interfaceType) {
                first = first + half + 1;
                count = count - half - 1;
            } else
                count = half;
        }
        return first;
    }
};

struct CBaseIntelligentPointer {                // 8 bytes: vptr + item
    virtual void v00();
    CThing* PItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};

class CCombatActionBase { public:
    virtual void v00();
    long _pad04;
    CBaseIntelligentPointer Owner;              // +0x08 (the owning creature)
    unsigned char _pad10[0x114 - 0x10];
    virtual void Initialise();                  // 0x00857520
};

class CCreatureAction_CastSkillMultiArrow : public CCombatActionBase { public:
    long Level;                                 // +0x114 (PDB +0x12c)
    virtual void Initialise();
};

void CCreatureAction_CastSkillMultiArrow::Initialise()
{
    CCombatActionBase::Initialise();
    CThing* owner = Owner.GetPItem();
    while (owner->LacksTC(0x88)) {
        CTCMultiArrow* tc = (CTCMultiArrow*)Owner.GetPItem()->AddTC("CTCMultiArrow", true, 0);
        if (Owner.GetPItem()->m_state == 1)
            tc->SetAICast(Level);
        owner = Owner.GetPItem();
    }
    owner->LowerBoundTC(0x88);
}

// ---- test harness ----
#include <stdio.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)

static int g_baseInit = 0, g_addCalls = 0, g_addNeeded = 1, g_strings = 0;
static long g_aiCast = -1;
static CTCMultiArrow g_tc;
CCharString::CCharString(const char*, long) : PStringData(0) { ++g_strings; }
CCharString::~CCharString() { --g_strings; }
CTCBase* CThing::AddTC(const CCharString&, bool, CTCParamsBase*)
{
    if (++g_addCalls >= g_addNeeded)
        m_presentBits[0x88 >> 5] |= 1UL << (0x88 & 31);
    return (CTCBase*)&g_tc;
}
void CTCMultiArrow::SetAICast(long level) { g_aiCast = level; }
CThing* CBaseIntelligentPointer::GetPItem() const { return PItem; }
void CBaseIntelligentPointer::v00() { }
void CCombatActionBase::v00() { }
void CCombatActionBase::Initialise() { ++g_baseInit; }

int main()
{
    static STCEntry entries[3] = { { 0x10, 0 }, { 0x88, 0 }, { 0x90, 0 } };
    static CThing thing;
    thing.m_tcFirst = entries; thing.m_tcLast = entries + 3;
    static CCreatureAction_CastSkillMultiArrow action;
    action.Owner.PItem = &thing;
    action.Level = 7;
    thing.m_state = 1;
    g_addNeeded = 2;
    action.Initialise();
    CHECK(g_baseInit == 1);
    CHECK(g_addCalls == 2);
    CHECK(g_aiCast == 7);
    CHECK(g_strings == 0);
    CHECK(thing.HasTC(0x88) != 0);
    g_aiCast = -1;
    action.Initialise();                     // already present: no add
    CHECK(g_baseInit == 2);
    CHECK(g_addCalls == 2);
    CHECK(g_aiCast == -1);
    static CThing other;
    other.m_state = 0;
    action.Owner.PItem = &other;
    g_addCalls = 0; g_addNeeded = 1;
    action.Initialise();                     // not AI controlled: add once, no SetAICast
    CHECK(g_addCalls == 1);
    CHECK(g_aiCast == -1);
    if (fails == 0) printf("008cdc40_TEST PASS\n");
    return fails;
}