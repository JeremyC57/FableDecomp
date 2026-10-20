// CCombatManager::DeregisterAttackedCombatant(CThingCreatureBase* attacked) -- look the creature's combatant record up; while it still has attackers, deregister each (the record goes once the last attacker leaves, so the function returns then); with none, clear its last attacker, attack pattern and index, drop its being-attacked bit and, when it is no longer a registered combatant, erase its record
// Grade: DIFFER(242v242) -- frame packing + register swap: retail shares one 8-byte frame slot between both UID key temporaries and the CIntelligentPointer temporary (8-byte frame) and keeps `attacked` in ebp / `this` in ebx; ours gives the pointer temporary its own slot (0x10 frame, all offsets +8) with `this` in ebp / `attacked` in ebx; the def release matches with a pointee Release() (DecRefToZero on the member loads the def into eax); tried a reference and a pointer for the record
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
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
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
class CCombatManager {
public:
    virtual void v00();
    unsigned char _sequences[0x0c];                    // +0x04 RegisteredCombatSequences
    CCombatantMap Combatants;                          // +0x10
    void DeregisterAttackingCombatant(CThingCreatureBase* attacker);   // 0x006ED510
    void DeregisterAttackedCombatant(CThingCreatureBase* attacked);
};

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
