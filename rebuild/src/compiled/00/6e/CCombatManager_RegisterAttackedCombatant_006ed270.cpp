// CCombatManager::RegisterAttackedCombatant(CThingCreatureBase&, CThingCreatureBase&) -- make sure the attacked creature has a combatant record (a fresh one keyed by its UniqueID when missing), add the attacker to its attackers, restart its attack pattern from ATTACK_PATTERN_CONTINUOUS and flag it as attacked
// Leaf: RegisterAttackedCombatant
// Retail: Fable.exe (TLC, VC7.1) @ 0x006ED270, 383 bytes (384 with padding). `ret 8`.
// Callees: the combatant map's find 0x006F0250 (same-TU body below) and insert 0x006F4F30 (pair<iterator, bool> by
//          hidden pointer); CCombatantInfo() 0x006F4A00 (catalogued C3DMeshMaterial) and its copy ctor 0x006F4990;
//          the implicit ~CCombatantInfo (~CDefendingCombatantInfo 0x006F48A0 out of line, the sequence vector's
//          _Destroy_range 0x006F2300 + free 0x00BFEA14 inline); the attackers map's insert 0x006F1EF0;
//          CDefinitionManager::GetDef<CAttackPatternDef>(const CCharString&, CDefPointer&) 0x006F0E10 (same-TU
//          stand-in below) on the world's manager (+0x20, +0x10); ResetDefenderAttackPattern(CCombatantInfo&,
//          CDefPointer) 0x006E8C30. Global: the "ATTACK_PATTERN_CONTINUOUS" string DEFAULT_ATTACK_PATTERN_DEF_NAME (was g_CCharString_013babb0).
//
// Shape: the new record is a named local (Parent and Attacked stored after its ctor) copied into a pair temporary
// for insert; both are destroyed inline, and the insert result is a named pair read after the temporary dies. The def pointer is loaded once after GetDef (the stand-in shows its
// address does not escape), copied by value into the call's argument slot with an add-ref, and released with the
// def's inline Release. The attacked flag is a one-bit field of the creature's byte +0x15c.
// Layout: the landed CCombatManager / CCombatantInfo models (0x006E9D90, 0x006F4E40): Combatants +0x10, World +0x20,
// node value +0x18, DefenderInfo +0x34 (Attackers first).
extern "C" void __cdecl free(void* block);  // 0x00BFEA14
// Name fix (NF207, 2026-11-20, q4389, family FP-COMBATMAP): the invented CRetailRbMap_UID_CombatantInfo / SCombatantValue stand-ins
//   (and the same-TU noinline find body) are replaced in place by the STLport map<unsigned __int64, CCombatantInfo> and
//   pair<const unsigned __int64, CCombatantInfo> (stlport/fable_map.h): _M_find<unsigned __int64> 0x006F0250 visible but out of line
//   (FABLE_RETAIL_STLP_TREE_MFIND_NOINLINE; the pick-any copy is byte- and relocation-identical to the 0x006F0250 row), map::insert
//   forwards to the declared-only _Rb_tree::insert_unique 0x006F4F30 (its self Leaf). The local std::__false_type / __destroy_aux
//   declarations give way to fable_containers.h's FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL_DECL (same __fastcall 0x006F2300 symbol).
#define FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL_DECL
#define FABLE_RETAIL_STLP_TREE_MFIND_NOINLINE
#include "stlport/fable_map.h"

class CCharString { public:
    void* m_pData;
};
namespace { extern CCharString DEFAULT_ATTACK_PATTERN_DEF_NAME; }  // 0x013BABB0 ("ATTACK_PATTERN_CONTINUOUS")

class CDefPointeeBase { public:                    // a counted definition object
    virtual void v000();
    virtual void DestroyDef();              // slot 1 (+4)
    long m_refCount;                        // +4
};
class CAttackPatternDef : public CDefPointeeBase { public: };
struct CAttackPatternPointer {              // CDefPointer<const CAttackPatternDef>
    CAttackPatternDef* Object;
    CAttackPatternPointer() : Object(0) {}
    CAttackPatternPointer(const CAttackPatternPointer& o) : Object(o.Object)
    {
        if (Object)
            ++Object->m_refCount;
    }
    ~CAttackPatternPointer()
    {
        if (Object && --Object->m_refCount == 0)
            Object->DestroyDef();
    }
};
void* FindDef_GetDef_006f0e10();            // stand-in callee of the stand-in body below
class CDefinitionManager { public:
    bool GetDef(const CCharString& name, CAttackPatternPointer& out);   // 0x006F0E10
};
#pragma auto_inline(off)
// Same-TU stand-in for the GetDef<T> instantiation 0x006F0E10: it writes the out slot only inline, so VC7.1 knows the
// def pointer's address does not escape the call (the landed 0x005DA6E0 idiom).
__declspec(noinline) bool CDefinitionManager::GetDef(const CCharString& name, CAttackPatternPointer& out)
{
    void* def = FindDef_GetDef_006f0e10();
    if (!def)
        return false;
    *(void**)&out = def;
    return true;
}
#pragma auto_inline(on)
class CWorld { public:
    unsigned char _pad[0x10];
    CDefinitionManager* DefinitionManager;  // +0x10
};

class CThingCreatureBase { public:
    unsigned char _pad[0x18];
    unsigned __int64 UniqueID;              // +0x18
    unsigned char _pad20[0x13c];
    unsigned char CombatFlags0 : 3;         // +0x15c
    unsigned char IsAttackedCombatant : 1;  //        bit 3
    unsigned char CombatFlags4 : 4;
    unsigned __int64 GetUID() const { return UniqueID; }
};

struct SCountBlock;
class CCombatSequenceBase;
#include "engine/CCountedPointer.h"
struct CRetailAllocator {
    CRetailAllocator() { }
};
struct CSequenceVector {                    // retail vector<CCountedPointer<CCombatSequenceBase>>, 12 bytes
    CCountedPointer<CCombatSequenceBase>* _M_start;
    CCountedPointer<CCombatSequenceBase>* _M_finish;
    CCountedPointer<CCombatSequenceBase>* _M_end_of_storage;
    ~CSequenceVector()
    {
        std::__destroy_aux(_M_start, _M_finish, std::__false_type());
        if (_M_start)
            free(_M_start);
    }
};
class CAttackingCombatantInfo { public:            // retail 0x30 bytes
    CThingCreatureBase* Attacked;           // +0x00
    unsigned char _data[0x20];
    CSequenceVector CombatSequences;        // +0x24
};
struct SAttackerValue {                     // pair<const unsigned __int64, CThingCreatureBase*>
    unsigned __int64 first;
    CThingCreatureBase* second;
    SAttackerValue(const unsigned __int64& k, CThingCreatureBase* const& v) : first(k), second(v) {}
};
struct SInsertResult {                      // pair<iterator, bool>
    void* first;
    bool second;
};
struct CAttackerMap {                       // map<unsigned __int64, CThingCreatureBase*> (STLport, 12 bytes)
    void* _M_header;
    unsigned long _M_node_count;
    unsigned long _M_compare;
    SInsertResult insert(const SAttackerValue& value);   // 0x006F1EF0
};
class CDefendingCombatantInfo { public:            // retail 0x3c bytes
    CAttackerMap Attackers;                 // +0x00
    unsigned char _data[0x30];
    ~CDefendingCombatantInfo();             // 0x006F48A0
};
struct CCombatantInfo {                     // retail 0x70 bytes
    CThingCreatureBase* Parent;             // +0x00
    CAttackingCombatantInfo AttackerInfo;   // +0x04
    CDefendingCombatantInfo DefenderInfo;   // +0x34
    CCombatantInfo();                       // 0x006F4A00
    CCombatantInfo(const CCombatantInfo& other);    // 0x006F4990
};

class CCombatManager { public:
    virtual void v00();
    unsigned char _sequences[0xc];          // +0x04 RegisteredCombatSequences
    std::map<unsigned __int64, CCombatantInfo> Combatants;  // +0x10
    bool    CombatStrafeTurningEnabled;     // +0x1c
    CWorld* World;                          // +0x20
    void ResetDefenderAttackPattern(CCombatantInfo& info, CAttackPatternPointer pattern);   // 0x006E8C30
    void RegisterAttackedCombatant(CThingCreatureBase& attacker, CThingCreatureBase& attacked);
};

void CCombatManager::RegisterAttackedCombatant(CThingCreatureBase& attacker, CThingCreatureBase& attacked)
{
    std::map<unsigned __int64, CCombatantInfo>::iterator it = Combatants.find(attacked.GetUID());
    if (it == Combatants.end()) {
        CCombatantInfo info;
        info.Parent = &attacked;
        info.AttackerInfo.Attacked = 0;
        std::pair<std::map<unsigned __int64, CCombatantInfo>::iterator, bool> result = Combatants.insert(std::pair<const unsigned __int64, CCombatantInfo>(attacked.GetUID(), info));
        it = result.first;
    }
    CCombatantInfo& info = it->second;
    info.DefenderInfo.Attackers.insert(SAttackerValue(attacker.GetUID(), &attacker));
    CAttackPatternPointer pattern;
    World->DefinitionManager->GetDef(DEFAULT_ATTACK_PATTERN_DEF_NAME, pattern);
    ResetDefenderAttackPattern(info, pattern);
    attacked.IsAttackedCombatant = true;
}