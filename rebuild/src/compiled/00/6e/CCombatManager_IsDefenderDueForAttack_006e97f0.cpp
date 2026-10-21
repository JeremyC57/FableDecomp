// CCombatManager::IsDefenderDueForAttack(const CCombatantInfo&) const -- a defender may be attacked now only when it is attackable, none of its other attackers is already running a melee or ranged sequence, and its attack pattern's current entry is an attack; each refusal builds (and drops) a "<def name> - not attack - based on : ..." trace string
// Leaf: ?IsDefenderDueForAttack@CCombatManager@@QBE_NABUCCombatantInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E97F0, 358 bytes (368 with padding). `ret 4`.
// Callees: `__fastcall` IsAbleToBeAttacked(const CThingCreatureBase&) 0x0085FB30; CThing::GetDefName 0x004C7CC0
//          (CDefString by hidden pointer), CDefStringTable::GetString 0x009D49B0 on the table 0x013CA828,
//          `__fastcall` operator+(const CCharString&, const char*) 0x0099F600, ~CCharString 0x0099EAE0; the attacker
//          map's out-of-line iterator increment 0x006EF790 and the combatant map's find 0x006F0250; the sequence's
//          virtual slots 7 (+0x1c) and 8 (+0x20). Literals " - not attack - based on : mode" 0x01261E28,
//          "... : attack patterns" 0x01261DFC, "... : other attackers" 0x01261DD0.
//
// Shape: each trace is a block with the looked-up name and the message as NAMED locals handed to an inline no-op
// (the debug output is compiled out but its argument is still built and destroyed): the name lands in the dead
// argument slot and is passed by `lea`, and the message keeps its own frame slot (a temporary message lets VC7.1
// share it with the CDefString temporary, giving an 8-byte frame). The other-attackers walk is a __forceinline
// predicate, so its refusal block sits after the loop (iterator in the dead argument slot, increment out of line)
// and is cross-jumped with the attack-pattern block; each attacker's UniqueID goes to the inline GetCombatantInfo
// (find, no end test, as the landed 0x006E9E40). The final test is `<= 0` so `return true` is the last block.
// Layout: the retail CCombatantInfo of the landed rows (0x006E86A0, 0x006E9E40): Parent +0, CurrentSequence +0x24,
// CombatSequences +0x28, DefenderInfo.Attackers {header +0x34, count +0x38}, PAttackPattern +0x64,
// AttackPatternIndex +0x68; CAttackPatternDef::AttackPattern +0x3c; CCombatManager::Combatants +0x10.
struct CCharString {
    void* m_pData;
    ~CCharString();                                // 0x0099EAE0
};
CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600
struct CDefString {
    long m_index;
};
struct CDefStringTable {
    CCharString GetString(CDefString name);        // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;           // 0x013CA828
inline void CombatTrace(const CCharString&) {}     // the trace output, compiled out

struct CThingCreatureBase {
    unsigned char _pad[0x18];
    unsigned __int64 UniqueID;                     // +0x18
    CDefString GetDefName() const;                 // 0x004C7CC0 (CThing::GetDefName)
    unsigned __int64 GetUID() const { return UniqueID; }
};
bool __fastcall IsAbleToBeAttacked(const CThingCreatureBase& creature);   // 0x0085FB30

struct CCombatSequenceBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06();
    virtual bool IsMeleeSequence();                // slot 7 (+0x1c)
    virtual bool IsRangedSequence();               // slot 8 (+0x20)
    bool Running;                                  // +0x04
};
struct CSequencePointer {                          // CCountedPointer<CCombatSequenceBase>
    CCombatSequenceBase* Object;
    void* Count;
};
template<class T> struct CRetailVector {           // retail std::vector: {first, last, end_of_storage}, 12 bytes
    T* first;
    T* last;
    T* end_of_storage;
    T& operator[](unsigned long n) const { return first[n]; }
};
struct CAttackPatternDef {
    unsigned char _pad00[0x3c];
    CRetailVector<long> AttackPattern;             // +0x3c (PDB +0x40)
};
struct SAttackerNode {                             // STLport node of map<unsigned __int64, CThingCreatureBase*>
    long colour; SAttackerNode* parent; SAttackerNode* left; SAttackerNode* right;
    unsigned __int64 first;                        // +0x10
    CThingCreatureBase* second;                    // +0x18
};
struct CAttackerIterator {
    SAttackerNode* _M_node;
    CAttackerIterator(SAttackerNode* node) : _M_node(node) {}
    bool operator!=(const CAttackerIterator& o) const { return _M_node != o._M_node; }
    CAttackerIterator& operator++();               // 0x006EF790
    SAttackerNode* operator->() const { return _M_node; }
};
struct CAttackerMap {                              // map<unsigned __int64, CThingCreatureBase*> (STLport, 12 bytes)
    SAttackerNode* _M_header;
    unsigned long  _M_node_count;
    unsigned long  _M_compare;
    unsigned long size() const { return _M_node_count; }
    CAttackerIterator begin() const { return CAttackerIterator(_M_header->left); }
    CAttackerIterator end() const { return CAttackerIterator(_M_header); }
};
struct CAttackingCombatantInfo {                   // retail 0x30 bytes
    CThingCreatureBase* Attacked;                  // +0x00
    long AttackerNumber;                           // +0x04
    long CombatWheelRingIndex;                     // +0x08
    bool PreferredAttackDollySet;                  // +0x0c
    long PreferredAttackDollyTimer;                // +0x10
    float PreferredAttackDollyVector[2];           // +0x14
    bool UsingCombatWheel;                         // +0x1c
    long CurrentSequence;                          // +0x20
    CRetailVector<CSequencePointer> CombatSequences;   // +0x24
};
struct CCombatantInfo {                            // retail layout (Ego_r.pdb names; 12-byte vectors/maps)
    CThingCreatureBase* Parent;                    // +0x00
    CAttackingCombatantInfo AttackerInfo;          // +0x04
    CAttackerMap Attackers;                        // +0x34 DefenderInfo.Attackers
    unsigned char CombatWheel[0x0c];               // +0x40
    float LastVictimWheelUpdatePos[3];             // +0x4c
    long LastWheelUpdateWorldFrame;                // +0x58
    unsigned char PLastAttacker[8];                // +0x5c CIntelligentPointer<CThing>
    CAttackPatternDef* PAttackPattern;             // +0x64 CDefPointer<const CAttackPatternDef>
    long AttackPatternIndex;                       // +0x68
    long FrameForNextInstruction;                  // +0x6c
};
struct CRetailRbMap_UID_CombatantInfo {            // STLport map<unsigned __int64, CCombatantInfo>
    struct node {
        long  colour; node* parent; node* left; node* right;
        unsigned __int64 key;                      // +0x10
        CCombatantInfo value;                      // +0x18
    };
    node* header;
    unsigned long count;
    unsigned long comp;
    node* find(const unsigned __int64& key) const; // 0x006F0250
};
struct CCombatManager {
    virtual void v00();
    unsigned char _sequences[0xc];                 // +0x04 RegisteredCombatSequences
    CRetailRbMap_UID_CombatantInfo Combatants;     // +0x10
    const CCombatantInfo& GetCombatantInfo(unsigned __int64 uid) const { return Combatants.find(uid)->value; }
    __forceinline bool IsAnotherAttackerAttacking(const CCombatantInfo& info) const
    {
        if (info.Attackers.size() > 1) {
            for (CAttackerIterator it = info.Attackers.begin(); it != info.Attackers.end(); ++it) {
                const CCombatantInfo& attacker = GetCombatantInfo(it->second->GetUID());
                if (attacker.AttackerInfo.CurrentSequence >= 0) {
                    CCombatSequenceBase* sequence = attacker.AttackerInfo.CombatSequences[attacker.AttackerInfo.CurrentSequence].Object;
                    if ((sequence->IsMeleeSequence() || sequence->IsRangedSequence()) && sequence->Running)
                        return true;
                }
            }
        }
        return false;
    }
    bool IsDefenderDueForAttack(const CCombatantInfo& info) const;
};

bool CCombatManager::IsDefenderDueForAttack(const CCombatantInfo& info) const
{
    if (!IsAbleToBeAttacked(*info.Parent)) {
        {
            CCharString name = g_DefStringTable.GetString(info.Parent->GetDefName());
            CCharString message = name + " - not attack - based on : mode";
            CombatTrace(message);
        }
        return false;
    }
    if (IsAnotherAttackerAttacking(info)) {
        {
            CCharString name = g_DefStringTable.GetString(info.Parent->GetDefName());
            CCharString message = name + " - not attack - based on : other attackers";
            CombatTrace(message);
        }
        return false;
    }
    if (info.PAttackPattern->AttackPattern[info.AttackPatternIndex] <= 0) {
        {
            CCharString name = g_DefStringTable.GetString(info.Parent->GetDefName());
            CCharString message = name + " - not attack - based on : attack patterns";
            CombatTrace(message);
        }
        return false;
    }
    return true;
}