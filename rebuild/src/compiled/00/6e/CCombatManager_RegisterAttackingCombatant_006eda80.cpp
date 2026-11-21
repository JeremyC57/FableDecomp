// CCombatManager::RegisterAttackingCombatant(CThingCreatureBase&, CThingCreatureBase&, bool) -- (re)register the attacker:
// drop its old attacking record (registering the attacked side when that has none), make sure it has a combatant record,
// pick its combat sequences, point it at the attacked creature with the next attacker number and reset its wheel state,
// flag it as attacking and put the attacked creature's stealth component in danger
// Leaf: ?RegisterAttackingCombatant@CCombatManager@@QAEXAAVCThingCreatureBase@@0_N@Z
// Plan phase 3 (2026-11-04), CVectorMap wave (stand-in map): the local stand-in map class (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Shape: the map::insert(const value_type&) forwards to the visible STLport insert_unique body (FABLE_RETAIL_STLP_MAP), so the
//   dropped result shares the find-key frame slot (frame 0xF8 as retail; declared only it keeps its own slot, 0x100: NF207);
//   _M_find is visible but out of line and LowerBound has a same-TU body.
// Name fix (agentNF6 2026-11-10): CThingCreatureBase::IsInMeleeAsAttacker 0x00662410 is ?IsInMeleeAsAttacker@CThingCreatureBase@@QBE_NXZ (Ego_r.pdb / FableWin.pdb), was IsAttackingCombatant / IsInCombatAsAttacker / IsRegisteredCombatant.
// Name fix (NF210, 2026-11-20, q4389, family FP-COMBATMAP): the invented CRetailRbMap_UID_CombatantInfo / SCombatantValue stand-ins
//   (and their same-TU noinline find / insert bodies) are replaced in place by the STLport map<unsigned __int64, CCombatantInfo> and
//   pair<const unsigned __int64, CCombatantInfo> (stlport/fable_map.h): _M_find<unsigned __int64> 0x006F0250 visible but out of line
//   (FABLE_RETAIL_STLP_TREE_MFIND_NOINLINE), map::insert forwards to the visible STLport insert_unique 0x006F4F30 (FABLE_RETAIL_STLP_MAP,
//   as the HEAD same-TU insert stand-in was visible: the dropped result keeps the 0xF8 frame). Pick-any copies (insert_unique 0x006F4F30,
//   operator-- 0x006F0030, _M_find 0x006F0250, less<_K> 0x00526120) equal their owners; _Rb_global<bool>::_M_decrement 0x004294E2 is
//   kept equal to its /Os owner by FABLE_RETAIL_STLP_RB_GLOBAL_DECREMENT_SIZE_BODY (scaffolding; the row's own body is the same without it).
#define FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL_DECL
#define FABLE_RETAIL_STLP_TREE_MFIND_NOINLINE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_RB_GLOBAL_DECREMENT_SIZE_BODY
#include "stlport/fable_map.h"
#include "fable_compiler.h"
extern "C" void __cdecl free(void* block);  // 0x00BFEA14

class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingCreatureBase { public:
    unsigned char _pad[0x18];
    unsigned __int64 UniqueID;              // +0x18
    unsigned long m_presentBits[9];         // +0x20
    CInterfaceMap m_interfaces;             // +0x44
    unsigned char _pad54[0x108];
    unsigned char CombatFlags0 : 2;         // +0x15c
    unsigned char IsAttackingCombatant : 1; //        bit 2
    unsigned char IsAttackedCombatant : 1;  //        bit 3
    unsigned char CombatFlags4 : 4;
    unsigned __int64 GetUID() const { return UniqueID; }
    bool IsInMeleeAsAttacker() const;            // 0x00662410
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
enum EDangerStatus { DANGER_STATUS_3 = 3 };
class CTCStealth { public:                         // interface 0x36
    void SetAsInDanger(CThingCreatureBase* source, EDangerStatus status);   // 0x00747B80
};

struct SCountBlock;
class CCombatSequenceBase;
#include "engine/CCountedPointer.h"
struct CRetailAllocator {
    CRetailAllocator() { }
};
// std::__false_type and the declaration-only std::__destroy_aux come from stlport/fable_containers.h (via engine/CArray.h)
// with FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL_DECL: 0x006F2300 std::__destroy_aux<CCountedPointer<CCombatSequenceBase>*>.
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
class CAttackingCombatantInfo { public:            // retail 0x30 bytes (Ego_r names)
    CThingCreatureBase* Attacked;           // +0x00
    long AttackerNumber;                    // +0x04
    long CombatWheelRingIndex;              // +0x08
    bool PreferredAttackDollySet;           // +0x0c
    long PreferredAttackDollyTimer;         // +0x10
    float PreferredAttackDollyVector[2];    // +0x14 C2DVector
    bool UsingCombatWheel;                  // +0x1c
    long CurrentSequence;                   // +0x20
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
    unsigned long size() const { return _M_node_count; }
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
    CCombatantInfo& GetCombatantInfo(const CThingCreatureBase& creature) { return Combatants.find(creature.GetUID())->second; }
    void RegisterAttackedCombatant(CThingCreatureBase& attacker, CThingCreatureBase& attacked);   // 0x006ED270
    private: void DeregisterAttackingCombatant(CThingCreatureBase& creature); public:   // 0x006ED510 (Ego_r.pdb + FableWin.pdb, private)
    void GetCombatSequences(CThingCreatureBase* attacker, CThingCreatureBase* attacked, CSequenceVector* sequences);   // 0x006EC4F0
    void RegisterAttackingCombatant(CThingCreatureBase& attacker, CThingCreatureBase& attacked, bool usingCombatWheel);
};

void CCombatManager::RegisterAttackingCombatant(CThingCreatureBase& attacker, CThingCreatureBase& attacked, bool usingCombatWheel)
{
    if (attacker.IsInMeleeAsAttacker()) {
        DeregisterAttackingCombatant(attacker);
        if (Combatants.find(attacked.GetUID()) == Combatants.end())
            RegisterAttackedCombatant(attacker, attacked);
    }
    std::map<unsigned __int64, CCombatantInfo>::iterator it = Combatants.find(attacker.GetUID());
    if (it == Combatants.end()) {
        CCombatantInfo info;
        info.Parent = &attacker;
        std::pair<std::map<unsigned __int64, CCombatantInfo>::iterator, bool> result = Combatants.insert(std::pair<const unsigned __int64, CCombatantInfo>(attacker.UniqueID, info));
        it = result.first;
    }
    CCombatantInfo& info = it->second;
    GetCombatSequences(&attacker, &attacked, &info.AttackerInfo.CombatSequences);
    info.AttackerInfo.Attacked = &attacked;
    info.AttackerInfo.AttackerNumber = GetCombatantInfo(attacked).DefenderInfo.Attackers.size();
    info.AttackerInfo.CurrentSequence = -1;
    info.AttackerInfo.CombatWheelRingIndex = -1;
    info.AttackerInfo.PreferredAttackDollySet = false;
    info.AttackerInfo.PreferredAttackDollyTimer = -1;
    info.AttackerInfo.UsingCombatWheel = usingCombatWheel;
    attacker.IsAttackingCombatant = true;
    CTCStealth* stealth;
    if (attacked.GetInterface(&stealth, 0x36))
        stealth->SetAsInDanger(&attacker, DANGER_STATUS_3);
}