// Behaviour test for 008b8bc0: src/008b8bc0.cpp copied verbatim; positions, the combat-type def and the animation
// setter stubbed so the advance animation (target beyond the front-near limit in the ground plane), the plain one
// and the base Initialise call are checked.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CCreatureAction_ScreamerDrainAttack::Initialise() -- pick the drain-life cast animation: ST_CAST_DRAIN_LIFE_ADVANCE when the live target is farther (in the ground plane) than the creature's combat-type front-near limit, else ST_CAST_DRAIN_LIFE; then the combat-action Initialise
// Retry 10b (chain-split sweep, FIXES B-16): the creature is a named local (`CThing* creature = PCreature.GetPItem();`) before its position is taken: retail keeps `this` in ebp and the target in ebx (the chained call swaps them).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B8BC0, length 240 bytes (plain `ret`, virtual).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PTarget +0xA8 three times, PCreature +8 twice), CThing::GetPosition
// 0x004C73D0 (creature first, then target), CThingCreatureBase::GetCombatTypeDef 0x00666D10, CCharString(const char*, long)
// 0x0099EBF0 / ~CCharString 0x0099EAE0, CCombatActionBase::SetAnimationName(const CCharString&) 0x008574D0,
// CCombatActionBase::Initialise 0x00857520 (direct base call, last).
// Layout: the target pointer is the CCreatureActionBase member at +0xA8; the combat-type def's FrontNearLimit sits at retail
// +0x80 (PDB +0x84: def layouts are 4 bytes smaller in retail). The PDB header for this class flattens its base.
// Shape: the XY delta (target - creature) is a two-float local; the limit is read before the squared length, which is
// evaluated right operand first (Y*Y, then X*X); `limit * limit < lengthSquared` is the `test ah,5 ; jp` ordered
// less-than; the two animation names are if/else blocks whose SetAnimationName call and string destructor tail-merge.
class C3DVector { public:
    float X, Y, Z;
    float GetMagnitudeSquared2D() const { return X * X + Y * Y; }
};

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

class CCombatTypeDef { public:
    unsigned char _pad0[0x80];
    float         FrontNearLimit;               // +0x80
};

class CThing { public:
    unsigned char _pad0[0x91];
    unsigned char m_deleteFlags;                // +0x91 bit 0: being deleted
    const C3DVector& GetPosition() const;       // 0x004C73D0
    CCombatTypeDef* GetCombatTypeDef();         // 0x00666D10 (CThingCreatureBase)
};

struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

class CCombatActionBase { public:
    virtual void v00();
    virtual void Initialise();                  // 0x00857520
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CBaseIntelligentPointer PTarget;            // +0xa8
    unsigned char           _padb0[0x114 - 0xb0];
    void SetAnimationName(const CCharString& name);   // 0x008574D0
};

class CCreatureAction_ScreamerDrainAttack : public CCombatActionBase { public:
    virtual void Initialise();
};

void CCreatureAction_ScreamerDrainAttack::Initialise()
{
    bool advance = false;
    C3DVector delta;
    if (PTarget.GetPItem() && !(PTarget.GetPItem()->m_deleteFlags & 1)) {
        CThing* target = PTarget.GetPItem();
        CThing* creature = PCreature.GetPItem();
        const C3DVector& from = creature->GetPosition();
        const C3DVector& to = target->GetPosition();
        delta.X = to.X - from.X;
        delta.Y = to.Y - from.Y;
        float limit = PCreature.GetPItem()->GetCombatTypeDef()->FrontNearLimit;
        if (delta.GetMagnitudeSquared2D() > limit * limit)
            advance = true;
    }
    if (advance) {
        CCharString name("ST_CAST_DRAIN_LIFE_ADVANCE", -1);
        SetAnimationName(name);
    }
    else {
        CCharString name("ST_CAST_DRAIN_LIFE", -1);
        SetAnimationName(name);
    }
    CCombatActionBase::Initialise();
}
// ---- end source ----
void CCombatActionBase::v00() {}
static CThing g_creature, g_target; static C3DVector g_cpos = { 0, 0, 0 }, g_tpos = { 3, 4, 50 };
const C3DVector& CThing::GetPosition() const { return this == &g_creature ? g_cpos : g_tpos; }
static CCombatTypeDef g_ctd;
CCombatTypeDef* CThing::GetCombatTypeDef() { return &g_ctd; }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
CCharString::CCharString(const char* t, long) : m_pData((void*)t) {}
CCharString::~CCharString() {}
static char g_anim[64]; static int g_baseInits = 0;
void CCombatActionBase::SetAnimationName(const CCharString& n) { strcpy(g_anim, (const char*)n.m_pData); }
void CCombatActionBase::Initialise() { ++g_baseInits; }
int main()
{
    static unsigned char raw[sizeof(CCreatureAction_ScreamerDrainAttack)]; memset(raw, 0, sizeof(raw));
    CCreatureAction_ScreamerDrainAttack* a = (CCreatureAction_ScreamerDrainAttack*)raw;
    a->PCreature.m_pItem = &g_creature; a->PTarget.m_pItem = &g_target;
    g_ctd.FrontNearLimit = 4.0f;                                       // 25 > 16: advance
    a->CCreatureAction_ScreamerDrainAttack::Initialise();
    CHECK(strcmp(g_anim, "ST_CAST_DRAIN_LIFE_ADVANCE") == 0 && g_baseInits == 1);
    g_ctd.FrontNearLimit = 6.0f;                                       // 25 < 36
    a->CCreatureAction_ScreamerDrainAttack::Initialise();
    CHECK(strcmp(g_anim, "ST_CAST_DRAIN_LIFE") == 0 && g_baseInits == 2);
    g_ctd.FrontNearLimit = 1.0f; g_target.m_deleteFlags = 1;           // dying target: no advance
    a->CCreatureAction_ScreamerDrainAttack::Initialise();
    CHECK(strcmp(g_anim, "ST_CAST_DRAIN_LIFE") == 0);
    a->PTarget.m_pItem = 0;
    a->CCreatureAction_ScreamerDrainAttack::Initialise();
    CHECK(strcmp(g_anim, "ST_CAST_DRAIN_LIFE") == 0 && g_baseInits == 4);
    if (fails == 0) printf("008b8bc0_TEST PASS\n"); else printf("008b8bc0_TEST FAIL\n");
    return fails;
}