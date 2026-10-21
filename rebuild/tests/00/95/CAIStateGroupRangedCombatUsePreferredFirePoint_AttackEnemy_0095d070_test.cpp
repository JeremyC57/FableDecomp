// Behaviour test for 0095d070: src/0095d070.cpp copied verbatim, the readiness test, positions and the combat pump stubbed
// so every exit is observable. CAIStateGroup_RangedCombatUsePreferredFirePoint::AttackEnemy()
#include <cstdio>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CAIStateGroup_RangedCombatUsePreferredFirePoint::AttackEnemy() -- when the creature may attack and stands within
// 0.05 (squared 0.0025) of its fire point in the XY plane, pump the combat actions; false when they report 1.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095D070, length 108 bytes (+ int3 padding).
// Callees: the creature's attack-readiness test @ 0x00662410 (bool; manifest "C3DAnimationBlendState::
// AddCombinationBlend" is propagated), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 on PFirePoint at +0x1c,
// CThing::GetPosition @ 0x004C73D0 (fire point, then creature), CAIStateGroupBase::AIAction_PumpCombatActions
// @ 0x0097D980. Global: the squared fire-point tolerance float @ 0x0129531C (0.0025f).
// Shape (retry 10b): three plain early returns (`if (!CanAttackNow()) return false; if (IsAwayFromFirePoint()) return
// false; if (Pump() == 1) return false; return true;`); the creature is read through an inline GetCreature() accessor
// (PBrain->PCreature) at both uses -- the accessor keeps the first `xor al,al` exit after the first test and gives the
// second PBrain load ecx as retail; the 2D delta (creature - fire point) is built on the x87 stack by a by-value
// `operator-` and `X*X + Y*Y` compared with `fcomp [g] ; test ah,41h` (greater-than).
// PFirePoint is the engine header name (the header flattens the base so the class is modelled locally).
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
struct CTCBase;
struct CThing;
struct CWorld;
struct CThingAICreature;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020 -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const     // the masked word (a bool return narrows to `shr ; test dl,1`)
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void InitStateGroup();              // slot 13 (+0x34)
    CAIBrain* PBrain;                           // +0x04
    CThing* GetCreature() const { return PBrain->PCreature; }
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CWorld* GetPWorld();                        // 0x008FCFC0 (PBrain->+0x1c->+0x24; manifest "CPlayer::GetCurrentMode" is propagated)
    unsigned long* GetPWorldSeed();             // 0x008FCFD0 (world random seed)
};

long __fastcall GFGetCurrentFrame();            // 0x0049D870 (manifest "GFGetPreMainMemoryUsed" is propagated)
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector operator-(const C2DVector& o) const { return C2DVector(X - o.X, Y - o.Y); }
    float MagnitudeSquared() const { return X * X + Y * Y; }
};

struct CThingPos {
    bool CanAttackNow();                        // 0x00662410
    const C2DVector& GetPosition() const;       // 0x004C73D0 (the X/Y head of the C3DVector)
};

extern const float g_firePointToleranceSquared;    // 0x0129531C (0.0025f)

struct CAIStateGroupBaseC : CAIStateGroupBase {
    long AIAction_PumpCombatActions();          // 0x0097D980
};

struct CAIStateGroup_RangedCombatUsePreferredFirePoint : CAIStateGroupBaseC {
    CBaseIntelligentPointer PEnemy;             // +0x14 CIntelligentPointer<CThingCreatureBase>
    CBaseIntelligentPointer PFirePoint;         // +0x1c CIntelligentPointer<CThing>
    long TimeOutCounter;                        // +0x24
    bool AttackEnemy();
    // the creature is a named local read before the fire-point position; the 2D delta is built by the by-value
    // `operator-` and `X*X + Y*Y` compared with the tolerance
    __forceinline bool IsAwayFromFirePoint()
    {
        CThingPos* creature = (CThingPos*)GetCreature();
        const C2DVector& firePos = ((CThingPos*)PFirePoint.GetPItem())->GetPosition();
        return (creature->GetPosition() - firePos).MagnitudeSquared() > g_firePointToleranceSquared;
    }
};

bool CAIStateGroup_RangedCombatUsePreferredFirePoint::AttackEnemy()
{
    if (!((CThingPos*)GetCreature())->CanAttackNow())
        return false;
    if (IsAwayFromFirePoint())
        return false;
    if (AIAction_PumpCombatActions() == 1)
        return false;
    return true;
}

// ---- stubs ----
const float g_firePointToleranceSquared = 0.0025f;
static bool g_canAttack = true; static int g_canCalls = 0; static const void* g_canThis = 0;
bool CThingPos::CanAttackNow() { ++g_canCalls; g_canThis = this; return g_canAttack; }
static C2DVector g_creaturePos(0.0f, 0.0f), g_firePos(0.0f, 0.0f); static const void* g_creatureObj = 0;
const C2DVector& CThingPos::GetPosition() const { return this == g_creatureObj ? g_creaturePos : g_firePos; }
static CThing* g_firePoint = 0;
CThing* CBaseIntelligentPointer::GetPItem() const { return g_firePoint; }
static long g_pump = 0; static int g_pumpCalls = 0;
long CAIStateGroupBaseC::AIAction_PumpCombatActions() { ++g_pumpCalls; return g_pump; }
int main()
{
    static unsigned char creature[4], firePoint[4];
    static CAIBrain brain; brain.PCreature = (CThing*)creature; g_creatureObj = creature; g_firePoint = (CThing*)firePoint;
    static unsigned char raw[sizeof(CAIStateGroup_RangedCombatUsePreferredFirePoint)];
    CAIStateGroup_RangedCombatUsePreferredFirePoint* g = (CAIStateGroup_RangedCombatUsePreferredFirePoint*)raw; g->PBrain = &brain;
    // cannot attack
    g_canAttack = false; CHECK(g->AttackEnemy() == false); CHECK(g_canThis == creature && g_pumpCalls == 0);
    // away from the fire point (0.06 > 0.05)
    g_canAttack = true; g_creaturePos = C2DVector(1.06f, 2.0f); g_firePos = C2DVector(1.0f, 2.0f);
    CHECK(g->AttackEnemy() == false); CHECK(g_pumpCalls == 0);
    // at the fire point: the pump result decides
    g_creaturePos = C2DVector(1.03f, 2.0f); g_pump = 0;
    CHECK(g->AttackEnemy() == true); CHECK(g_pumpCalls == 1);
    g_pump = 1; CHECK(g->AttackEnemy() == false); CHECK(g_pumpCalls == 2);
    g_pump = 2; CHECK(g->AttackEnemy() == true);
    if (fails == 0) printf("0095d070_TEST PASS\n"); else printf("0095d070_TEST FAIL\n");
    return fails;
}