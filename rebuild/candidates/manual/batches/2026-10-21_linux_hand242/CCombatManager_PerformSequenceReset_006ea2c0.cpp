// CCombatManager::PerformSequenceReset(CThingCreatureBase&) -- end the creature's running combat sequence: a running melee sequence advances its victim's attack pattern and, unless the creature already circles on a rotating ring, may (by its combat type's ring-rotation chance) set its ring turning either way at the def's speed; the sequence is stopped and the current index cleared
// Leaf: PerformSequenceReset
// Retail: Fable.exe (TLC, VC7.1) @ 0x006EA2C0, 355 bytes (368 with padding). `ret 4`.
// Callees: the combatant map's find 0x006F0250 (twice), CCombatManager::IncrementDefenderAttackPattern 0x006E8600,
//          IsCreatureOnRotatingCombatRing 0x006E9700 (catalogued "CCombatWheel_ValidateSequencePosition"; name from
//          the Ego_r CCombatManager predicates), CThingCreatureBase::GetCombatTypeDef 0x00666D10, CDefClassBase::
//          GetSubDef<T> 0x006F0F60 (same-TU stand-in below), CWorld::GetPRandomSeed 0x0049D810, `__fastcall` GFROR13
//          0x00497890, SetAttackerRingAsRotating 0x006E9D90, CCombatSequenceBase::Stop 0x008B0640, the sequence's
//          virtual slot 7 (+0x1c, IsMeleeSequence), the sub-def's virtual slot 1 on release. Floats 1.0f 0x0122DED8,
//          -1.0f 0x0122DEE0.
//
// Shape: GetCombatantInfo by reference (each UniqueID a GetUID() temporary in one 8-byte slot, as the landed
// 0x006E9D90, with the same-TU find body); the victim is kept as the found NODE and `->value` is formed at its one
// use (a CCombatantInfo& local adds the +0x18 early and swaps esi/edi); the sub-def pointer is zeroed before the rotating-ring test and loaded once (ebp) after GetSubDef:
// the same-TU stand-in (writes the out slot only inline) tells VC7.1 its address does not escape; it is released
// with the def's inline Release (`dec [ebp+4] ; jne`) and a null test. The world random step is inline
// (seed * 0x24A1 + 0x24DF, GFROR13), the roll a signed long.
// Layout: CCombatManager / CCombatantInfo as the landed 0x006E9D90 and 0x006E97F0 (World +0x20, Combatants +0x10,
// Attacked +4, CurrentSequence +0x24, CombatSequences +0x28); the ring-rotation sub-def's chance +0x2c and speed
// +0x40 (descriptive names).
extern unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

__forceinline unsigned long WorldRandom(unsigned long& seed)
{
    seed = seed * 0x24A1 + 0x24DF;
    seed = GFROR13(seed);
    return seed;
}

struct CDefPointeeBase {                    // a counted definition object
    virtual void v000();
    virtual void DestroyDef();              // slot 1 (+4): called when the count drops to zero
    long m_refCount;                        // +4
    void ReleaseDef()
    {
        if (--m_refCount == 0)
            DestroyDef();
    }
};
template<class T> struct CDefPointer {      // CDefPointer<const T>: one counted pointer to a definition
    T* m_pObject;
    CDefPointer() : m_pObject(0) { }
    ~CDefPointer()
    {
        if (m_pObject)
            m_pObject->ReleaseDef();
    }
    T* operator->() const { return m_pObject; }
};
struct CRingRotationDef : CDefPointeeBase {
    unsigned char _pad08[0x24];
    long  RotationChance;                   // +0x2c (percent)
    unsigned char _pad30[0x10];
    float RotationSpeed;                    // +0x40
};
void* FindDef_GetSubDef_006f0f60();         // stand-in callee of the stand-in body below
struct CCombatTypeDef {
    bool GetSubDef(CDefPointer<CRingRotationDef>& out) const;   // 0x006F0F60
};
#pragma auto_inline(off)
// Same-TU stand-in for the GetSubDef<T> instantiation 0x006F0F60: it writes the out slot only inline, so VC7.1
// knows the def pointer's address does not escape the call (the landed 0x006E3780 idiom).
__declspec(noinline) bool CCombatTypeDef::GetSubDef(CDefPointer<CRingRotationDef>& out) const
{
    void* def = FindDef_GetSubDef_006f0f60();
    if (!def)
        return false;
    *(void**)&out = def;
    return true;
}
#pragma auto_inline(on)

struct CThingCreatureBase {
    unsigned char _pad[0x18];
    unsigned __int64 UniqueID;              // +0x18
    unsigned __int64 GetUID() const { return UniqueID; }
    CCombatTypeDef* GetCombatTypeDef() const;   // 0x00666D10
};
struct CCombatSequenceBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06();
    virtual bool IsMeleeSequence();         // slot 7 (+0x1c)
    bool Running;                           // +0x04
    void Stop();                            // 0x008B0640
};
struct CSequencePointer {                   // CCountedPointer<CCombatSequenceBase>
    CCombatSequenceBase* Object;
    void* Count;
};
template<class T> struct CRetailVector {    // retail std::vector: {first, last, end_of_storage}, 12 bytes
    T* first;
    T* last;
    T* end_of_storage;
    T& operator[](unsigned long n) const { return first[n]; }
};
struct CAttackingCombatantInfo {            // retail 0x30 bytes
    CThingCreatureBase* Attacked;           // +0x00
    long AttackerNumber;                    // +0x04
    long CombatWheelRingIndex;              // +0x08
    bool PreferredAttackDollySet;           // +0x0c
    long PreferredAttackDollyTimer;         // +0x10
    float PreferredAttackDollyVector[2];    // +0x14
    bool UsingCombatWheel;                  // +0x1c
    long CurrentSequence;                   // +0x20
    CRetailVector<CSequencePointer> CombatSequences;   // +0x24
};
struct CCombatantInfo {                     // retail layout (Ego_r.pdb names; 12-byte vectors/maps)
    CThingCreatureBase* Parent;             // +0x00
    CAttackingCombatantInfo AttackerInfo;   // +0x04
    unsigned char DefenderInfo[0x3c];       // +0x34
};
struct CRetailRbMap_UID_CombatantInfo {     // STLport map<unsigned __int64, CCombatantInfo>
    struct node {
        long  colour; node* parent; node* left; node* right;
        unsigned __int64 key;               // +0x10
        CCombatantInfo value;               // +0x18
    };
    node* header;
    unsigned long count;
    unsigned long comp;
    node* find(const unsigned __int64& key);    // 0x006F0250
};
#pragma auto_inline(off)
// Same-TU body of the STLport map::find instantiation 0x006F0250 (template bodies are compiled in the caller's TU,
// as in the landed 0x006E9D90): the lower_bound walk reading the 64-bit key through its const reference, then the
// end() test.
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
struct CWorld {
    unsigned long* GetPRandomSeed();        // 0x0049D810
};
struct CCombatManager {
    virtual void v00();
    unsigned char _sequences[0xc];          // +0x04 RegisteredCombatSequences
    CRetailRbMap_UID_CombatantInfo Combatants;  // +0x10
    bool    CombatStrafeTurningEnabled;     // +0x1c
    CWorld* World;                          // +0x20
    CCombatantInfo& GetCombatantInfo(const CThingCreatureBase& creature)
    {
        return Combatants.find(creature.GetUID())->value;
    }
    void IncrementDefenderAttackPattern(CCombatantInfo& info);             // 0x006E8600
    bool IsCreatureOnRotatingCombatRing(const CThingCreatureBase& creature);   // 0x006E9700
    void SetAttackerRingAsRotating(const CThingCreatureBase& attacker, float degreesPerSecond);   // 0x006E9D90
    void PerformSequenceReset(CThingCreatureBase& creature);
};

void CCombatManager::PerformSequenceReset(CThingCreatureBase& creature)
{
    CCombatantInfo& info = GetCombatantInfo(creature);
    CRetailRbMap_UID_CombatantInfo::node* victim = Combatants.find(info.AttackerInfo.Attacked->GetUID());
    if (info.AttackerInfo.CurrentSequence >= 0) {
        CCombatSequenceBase* sequence = info.AttackerInfo.CombatSequences[info.AttackerInfo.CurrentSequence].Object;
        if (sequence->Running) {
            if (sequence->IsMeleeSequence()) {
                IncrementDefenderAttackPattern(victim->value);
                CDefPointer<CRingRotationDef> rotation;
                if (!IsCreatureOnRotatingCombatRing(creature)) {
                    if (creature.GetCombatTypeDef()->GetSubDef(rotation)) {
                        long roll = WorldRandom(*World->GetPRandomSeed()) % 100;
                        if (roll <= rotation->RotationChance) {
                            float speed = rotation->RotationSpeed;
                            float direction = (WorldRandom(*World->GetPRandomSeed()) & 1) == 0 ? 1.0f : -1.0f;
                            SetAttackerRingAsRotating(creature, direction * speed);
                        }
                    }
                }
            }
            sequence->Stop();
        }
        info.AttackerInfo.CurrentSequence = -1;
    }
}
