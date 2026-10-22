// CCombatManager::DeregisterAttackingCombatant(CThingCreatureBase* attacker) -- take an attacker out of its fight: clear the defender's last-attacker pointer when it is this attacker (and advance the defender's attack pattern if the attacker's current sequence is melee or ranged), trace the deregistration, drop the attacker from the defender's attacker map (deregister the defender when none are left, else refresh its attacker list), clear the attacker's sequences and target, and for an attacker that is no longer a combatant erase its entry, reset a projectile weapon held in its primary slot and unsheathe when its def asks; finally clear the defender's stealth danger
// Leaf: ?DeregisterAttackingCombatant@CCombatManager
// Retail: Fable.exe (TLC, VC7.1) @ 0x006ED510, 941 bytes, `ret 4`, frame 0x30.
// Callees: the combatant map's find 0x006F0250 and erase(key) 0x006F50B0, the defender's attacker-map erase(key) 0x006F3200
//   (same-TU stand-ins: retail compiles the map instantiations into this TU), CBaseIntelligentPointer ctor/GetPItem/SetPItem/
//   dtor 0x00A01B10/0x00A01B50/0x00A01B90/0x00A01C10, IncrementDefenderAttackPattern 0x006E8600, UpdateAttackingCombatantsList
//   0x006ED030, DeregisterAttackedCombatant 0x006ED8C0, CreatureUnsheathWeapon 0x006EBDD0, CThingCreatureBase::IsCombatant
//   0x00662430, CThing::GetDefName 0x004C7CC0, CDefStringTable::GetString 0x009D49B0 (table 0x013CA828), `__fastcall` CCharString
//   operator+ 0x0099F690/0x0099F600/0x0099F570, GFIntToCharString 0x0099F830, ~CCharString 0x0099EAE0 (manifest label is a
//   propagated _Cons_val twin), the sequence vector's __copy 0x006F1410 and _Destroy_range 0x006F2300, CVectorMap LowerBound
//   0x0040F020 (interfaces 0x48, 0x36) and its const twin 0x004365B0 (interface 0x46), CTCCarrying::GetThingInPrimarySlot
//   0x006A1440, CTCWeapon 0x005DAA40/0x005DA4E0, CTCStealth::SetAsInDanger 0x00747B80.
// Shape (rescue R282s4): (1) the first trace is two nested __forceinline helpers that take the names BY VALUE (outer:
//   attackedName by value + the attacker; inner: attackerName by value + attackedName by const ref), so VC7.1 builds GetString
//   straight into each parameter in retail's order (attacked first), keeps both GetDefName results alive across the
//   concatenation (slots esp+34h/38h, read through the returned pointer) and destroys F..A, intStr, attackerName,
//   attackedName; the declared out-of-line copy ctor (0x0099EC30, never called) is what allows the in-place build.
//   (2) vector clear() as STLport `erase(begin(), end())` (the iterator plumbing fixes the post-trace scratch registers).
//   (3) the def release through an inline `Release()` member (ecx inc temp). (4) the stealth lookup as HasTC + a returned
//   GetTC (component lands in ecx). (5) the 0x46 carrying lookup goes through the const LowerBound 0x004365B0, as retail.
//   Both traces are built and dropped (the debug output is compiled out).
// Layout: local model (no engine header for CCombatManager): RegisteredCombatSequences +0x04 (12 bytes), Combatants +0x10;
//   map node value CCombatantInfo at +0x18 (retail 0x70 bytes: AttackerInfo +0x04, DefenderInfo +0x34); retail CThing
//   UniqueID +0x18, component bits +0x20, interface map +0x44, def +0x70, dead flag +0x91 bit 0, combat flags +0x15c.
struct CCharString {
    void* m_pData;
    CCharString(const CCharString& other);         // 0x0099EC30
    ~CCharString();                                // 0x0099EAE0
};
CCharString __fastcall operator+(const char* left, const CCharString& right);          // 0x0099F690
CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
CCharString __fastcall GFIntToCharString(long value);                                  // 0x0099F830
struct CDefString {
    long m_index;
};
struct CDefStringTable {
    CCharString GetString(CDefString name);        // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;           // 0x013CA828
inline void CombatTrace(const CCharString&) {}     // the trace output, compiled out

struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);          // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0 (the const instantiation)
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                        // slot 1
};
struct CCreatureDef : CDefVirtual {
    long          RefCount;                        // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
    unsigned char _pad08[0x148 - 0x08];
    unsigned char Flags148;                        // +0x148 bit 5: unsheathe after combat
};
template<class T> struct CDefPointer {
    T* Object;
    CDefPointer(T* object) : Object(object) { if (Object) ++Object->RefCount; }
    ~CDefPointer() { if (Object) Object->Release(); }
    T* Get() const { return Object; }
};
struct CThing {
    unsigned char    _pad0[0x18];
    unsigned __int64 UniqueID;                     // +0x18
    unsigned long    m_presentBits[9];             // +0x20
    CInterfaceMap    m_interfaces;                 // +0x44
    unsigned char    _pad4c[0x70 - 0x4c];
    CCreatureDef*    m_pDef;                       // +0x70
    unsigned char    _pad74[0x91 - 0x74];
    unsigned char    m_flags91;                    // +0x91 bit 0: dead
    unsigned char    _pad92[0x15c - 0x92];
    unsigned char    CombatFlags0 : 2;             // +0x15c
    unsigned char    IsAttackingCombatant : 1;     //        bit 2
    unsigned char    IsAttackedCombatant : 1;      //        bit 3
    unsigned char    CombatFlags4 : 4;
    CDefString GetDefName() const;                 // 0x004C7CC0
    unsigned __int64 GetUID() const { return UniqueID; }
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    template<class T> __forceinline bool GetConstInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
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
struct CThingCreatureBase : CThing {
    bool IsCombatant();                            // 0x00662430
};
struct CTCCarrying {                               // interface 0x46
    CThing* GetThingInPrimarySlot();               // 0x006A1440
};
struct CTCWeapon {                                 // interface 0x48
    bool IsProjectileWeapon();                     // 0x005DAA40
    void ResetAiming();                            // 0x005DA4E0 (AimingTime +0x14 = -1, AimingTimeOverride +0x18 = false)
};
enum EDangerStatus { DANGER_NONE = 0 };
struct CTCStealth {                                // interface 0x36
    void SetAsInDanger(CThing* source, EDangerStatus status);   // 0x00747B80
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                               // +0x04
    CBaseIntelligentPointer();                     // 0x00A01B10
    ~CBaseIntelligentPointer();                    // 0x00A01C10
    CThing* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CThing* item);                   // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};

struct CCombatSequenceBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06();
    virtual bool IsMeleeSequence();                // slot 7 (+0x1c)
    virtual bool IsRangedSequence();               // slot 8 (+0x20)
};
struct SCountBlock;
struct CSequencePointer {                          // CCountedPointer<CCombatSequenceBase>
    CCombatSequenceBase* Object;
    SCountBlock*         Count;
};
struct random_access_iterator_tag {
    random_access_iterator_tag() { }
};
struct CRetailAllocator {
    CRetailAllocator() { }
};
CSequencePointer* __fastcall __copy(CSequencePointer* first, CSequencePointer* last, CSequencePointer* result,
                                    const random_access_iterator_tag& tag, int* distance);         // 0x006F1410
void __fastcall _Destroy_range(CSequencePointer* first, CSequencePointer* last, const CRetailAllocator& al);   // 0x006F2300
struct CSequenceVector {                           // retail vector<CCountedPointer<CCombatSequenceBase>>, 12 bytes
    CSequencePointer* _M_start;
    CSequencePointer* _M_finish;
    CSequencePointer* _M_end_of_storage;
    CSequencePointer& operator[](unsigned long n) { return _M_start[n]; }
    CSequencePointer* begin() { return _M_start; }
    CSequencePointer* end() { return _M_finish; }
    CSequencePointer* erase(CSequencePointer* first, CSequencePointer* last)
    {
        CSequencePointer* i = __copy(last, _M_finish, first, random_access_iterator_tag(), (int*)0);
        _Destroy_range(i, _M_finish, CRetailAllocator());
        _M_finish = i;
        return first;
    }
    void clear() { erase(begin(), end()); }
};
struct CAttackerMap {                              // map<unsigned __int64, CThingCreatureBase*> (STLport, 12 bytes)
    void*         _M_header;
    unsigned long _M_node_count;
    unsigned long _M_compare;
    unsigned long size() const { return _M_node_count; }
    unsigned long erase(const unsigned __int64& key);   // 0x006F3200
};
struct CAttackingCombatantInfo {                   // retail 0x30 bytes
    CThingCreatureBase* Attacked;                  // +0x00
    long  AttackerNumber;                          // +0x04
    long  CombatWheelRingIndex;                    // +0x08
    bool  PreferredAttackDollySet;                 // +0x0c
    long  PreferredAttackDollyTimer;               // +0x10
    float PreferredAttackDollyVector[2];           // +0x14
    bool  UsingCombatWheel;                        // +0x1c
    long  CurrentSequence;                         // +0x20
    CSequenceVector CombatSequences;               // +0x24
};
struct CDefendingCombatantInfo {                   // retail 0x3c bytes
    CAttackerMap  Attackers;                       // +0x00
    unsigned char CombatWheel[0x0c];               // +0x0c
    float         LastVictimWheelUpdatePos[3];     // +0x18
    long          LastWheelUpdateWorldFrame;       // +0x24
    CIntelligentPointerThing PLastAttacker;        // +0x28
    void*         PAttackPattern;                  // +0x30
    long          AttackPatternIndex;              // +0x34
    long          FrameForNextInstruction;         // +0x38
};
struct CCombatantInfo {                            // retail 0x70 bytes
    CThingCreatureBase*     Parent;                // +0x00
    CAttackingCombatantInfo AttackerInfo;          // +0x04
    CDefendingCombatantInfo DefenderInfo;          // +0x34
};
struct SCombatantNode {
    char             _M_color;
    SCombatantNode*  _M_parent;
    SCombatantNode*  _M_left;
    SCombatantNode*  _M_right;
    unsigned __int64 first;                        // +0x10
    CCombatantInfo   second;                       // +0x18
};
struct CCombatantMap {
    SCombatantNode* _M_header;
    unsigned long   _M_node_count;
    unsigned long   _M_key_compare;
    SCombatantNode* find(const unsigned __int64& key);     // 0x006F0250
    void erase(const unsigned __int64& key);               // 0x006F50B0
};

long TmplSink_0(const void* self, long seen);      // stand-ins: the work the STLport instantiations do (outside the graded bytes)
#pragma auto_inline(off)
// Same-TU stand-ins for the map instantiations (template bodies are compiled in the caller's TU): they read the key
// through the reference and let no argument escape.
__declspec(noinline) SCombatantNode* CCombatantMap::find(const unsigned __int64& key)
{
    return (SCombatantNode*)TmplSink_0(this, 0 + *(const long*)&key);
}
__declspec(noinline) void CCombatantMap::erase(const unsigned __int64& key)
{
    TmplSink_0(this, 1 + *(const long*)&key);
}
__declspec(noinline) unsigned long CAttackerMap::erase(const unsigned __int64& key)
{
    return TmplSink_0(this, 2 + *(const long*)&key);
}
#pragma auto_inline(on)
__forceinline void TraceDeregister(CCharString attackerName, const CCharString& attackedName, const CCombatantInfo& attackedInfo)
{
    CombatTrace("Deregistering combatant " + attackerName + " was attacking " + attackedName + " (who has " +
                GFIntToCharString(attackedInfo.DefenderInfo.Attackers.size() - 1) + " things still attacking)");
}
__forceinline void TraceDeregister(const CThing* attacker, CCharString attackedName, const CCombatantInfo& attackedInfo)
{
    TraceDeregister(g_DefStringTable.GetString(attacker->GetDefName()), attackedName, attackedInfo);
}
class CCombatManager {
public:
    virtual void v00();
    unsigned char _sequences[0x0c];                // +0x04 RegisteredCombatSequences
    CCombatantMap Combatants;                      // +0x10
    void IncrementDefenderAttackPattern(CCombatantInfo& info);                    // 0x006E8600
    void CreatureUnsheathWeapon(CThingCreatureBase* creature, long weapon);        // 0x006EBDD0
    void UpdateAttackingCombatantsList(CCombatantInfo& info, bool force);         // 0x006ED030
    void DeregisterAttackedCombatant(CThingCreatureBase* attacked);               // 0x006ED8C0
    void DeregisterAttackingCombatant(CThingCreatureBase* attacker);
};

void CCombatManager::DeregisterAttackingCombatant(CThingCreatureBase* attacker)
{
    CCombatantInfo& info = Combatants.find(attacker->GetUID())->second;
    CThingCreatureBase* attacked = info.AttackerInfo.Attacked;
    CCombatantInfo& attackedInfo = Combatants.find(attacked->GetUID())->second;
    if (attackedInfo.DefenderInfo.PLastAttacker.GetPItem() == attacker) {
        attackedInfo.DefenderInfo.PLastAttacker = 0;
        if (info.AttackerInfo.CurrentSequence >= 0) {
            CCombatSequenceBase* sequence = info.AttackerInfo.CombatSequences[info.AttackerInfo.CurrentSequence].Object;
            if (sequence->IsMeleeSequence() || sequence->IsRangedSequence())
                IncrementDefenderAttackPattern(attackedInfo);
        }
    }
    TraceDeregister(attacker, g_DefStringTable.GetString(info.AttackerInfo.Attacked->GetDefName()), attackedInfo);
    attackedInfo.DefenderInfo.Attackers.erase(attacker->GetUID());
    if (attackedInfo.DefenderInfo.Attackers.size() == 0) {
        DeregisterAttackedCombatant(attacked);
    } else {
        CombatTrace("deregistered guy but defender is still has " + GFIntToCharString(attackedInfo.DefenderInfo.Attackers.size()) +
                    " attackers");
        UpdateAttackingCombatantsList(attackedInfo, true);
    }
    info.AttackerInfo.CombatSequences.clear();
    info.AttackerInfo.Attacked = 0;
    attacker->IsAttackingCombatant = 0;
    if (!attacker->IsCombatant()) {
        Combatants.erase(attacker->GetUID());
        CTCCarrying* carrying;
        if (attacker->GetConstInterface(&carrying, 0x46)) {
            CThing* item = carrying->GetThingInPrimarySlot();
            CTCWeapon* weapon;
            if (item && !(item->m_flags91 & 1) && item->GetInterface(&weapon, 0x48)) {
                if (weapon->IsProjectileWeapon())
                    weapon->ResetAiming();
            }
        }
        CCreatureDef* def = CDefPointer<CCreatureDef>(attacker->m_pDef).Get();
        if (def->Flags148 & 0x20)
            CreatureUnsheathWeapon(attacker, 3);
    }
    if (attacked->HasTC(0x36))
        attacked->GetTC<CTCStealth>(0x36)->SetAsInDanger(attacker, DANGER_NONE);
}
