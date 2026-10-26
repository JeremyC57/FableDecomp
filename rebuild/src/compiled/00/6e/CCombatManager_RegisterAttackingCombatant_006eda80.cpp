// CCombatManager::RegisterAttackingCombatant(CThingCreatureBase&, CThingCreatureBase&, bool) -- (re)register the attacker:
// drop its old attacking record (registering the attacked side when that has none), make sure it has a combatant record,
// pick its combat sequences, point it at the attacked creature with the next attacker number and reset its wheel state,
// flag it as attacking and put the attacked creature's stealth component in danger
// Leaf: RegisterCombatant
// Shape: the map::insert(const value_type&) instantiation has a same-TU stand-in that builds the pair<iterator, bool>
//   result in place, so the dropped result shares the find-key frame slot (frame 0xF8 as retail); find has its real
//   same-TU body and LowerBound a same-TU body.
extern "C" void __cdecl free(void* block);  // 0x00BFEA14

class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)

class CThingCreatureBase { public:
    unsigned char _pad[0x18];
    unsigned __int64 UniqueID;              // +0x18
    unsigned long m_presentBits[9];         // +0x20
    CInterfaceMap m_interfaces;             // +0x44
    unsigned char _pad4c[0x15c - 0x4c];
    unsigned char CombatFlags0 : 2;         // +0x15c
    unsigned char IsAttackingCombatant : 1; //        bit 2
    unsigned char IsAttackedCombatant : 1;  //        bit 3
    unsigned char CombatFlags4 : 4;
    unsigned __int64 GetUID() const { return UniqueID; }
    bool IsInCombatAsAttacker();            // 0x00662410
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
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
struct CSequencePointer {                   // CCountedPointer<CCombatSequenceBase>
    void* m_pObject;
    SCountBlock* m_pCount;
};
struct CRetailAllocator {
    CRetailAllocator() { }
};
void __fastcall DestroyRange_CountedPointer_006f2300(CSequencePointer* first, CSequencePointer* last, const CRetailAllocator& al);   // 0x006F2300
struct CSequenceVector {                    // retail vector<CCountedPointer<CCombatSequenceBase>>, 12 bytes
    CSequencePointer* _M_start;
    CSequencePointer* _M_finish;
    CSequencePointer* _M_end_of_storage;
    ~CSequenceVector()
    {
        DestroyRange_CountedPointer_006f2300(_M_start, _M_finish, CRetailAllocator());
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
struct SCombatantValue {                    // pair<const unsigned __int64, CCombatantInfo>
    unsigned __int64 first;                 // +0x00
    CCombatantInfo second;                  // +0x08
    SCombatantValue(const unsigned __int64& k, const CCombatantInfo& v) : first(k), second(v) {}
};
struct CRetailRbMap_UID_CombatantInfo {     // STLport map<unsigned __int64, CCombatantInfo>
    struct node {
        long  colour; node* parent; node* left; node* right;
        unsigned __int64 key;               // +0x10
        CCombatantInfo value;               // +0x18
    };
    struct SResult {                        // pair<iterator, bool>
        node* first;
        bool second;
    };
    node* header;
    unsigned long count;
    unsigned long comp;
    node* find(const unsigned __int64& key);    // 0x006F0250
    SResult insert(const SCombatantValue& value);   // 0x006F4F30
    node* end() { return header; }
};
#pragma auto_inline(off)
// Same-TU body of the STLport map::find instantiation 0x006F0250 (as in the landed 0x006E9D90).
__declspec(noinline) CRetailRbMap_UID_CombatantInfo::node* CRetailRbMap_UID_CombatantInfo::find(const unsigned __int64& key)
{
    node* y = header;
    node* x = header->parent;
    while (x != 0) {
        if (!(x->key < key)) {
            y = x;
            x = x->left;
        } else
            x = x->right;
    }
    if (y == header || key < y->key)
        return header;
    return y;
}
#pragma auto_inline(on)

CRetailRbMap_UID_CombatantInfo::node* InsertNode_006F4F30(CRetailRbMap_UID_CombatantInfo* map, const SCombatantValue& value);
#pragma auto_inline(off)
// Same-TU stand-in for the STLport map::insert(const value_type&) instantiation 0x006F4F30: it builds the
// pair<iterator, bool> result in place, so VC7.1 knows the hidden result slot does not escape.
__declspec(noinline) CRetailRbMap_UID_CombatantInfo::SResult CRetailRbMap_UID_CombatantInfo::insert(const SCombatantValue& value)
{
    SResult result;
    result.first = InsertNode_006F4F30(this, value);
    result.second = true;
    return result;
}
#pragma auto_inline(on)

class CCombatManager { public:
    virtual void v00();
    unsigned char _sequences[0xc];          // +0x04 RegisteredCombatSequences
    CRetailRbMap_UID_CombatantInfo Combatants;  // +0x10
    CCombatantInfo& GetCombatantInfo(const CThingCreatureBase& creature) { return Combatants.find(creature.GetUID())->value; }
    void RegisterAttackedCombatant(CThingCreatureBase& attacker, CThingCreatureBase& attacked);   // 0x006ED270
    void DeregisterAttackingCombatant(CThingCreatureBase* creature);   // 0x006ED510
    void GetCombatSequences(CThingCreatureBase* attacker, CThingCreatureBase* attacked, CSequenceVector* sequences);   // 0x006EC4F0
    void RegisterAttackingCombatant(CThingCreatureBase& attacker, CThingCreatureBase& attacked, bool usingCombatWheel);
};

void CCombatManager::RegisterAttackingCombatant(CThingCreatureBase& attacker, CThingCreatureBase& attacked, bool usingCombatWheel)
{
    if (attacker.IsInCombatAsAttacker()) {
        DeregisterAttackingCombatant(&attacker);
        if (Combatants.find(attacked.GetUID()) == Combatants.end())
            RegisterAttackedCombatant(attacker, attacked);
    }
    CRetailRbMap_UID_CombatantInfo::node* it = Combatants.find(attacker.GetUID());
    if (it == Combatants.end()) {
        CCombatantInfo info;
        info.Parent = &attacker;
        CRetailRbMap_UID_CombatantInfo::SResult result = Combatants.insert(SCombatantValue(attacker.UniqueID, info));
        it = result.first;
    }
    CCombatantInfo& info = it->value;
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