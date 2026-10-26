// CAIStateGroup_StealthSeekEnemy::Cleanup() -- reset the creature's interface-0x45 state (mode 8, full weight, 0xFF),
// and if the enemy is still alive with interface 0x36, tell that component the creature stopped seeking it.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0093C420, length 180 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (two inlined lookups: type 0x45, bit 5 of the dword at +0x28; type 0x36,
// bit 22 of +0x24), the 0x45 component's setter @ 0x0071AAB0 (8, 0, 0, 1.0f, 0xFF), CBaseIntelligentPointer::GetPItem
// @ 0x00A01B50 (IS_THING_ALIVE, then the thing), the 0x36 component's RemoveSeeker @ 0x00747B80.
// Shape: both lookups are the out-parameter GetInterface whose entry pick is one end/hit ternary (the 0x45 component
// lands in eax, `mov ecx,eax` after the pushes; the 0x36 component loads straight into ecx between the two brain
// loads); IS_THING_ALIVE inlined. The engine header flattens the base, so the class is modelled locally; PEnemy is +0x14.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CThing;
class CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x004365B0.
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x004365B0
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual bool IsStillValid();                // slot 5  (+0x14)
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void InitStateGroup();              // slot 13 (+0x34)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CBaseIntelligentPointerCtor {            // CBaseIntelligentPointer (8 bytes: vptr + item)
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointerCtor();              // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointerCtor();             // 0x00A01C10
};

// `PThing = thing` is the converting-temporary + inline `SetPItem(other.GetPItem())` assignment.
struct CIntelligentPointerThing : CBaseIntelligentPointerCtor {
    __forceinline CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointerCtor() { SetPItem(item); }
    __forceinline CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointerCtor& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CTCStealthState {                        // the interface-0x45 component
    void SetState(long mode, long a, long b, float weight, long alpha);   // 0x0071AAB0
};
struct CTCSeekTarget {                          // the interface-0x36 component
    void RemoveSeeker(CThing* seeker, long flags);  // 0x00747B80
};

struct CAIStateGroup_StealthSeekEnemy : CAIStateGroupBase {
    CIntelligentPointerThing PEnemy;            // +0x14
    virtual void Cleanup();
};

void CAIStateGroup_StealthSeekEnemy::Cleanup()
{
    CTCStealthState* stealth;
    if (PBrain->PCreature->GetInterface(&stealth, 0x45))
        stealth->SetState(8, 0, 0, 1.0f, 0xff);
    if (IS_THING_ALIVE(PEnemy)) {
        CThing* enemy = PEnemy.GetPItem();
        CTCSeekTarget* seek;
        if (enemy->GetInterface(&seek, 0x36))
            seek->RemoveSeeker(PBrain->PCreature, 0);
    }
}
// ---- harness ----
#include <cstdio>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    while (it != last && it->type < *type)
        ++it;
    return it;
}
void CBaseIntelligentPointerCtor::v00() { }
CBaseIntelligentPointerCtor::CBaseIntelligentPointerCtor() { m_pItem = 0; }
CThing* CBaseIntelligentPointerCtor::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointerCtor::SetPItem(CThing* item) { m_pItem = item; }
CBaseIntelligentPointerCtor::~CBaseIntelligentPointerCtor() { }
static int g_removes = 0; static CThing* g_seeker = 0; static long g_flags = -1; static const void* g_seekThis = 0;
void CTCSeekTarget::RemoveSeeker(CThing* seeker, long flags) { ++g_removes; g_seeker = seeker; g_flags = flags; g_seekThis = this; }
static int g_states = 0; static long g_mode = 0, g_a = -1, g_b = -1, g_alpha = 0; static float g_weight = 0; static const void* g_stealthThis = 0;
void CTCStealthState::SetState(long mode, long a, long b, float weight, long alpha)
{ ++g_states; g_mode = mode; g_a = a; g_b = b; g_weight = weight; g_alpha = alpha; g_stealthThis = this; }
static CThing g_enemy, g_me;
static CAIBrain g_brain;
static CTCSeekTarget g_seek;
static CTCStealthState g_stealth;
static CInterfaceMap::SEntry g_enemyEntries[2], g_myEntries[2];
static long g_mem[(sizeof(CAIStateGroup_StealthSeekEnemy) + 3) / 4];      // the intelligent-pointer member has no default constructor
int main()
{
    g_enemyEntries[0].type = 0x10; g_enemyEntries[0].component = 0;
    g_enemyEntries[1].type = 0x36; g_enemyEntries[1].component = (CTCBase*)&g_seek;
    g_enemy.m_interfaces.first = g_enemyEntries; g_enemy.m_interfaces.last = g_enemyEntries + 2;
    g_enemy.m_presentBits[0x36 >> 5] = 1UL << (0x36 & 31);
    g_myEntries[0].type = 0x45; g_myEntries[0].component = (CTCBase*)&g_stealth;
    g_myEntries[1].type = 0x50; g_myEntries[1].component = 0;
    g_me.m_interfaces.first = g_myEntries; g_me.m_interfaces.last = g_myEntries + 2;
    g_me.m_presentBits[0x45 >> 5] = 1UL << (0x45 & 31);
    g_brain.PCreature = &g_me;
    CAIStateGroup_StealthSeekEnemy* s = (CAIStateGroup_StealthSeekEnemy*)g_mem;
    s->PBrain = &g_brain;
    s->PEnemy.m_pItem = &g_enemy;
    s->CAIStateGroup_StealthSeekEnemy::Cleanup();
    CHECK(g_states == 1 && g_mode == 8 && g_a == 0 && g_b == 0 && g_weight == 1.0f && g_alpha == 0xff && g_stealthThis == &g_stealth);
    CHECK(g_removes == 1 && g_seeker == &g_me && g_flags == 0 && g_seekThis == &g_seek);
    g_me.m_presentBits[0x45 >> 5] = 0; g_enemy.m_flags91 = 1;   // no stealth component, enemy dead
    s->CAIStateGroup_StealthSeekEnemy::Cleanup();
    CHECK(g_states == 1 && g_removes == 1);
    g_enemy.m_flags91 = 0; g_enemy.m_presentBits[0x36 >> 5] = 0;   // enemy without a seek component
    s->CAIStateGroup_StealthSeekEnemy::Cleanup();
    CHECK(g_removes == 1);
    s->PEnemy.m_pItem = 0;
    s->CAIStateGroup_StealthSeekEnemy::Cleanup();
    CHECK(g_removes == 1 && g_states == 1);
    if (fails == 0) printf("0093c420_TEST PASS\n");
    return fails;
}