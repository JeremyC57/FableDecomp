// CCombatManager::SetupNextSequenceForAttacker(CCombatantInfo& info) -- reset the attacker's sequences, refuse an attacker whose primary-slot weapon does not fit (a ranged-weapon creature on the rotating ring needs a projectile weapon, a creature whose def demands it needs a melee weapon), pick the best sequence against the defender (CurrentSequence), and for a melee or break-into-melee sequence mark the attacker as the defender's last attacker and apply its combat block response (stopping if that ends its attack); any non-melee sequence then says its battle speech
// Leaf: SetupNextSequenceForAttacker
// Retail: Fable.exe (TLC, VC7.1) @ 0x006EB390, 622 bytes (624 with padding), `ret 4`.
// Callees: CCombatManager::PerformSequenceReset 0x006EA2C0, the combatant map's find 0x006F0250, CCombatManager::
//   IsCreatureOnRotatingCombatRing 0x006E9700 (catalogued "CCombatWheel_ValidateSequencePosition"; name as the landed
//   0x006EA2C0), GetBestSequenceIndexAttackerOnDefender 0x006EAE20, CThingCreatureBase 0x00662400 / 0x006623F0 (the
//   +0xE0 record's +0xE0 / +0xD8 words; FableWin.pdb's long-returning ranged/melee getters) and IsInMeleeAsAttacker
//   0x00662410, CTCCarrying::GetThingInPrimarySlot 0x006A1440, CTCWeapon::IsProjectileWeapon 0x005DAA40 / IsMeleeWeapon
//   0x005DAA00, the combat component's 0x00733CF0 (catalogued ApplyBlockResponse), CombatDialogueManager::
//   GetBestSpeechForCreature 0x008AF990 / SaySpeech 0x008AFC40, CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem
//   0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01238C6C), CVectorMap::
//   LowerBound 0x0040F020 (non-const, the 0x30 combat lookup) / 0x004365B0 (const, the 0x46 / 0x48 lookups), the
//   sequence's virtual slots 7 (+0x1C IsMeleeSequence, 0x006F0700) and 8 (+0x20 IsBreakIntoMeleeSequence, 0x006F0710).
// Shape (rescue R282s4): two levers.
//   (1) The def's AddRef/Release are inline CThingDef members called from CDefPointer's copy ctor and dtor: retail's
//       `ecx` inc temp in the GetDef copy and with it the entry assignment (esi creature, edi this-then-weapon, ebx def,
//       ebp info).
//   (2) The 0x30 combat component is fetched through the out-pointer GetInterface template (`CTCCombat* combat;
//       if (attacker->GetInterface(&combat, 0x30))`, the template 0x008CDA30 uses). Its out-pointer store leaves no
//       instruction behind but sits inside the sequence local's live range, which drops the sequence below the cached
//       `this`: VC7.1 reloads `this` into ebx at the GetBestSequenceIndex call and keeps it to the end, with the
//       sequence in ebp. The HasTC + GetTC chain (same instructions) gives the sequence ebx and re-reads `this` from its
//       slot twice (625 bytes). Probes: one extra instruction anywhere inside the sequence's range flips the
//       allocation; instructions after its last use, extra `this` uses, condition shapes (||, &&-goto, nesting, a bool
//       local), API spellings and intelligent-pointer models do not. Duplicating the PLastAttacker assignment into
//       if / else-if arms flips it too, but VC7.1's cross-jump of the arms schedules the merged SetPItem setups
//       lea-first where retail is push-first (622 bytes, 14 lines).
//   Slot 8 is IsBreakIntoMeleeSequence: every CCombatSequence vtable holds 0x006F0710 right after IsMeleeSequence
//   0x006F0700 (labels_rtti_port); the landed 0x006EB600 calls it "IsRangedSequence".
// Layout: local model (retail CCombatantInfo as the landed 0x006E97F0: Parent +0, Attacked +4, CurrentSequence +0x24,
//   CombatSequences +0x28, PLastAttacker +0x5c); CCombatManager Combatants +0x10, DialogueManager +0x38 (PDB +0x40: two
//   12-byte maps); CThing UniqueID +0x18, interface bitset +0x20 / map +0x44, def +0x70, dead flag +0x91 bit 0;
//   CCreatureDef flag byte +0x148.
struct CTCBase;
struct CBaseObject {};
struct CThing;

struct CInterfaceMap {                                    // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                 // 0x0040F020
    const SEntry* LowerBound(const long* type) const;     // 0x004365B0
};
#pragma auto_inline(off)
// Same-TU body of the const LowerBound instantiation (0x004365B0): binary search for the first entry whose type is
// not below the key. Only its visibility matters (frame packing of the inlined lookups).
__declspec(noinline) const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* it = first;
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

struct CThingDef {
    virtual void v00();
    virtual void Destroy();                               // slot 1
    long RefCount;                                        // +0x04
    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) Destroy(); }
};
struct CCreatureDef : CThingDef {
    unsigned char _pad08[0x148 - 0x08];
    unsigned char Flags148;                               // +0x148 (bit 5: a melee attacker needs a melee weapon)
};
template<class T> struct CDefPointer {
    T* Data;
    CDefPointer(const CDefPointer& other) : Data(other.Data) { if (Data) Data->AddRef(); }
    ~CDefPointer() { if (Data) Data->Release(); }
    T* operator->() const { return Data; }
};

struct CTCCarrying {
    CThing* GetThingInPrimarySlot() const;                // 0x006A1440
};
struct CTCWeapon {
    bool IsProjectileWeapon() const;                      // 0x005DAA40
    bool IsMeleeWeapon() const;                           // 0x005DAA00
};
struct CTCCombat {
    void ApplyBlockResponse(CThing* attacked);            // 0x00733CF0
};

struct CThing : CBaseObject {
    unsigned char _pad00[0x18];
    unsigned __int64 UniqueID;                            // +0x18
    unsigned long UsingComponents[9];                     // +0x20
    CInterfaceMap Components;                             // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefPointer<CCreatureDef> Def;                        // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char Flags91;                                // +0x91 bit 0: dead
    unsigned __int64 GetUID() const { return UniqueID; }
    CDefPointer<CCreatureDef> GetDef() const { return Def; }
    __forceinline unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    template<class T> __forceinline T* GetConstTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &Components;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CThingCreatureBase : CThing {
    long GetRangedWeaponDef() const;                      // 0x00662400
    long GetMeleeWeaponDef() const;                       // 0x006623F0
    bool IsInMeleeAsAttacker() const;                     // 0x00662410
};

struct CBaseIntelligentPointer {
    CBaseIntelligentPointer();                            // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                   // 0x00A01C10
    CBaseObject* GetPItem() const;                        // 0x00A01B50
    void SetPItem(CBaseObject* item);                     // 0x00A01B90
    CBaseObject* PData;                                   // +0x04
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C for CThing
    CIntelligentPointer() {}
    CIntelligentPointer(T* item) { SetPointer(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPointer(other.GetPointer()); return *this; }
    T* operator->() const { return GetPointer(); }
    T* GetPointer() const { return static_cast<T*>(GetPItem()); }
    void SetPointer(T* item) { SetPItem(item); }
};

struct CCombatSequenceBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06();
    virtual bool IsMeleeSequence() const;                 // slot 7 (+0x1C), 0x006F0700
    virtual bool IsBreakIntoMeleeSequence() const;        // slot 8 (+0x20), 0x006F0710
};
struct CSequencePointer {                                 // CCountedPointer<CCombatSequenceBase>
    CCombatSequenceBase* Object;
    void* Count;
};
template<class T> struct CRetailVector {                  // STLport vector, 12 bytes
    T* first;
    T* last;
    T* end_of_storage;
};
struct CAttackingCombatantInfo {                          // retail 0x30 bytes
    CThingCreatureBase* Attacked;                         // +0x00
    long AttackerNumber;                                  // +0x04
    long CombatWheelRingIndex;                            // +0x08
    bool PreferredAttackDollySet;                         // +0x0c
    long PreferredAttackDollyTimer;                       // +0x10
    float PreferredAttackDollyVector[2];                  // +0x14
    bool UsingCombatWheel;                                // +0x1c
    long CurrentSequence;                                 // +0x20
    CRetailVector<CSequencePointer> CombatSequences;      // +0x24
};
struct CCombatantInfo {
    CThingCreatureBase* Parent;                           // +0x00
    CAttackingCombatantInfo AttackerInfo;                 // +0x04
    unsigned char Attackers[0x0c];                        // +0x34
    unsigned char CombatWheel[0x0c];                      // +0x40
    float LastVictimWheelUpdatePos[3];                    // +0x4c
    long LastWheelUpdateWorldFrame;                       // +0x58
    CIntelligentPointer<CThing> PLastAttacker;            // +0x5c
};
struct CCombatantMap {                                    // STLport map<unsigned __int64, CCombatantInfo>
    struct node {
        long colour; node* parent; node* left; node* right;
        unsigned __int64 key;                             // +0x10
        CCombatantInfo value;                             // +0x18
    };
    node* header;
    unsigned long count;
    unsigned long comp;
    node* find(const unsigned __int64& key);              // 0x006F0250
};
struct CCombatManager;
struct CombatDialogueManager {
    unsigned long GetBestSpeechForCreature(const CThingCreatureBase& speaker, const CThingCreatureBase& target,
                                           const CCombatManager& manager) const;   // 0x008AF990
    bool SaySpeech(CThingCreatureBase& speaker, unsigned long speech);   // 0x008AFC40
};
struct CCombatManager {
    virtual void v00();
    unsigned char _sequences[0xc];                        // +0x04 RegisteredCombatSequences
    CCombatantMap Combatants;                             // +0x10
    bool CombatStrafeTurningEnabled;                      // +0x1c
    void* World;                                          // +0x20
    unsigned char _pad24[0x38 - 0x24];
    CombatDialogueManager DialogueManager;                // +0x38
    CCombatantInfo& GetCombatantInfo(const CThingCreatureBase& creature) { return Combatants.find(creature.GetUID())->value; }
    void PerformSequenceReset(CThingCreatureBase& creature);   // 0x006EA2C0
    bool IsCreatureOnRotatingCombatRing(const CThingCreatureBase& creature) const;   // 0x006E9700
    long GetBestSequenceIndexAttackerOnDefender(const CCombatantInfo& attacker, const CCombatantInfo& defender) const;   // 0x006EAE20
    void SetupNextSequenceForAttacker(CCombatantInfo& info);
};

void CCombatManager::SetupNextSequenceForAttacker(CCombatantInfo& info)
{
    CThingCreatureBase* defender = info.AttackerInfo.Attacked;
    CThingCreatureBase* attacker = info.Parent;
    const CCreatureDef* def = attacker->GetDef().Data;
    CThingCreatureBase& attacked = *info.AttackerInfo.Attacked;
    CCombatantInfo& defenderInfo = GetCombatantInfo(attacked);
    PerformSequenceReset(*attacker);
    CThing* weapon = 0;
    if (attacker->HasTC(0x46))
        weapon = attacker->GetConstTC<CTCCarrying>(0x46)->GetThingInPrimarySlot();
    if (IsCreatureOnRotatingCombatRing(*attacker)) {
        if (attacker->GetRangedWeaponDef() > 0) {
            if (weapon == 0 || (weapon->Flags91 & 1) || !weapon->HasTC(0x48))
                return;
            if (!weapon->GetConstTC<CTCWeapon>(0x48)->IsProjectileWeapon())
                return;
        }
    } else if (def->Flags148 & 0x20) {
        if (attacker->GetMeleeWeaponDef() > 0) {
            if (weapon == 0 || (weapon->Flags91 & 1) || !weapon->HasTC(0x48))
                return;
            if (!weapon->GetConstTC<CTCWeapon>(0x48)->IsMeleeWeapon())
                return;
        }
    }
    long index = GetBestSequenceIndexAttackerOnDefender(info, defenderInfo);
    info.AttackerInfo.CurrentSequence = index;
    if (index < 0)
        return;
    CCombatSequenceBase* sequence = info.AttackerInfo.CombatSequences.first[index].Object;
    if (sequence->IsMeleeSequence() || sequence->IsBreakIntoMeleeSequence()) {
        defenderInfo.PLastAttacker = CIntelligentPointer<CThing>(attacker);
        CTCCombat* combat;
        if (attacker->GetInterface(&combat, 0x30)) {
            combat->ApplyBlockResponse(defender);
            if (!attacker->IsInMeleeAsAttacker())
                return;
        }
    }
    if (!sequence->IsMeleeSequence()) {
        unsigned long speech = DialogueManager.GetBestSpeechForCreature(*attacker, attacked, *this);
        if (speech)
            DialogueManager.SaySpeech(*attacker, speech);
    }
}