// CDefendingCombatantInfo::InitializeDefenderFromCreature(CCombatantInfo* info) -- resolve the defender's attack-pattern def: the creature's own combat type name becomes a def index; each attacker's combat type def maps (through its attack-pattern map) to the first non-null pattern index; when a pattern differs from the defender's own, fetch that def and reset the defender attack pattern on the combat manager
// Grade: DIFFER(367v360) (87 difflines, was 379v360/103) -- slot 0x28 returns a CDefString by value (non-POD: user copy ctor, so by hidden pointer, no destructor), read inline as `GetDefName().m_index` into GetString, with the index lookup as one expression: drops the CCharString ctor/dtor pair retail never had; residue: retail keeps info in ebx and the zero in ebp (pattern=0 shares it), puts the GetString temporary in the dead info argument slot and the own type in a frame word, ours holds info in ebp with a 0x14 frame; the loop registers rotate (edi/edx) (retry wave 2 sec2b)
// Was: DIFFER(379v360) -- best effort: inline map lower_bound + iterator increment over the attacker set; the retail loop keeps the pattern in ebp and the found node in edi with different test order
// Leaf: InitializeDefenderFromCreature
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006EA0C0, length 358 bytes, `ret 4`.
// Callees: the owner's virtual slot 0x28 (GetDefName into a CCharString*), CDefStringTable::GetString(long) @ 0x009D49B0 on the global table
// 0x013CA828, CDefinitionManager::GetDefGlobalIndexFromName @ 0x009AD410, ~CCharString @ 0x0099EAE0, CThingCreatureBase::GetCombatTypeDef
// @ 0x00666D10, CDefinitionManager::GetDef<...> @ 0x006F0E80 (CDefPointer by hidden pointer), CCombatManager::ResetDefenderAttackPattern
// @ 0x006E8C30 (def pointer by value), the def's virtual slot 1 on the last reference.
// Layout: CCombatantInfo Parent +0, Attackers tree +0x34 (map<uint64, CThingCreatureBase*>, value at node +0x18), CombatManager +0x64;
// CDefendingCombatantInfo Manager +0x20 (DefinitionManager at +0x10); combat type def: AttackPatterns map header +0x84 (nodes keyed by
// long at +0x10, value at +0x14), CombatTypeNameIndex +0x90.
// Shape: inline STLport map lower_bound + iterator increment over the attackers set; `pattern` stays zero until the first match.
struct CCharString {
    void* PStringData;
    CCharString();
    ~CCharString();                                        // 0x0099EAE0
};
struct CDefStringTable {
    CCharString GetString(long index);                     // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;                   // 0x013CA828
struct SPatternNode { long colour; SPatternNode* parent; SPatternNode* left; SPatternNode* right; long key; long value; };
struct CPatternMap {
    SPatternNode* header;
    SPatternNode* find(long key)
    {
        SPatternNode* y = header;
        SPatternNode* x = header->parent;
        while (x) {
            if (!(x->key < key)) { y = x; x = x->left; }
            else x = x->right;
        }
        if (y != header && key < y->key)
            y = header;
        return y;
    }
};
struct CCombatTypeDef {
    unsigned char _pad[0x84];
    CPatternMap AttackPatterns;                            // +0x84
    unsigned char _pad2[0x90 - 0x88];
    long CombatTypeNameIndex;                              // +0x90
};
struct CThingCreatureBase {
    CCombatTypeDef* GetCombatTypeDef();                    // 0x00666D10
};
struct SAttackerNode { long colour; SAttackerNode* parent; SAttackerNode* left; SAttackerNode* right; unsigned __int64 key; CThingCreatureBase* creature; };
struct CAttackerSet { SAttackerNode* header; unsigned long count; };
struct CRefCounted { long count; };
struct CDefVirtual { virtual void v00(); virtual void Destroy(); };
struct CAttackPatternDef : CDefVirtual { long RefCount; };
struct CDefPointer {
    CAttackPatternDef* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
};
struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
    CDefPointer GetDef(long index, long* outIndex);             // 0x006F0E80
};
struct CDefString { long m_index; CDefString(const CDefString& o) : m_index(o.m_index) {} };
struct CCombatManager {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual CDefString GetDefName();                       // slot 0x28 (CDefString by hidden pointer)
    unsigned char _pad[0x10 - 4];
    CDefinitionManager* DefinitionManager;                 // +0x10
    void ResetDefenderAttackPattern(CDefPointer def, void* info);   // 0x006E8C30
};
struct CCombatantInfo {
    CThingCreatureBase* Parent;                            // +0x00
    unsigned char _pad[0x34 - 4];
    CAttackerSet Attackers;                                // +0x34
    unsigned char _pad2[0x64 - 0x3c];
    CCombatManager* CombatManager;                         // +0x64
};
struct CDefendingCombatantInfo {
    unsigned char _pad[0x20];
    CCombatManager* Manager;                               // +0x20
    void InitializeDefenderFromCreature(CCombatantInfo* info);
};

void CDefendingCombatantInfo::InitializeDefenderFromCreature(CCombatantInfo* info)
{
    long pattern = 0;
    long ownIndex = Manager->DefinitionManager->GetDefGlobalIndexFromName(g_DefStringTable.GetString(info->CombatManager->GetDefName().m_index));
    long ownType = info->Parent->GetCombatTypeDef()->CombatTypeNameIndex;
    SAttackerNode* header = info->Attackers.header;
    for (SAttackerNode* it = header->left; it != header; ) {
        CCombatTypeDef* def = it->creature->GetCombatTypeDef();
        SPatternNode* found = def->AttackPatterns.find(ownType);
        if (found != def->AttackPatterns.header && pattern == 0)
            pattern = found->value;
        if (it->right) {
            it = it->right;
            while (it->left) it = it->left;
        } else {
            SAttackerNode* y = it->parent;
            while (it == y->right) { it = y; y = y->parent; }
            if (it->right != y) it = y;
        }
    }
    if (pattern && pattern != ownIndex) {
        CCombatManager* manager = Manager;
        long index = 0;
        CDefPointer def = manager->DefinitionManager->GetDef(pattern, &index);
        manager->ResetDefenderAttackPattern(def, info);
    }
}
