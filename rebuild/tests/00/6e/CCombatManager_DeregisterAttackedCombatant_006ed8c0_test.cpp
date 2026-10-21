// Behaviour test for 006ed8c0: src/006ed8c0.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CCombatManager::DeregisterAttackedCombatant(CThingCreatureBase* attacked)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CCombatManager::DeregisterAttackedCombatant(CThingCreatureBase* attacked) -- look the creature's combatant record up; while it still has attackers, deregister each (the record goes once the last attacker leaves, so the function returns then); with none, clear its last attacker, attack pattern and index, drop its being-attacked bit and, when it is no longer a registered combatant, erase its record
// Leaf: ?DeregisterAttackedCombatant@CCombatManager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006ED8C0, length 242 bytes (`ret 4`).
// Callees: Combatants.find 0x006F0250 / erase(key) 0x006F50B0 (STLport map<unsigned __int64, CCombatantInfo>),
//          DeregisterAttackingCombatant 0x006ED510, CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor (PLastAttacker = 0
//          through a CIntelligentPointer<CThing> temporary, vtable 0x01238C6C), the attack-pattern def's slot 1,
//          CThingCreatureBase::IsRegisteredCombatant 0x00662410.
// Layout: the landed CCombatManager_Retail model (Combatants +0x10, node key +0x10, info +0x18); retail CCombatantInfo has a
//          0x30-byte attacker part, so DefenderInfo sits at +0x34 (attackers map, PLastAttacker +0x28, PAttackPattern +0x30,
//          AttackPatternIndex +0x34); the being-attacked flag is bit 3 of creature +0x15c.
struct CThing;
struct CThingCreatureBase {
    unsigned char    _pad[0x18];
    unsigned __int64 UniqueID;                         // +0x18
    unsigned char    _pad20[0x15c - 0x20];
    unsigned char    _bits15c_0 : 3;
    unsigned char    IsBeingAttacked : 1;              // +0x15c bit 3
    unsigned char    _bits15c_4 : 4;
    unsigned __int64 GetUID() const { return UniqueID; }
    bool IsRegisteredCombatant();                      // 0x00662410
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                   // +0x04
    CBaseIntelligentPointer();                         // 0x00A01B10
    ~CBaseIntelligentPointer();                        // 0x00A01C10
    CThing* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CThing* item);                       // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};
struct CDefVirtual {
    virtual ~CDefVirtual();                            // slot 0
    virtual void Destroy();                            // slot 1
};
struct CAttackPatternDef : CDefVirtual {
    long RefCount;                                     // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
__forceinline bool DecRefToZero(long& count) { return --count == 0; }
template <class T> struct CDefPointer {
    T* m_p;
    CDefPointer& operator=(T* p)
    {
        if (m_p) {
            m_p->Release();
            m_p = 0;
        }
        m_p = p;
        return *this;
    }
};
struct SAttackerNode {                                 // map<unsigned __int64, CThingCreatureBase*> node
    char             _M_color;
    SAttackerNode*   _M_parent;
    SAttackerNode*   _M_left;
    SAttackerNode*   _M_right;
    unsigned __int64 first;                            // +0x10
    CThingCreatureBase* second;                        // +0x18
};
struct CAttackerMap {                                  // STLport map (12 bytes)
    SAttackerNode* _M_header;
    unsigned long  _M_node_count;
    unsigned long  _M_key_compare;
    unsigned long size() const { return _M_node_count; }
    SAttackerNode* begin() const { return _M_header->_M_left; }
};
struct CDefendingCombatantInfo {                       // retail 0x40 bytes
    CAttackerMap  Attackers;                           // +0x00
    unsigned char CombatWheel[0x0c];                   // +0x0c
    float         LastVictimWheelUpdatePos[3];         // +0x18
    long          LastWheelUpdateWorldFrame;           // +0x24
    CIntelligentPointerThing PLastAttacker;            // +0x28
    CDefPointer<CAttackPatternDef> PAttackPattern;     // +0x30
    long          AttackPatternIndex;                  // +0x34
    long          FrameForNextInstruction;             // +0x38
};
struct CCombatantInfo {
    CThingCreatureBase* Parent;                        // +0x00
    unsigned char       AttackerInfo[0x30];            // +0x04 CAttackingCombatantInfo (retail 0x30)
    CDefendingCombatantInfo DefenderInfo;              // +0x34
};
struct SCombatantNode {
    char             _M_color;
    SCombatantNode*  _M_parent;
    SCombatantNode*  _M_left;
    SCombatantNode*  _M_right;
    unsigned __int64 first;                            // +0x10
    CCombatantInfo   second;                           // +0x18
};
struct CCombatantMap {
    SCombatantNode* _M_header;
    unsigned long   _M_node_count;
    unsigned long   _M_key_compare;
    SCombatantNode* find(const unsigned __int64& key);     // 0x006F0250
    void erase(const unsigned __int64& key);               // 0x006F50B0
};
long TmplSink_s7_1(const void* self, long seen);   // stand-in: the work erase does (outside the bytes graded here)
#pragma auto_inline(off)
// Same-TU stand-in for the erase instantiation (template bodies are compiled in the caller's TU): reads the const-reference
// arguments (not the empty tags), stores the out pointers, lets no argument escape.
__declspec(noinline) void CCombatantMap::erase(const unsigned __int64& key)
{
    long r = TmplSink_s7_1(this, 0 + *(const long*)&key);
    (void)r;
}
#pragma auto_inline(on)
long TmplSink_s7_0(const void* self, long seen);   // stand-in: the work find does (outside the bytes graded here)
#pragma auto_inline(off)
// Same-TU stand-in for the find instantiation (template bodies are compiled in the caller's TU): reads the const-reference
// arguments (not the empty tags), stores the out pointers, lets no argument escape.
__declspec(noinline) SCombatantNode* CCombatantMap::find(const unsigned __int64& key)
{
    long r = TmplSink_s7_0(this, 0 + *(const long*)&key);
    return (SCombatantNode*)r;
}
#pragma auto_inline(on)
class CCombatManager {
public:
    virtual void v00();
    unsigned char _sequences[0x0c];                    // +0x04 RegisteredCombatSequences
    CCombatantMap Combatants;                          // +0x10
    void DeregisterAttackingCombatant(CThingCreatureBase* attacker);   // 0x006ED510
    void DeregisterAttackedCombatant(CThingCreatureBase* attacked);
};
long NearSink_s7_0(const void* self, long seen);   // stand-in: the work DeregisterAttackingCombatant does outside this TU
#pragma auto_inline(off)
// Same-TU stand-in for CCombatManager::DeregisterAttackingCombatant @ 0x006ED510 (compiled in retail's TU): reads its const reference / pointer-to-const
// arguments, updates its out-parameters, and lets no argument escape.
__declspec(noinline) void CCombatManager::DeregisterAttackingCombatant(CThingCreatureBase* attacker)
{
    NearSink_s7_0(this, (long)attacker);
}
#pragma auto_inline(on)

void CCombatManager::DeregisterAttackedCombatant(CThingCreatureBase* attacked)
{
    CCombatantInfo* info = &Combatants.find(attacked->GetUID())->second;
    long count = info->DefenderInfo.Attackers.size();
    while (count > 0) {
        DeregisterAttackingCombatant(info->DefenderInfo.Attackers.begin()->second);
        --count;
        if (count == 0)
            return;
    }
    info->DefenderInfo.PLastAttacker = 0;
    info->DefenderInfo.PAttackPattern = 0;
    info->DefenderInfo.AttackPatternIndex = -1;
    attacked->IsBeingAttacked = 0;
    if (!attacked->IsRegisteredCombatant())
        Combatants.erase(attacked->GetUID());
}

// ---- stubs and driver ----
void CBaseIntelligentPointer::v00() {}
void CIntelligentPointerThing::v00() {}
void CCombatManager::v00() {}
static int g_ipCtor = 0, g_ipDtor = 0;
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem(0) { ++g_ipCtor; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++g_ipDtor; }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
void CBaseIntelligentPointer::SetPItem(CThing* item) { m_pItem = item; }
static int g_destroyed = 0;
CDefVirtual::~CDefVirtual() {}
void CDefVirtual::Destroy() { ++g_destroyed; }
static bool g_registered = false;
bool CThingCreatureBase::IsRegisteredCombatant() { return g_registered; }
static SCombatantNode* g_found = 0; static long g_findKey = 0; static long g_eraseKey = 0; static int g_erase = 0;
long TmplSink_s7_0(const void* self, long seen) { g_findKey = seen; return (long)g_found; }
long TmplSink_s7_1(const void* self, long seen) { ++g_erase; g_eraseKey = seen; return 0; }
static long g_deregistered[8]; static int g_nDereg = 0;
long NearSink_s7_0(const void* self, long seen) { g_deregistered[g_nDereg++] = seen; return 0; }
int main()
{
    static unsigned char nodeStore[sizeof(SCombatantNode)]; SCombatantNode& node = *(SCombatantNode*)nodeStore;
    static SAttackerNode header, a1; header._M_left = &a1;
    static CThingCreatureBase attacked, attacker; attacked.UniqueID = 0x123456789ULL; attacked.IsBeingAttacked = 1; attacked._bits15c_0 = 5; attacked._bits15c_4 = 9;
    a1.second = &attacker;
    node.second.DefenderInfo.Attackers._M_header = &header; node.second.DefenderInfo.Attackers._M_node_count = 2;
    g_found = &node;
    static CCombatManager mgr;
    mgr.DeregisterAttackedCombatant(&attacked);                   // two attackers: each deregistered, then return
    CHECK(g_findKey == 0x23456789); CHECK(g_nDereg == 2); CHECK(g_deregistered[0] == (long)&attacker); CHECK(g_deregistered[1] == (long)&attacker);
    CHECK(attacked.IsBeingAttacked == 1); CHECK(g_erase == 0); CHECK(g_ipCtor == 0);
    CAttackPatternDef pattern; pattern.RefCount = 1;
    node.second.DefenderInfo.Attackers._M_node_count = 0; node.second.DefenderInfo.PLastAttacker.m_pItem = (CThing*)&attacker;
    node.second.DefenderInfo.PAttackPattern.m_p = &pattern; node.second.DefenderInfo.AttackPatternIndex = 3;
    g_registered = true;
    mgr.DeregisterAttackedCombatant(&attacked);                   // no attackers: clear the defender state, still registered
    CHECK(g_nDereg == 2); CHECK(node.second.DefenderInfo.PLastAttacker.m_pItem == 0); CHECK(g_ipCtor == 1); CHECK(g_ipDtor == 1);
    CHECK(pattern.RefCount == 0); CHECK(g_destroyed == 1); CHECK(node.second.DefenderInfo.PAttackPattern.m_p == 0);
    CHECK(node.second.DefenderInfo.AttackPatternIndex == -1); CHECK(attacked.IsBeingAttacked == 0);
    CHECK(attacked._bits15c_0 == 5); CHECK(attacked._bits15c_4 == 9); CHECK(g_erase == 0);
    g_registered = false; node.second.DefenderInfo.PAttackPattern.m_p = 0;
    mgr.DeregisterAttackedCombatant(&attacked);                   // no longer a combatant: record erased
    CHECK(g_erase == 1); CHECK(g_eraseKey == 0x23456789); CHECK(g_destroyed == 1);
    if (fails == 0) printf("006ed8c0_TEST PASS\n"); else printf("006ed8c0_TEST FAIL\n");
    return fails;
}