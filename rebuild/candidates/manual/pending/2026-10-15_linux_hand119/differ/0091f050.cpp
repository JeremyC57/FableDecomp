// Grade: DIFFER(128v126) -- register allocation of the component used as `this` of the slot-29 virtual call: retail `mov ecx,[eax+4] ; mov eax,[ecx] ; call [eax+74h]`, the lane `mov eax,[eax+4] ; mov edx,[eax] ; mov ecx,eax ; call [edx+74h]` for the out-parameter GetInterface, a pointer-returning find, the lookup written inline and an uncast CTCBase virtual (same gap as 0x0095EBF0)
// CAIStateGroup_CreatureRangedCombatUseWillShield::IsRunnable() -- runnable while the creature carries a melee weapon, its targeting component (interface 8) has a target it should attack, and it has no interface 0x93 component
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091F050, length 126 bytes (+ int3 padding).
// Callees: IsThingCarryingMeleeWeapon(CThing*) @ 0x006A1620 (__fastcall), the interface-map lower bound @ 0x004365B0
// (a second instantiation of CVectorMap::LowerBound; interface 8 = bitset word 0 bit 8 -> `mov ecx,[eax+20h] ;
// test ch,1`), the component's virtual slot 29 (+0x74, its current target), IsToAttackTarget(CThingAICreature*,
// CThingCreatureBase*) @ 0x00864D00 (__fastcall: creature in ecx, target in edx).
// Shape: nested ifs falling to one shared `xor al,al`; the lookup is split into the raw presence probe and a
// pointer-returning find whose component goes straight into ecx as `this` of the virtual call (the out-parameter
// GetInterface template routes it through eax); the creature is re-read from PBrain for every use; the
// interface-0x93 test is the inline raw bitset probe (`test dword ptr [eax+30h],80000h ; jne false`).
// Model: local (engine/CAIStateGroup_CreatureRangedCombatUseWillShield.h is a flattened base blob).
struct CTCBase;
struct CThingCreatureBase;
struct CThingAICreature;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x004365B0
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* FindInterface(long interfaceType)    // the lookup half: component straight from the entry
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};

bool __fastcall IsThingCarryingMeleeWeapon(CThing* thing);                              // 0x006A1620
bool __fastcall IsToAttackTarget(CThing* creature, CThingCreatureBase* target);          // 0x00864D00

struct CTCTargeting {                           // interface 8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual CThingCreatureBase* GetTarget();    // slot 29 (+0x74)
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* Creature;                           // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_CreatureRangedCombatUseWillShield : CAIStateGroupBase {
    virtual bool IsRunnable();
};

bool CAIStateGroup_CreatureRangedCombatUseWillShield::IsRunnable()
{
    if (IsThingCarryingMeleeWeapon(PBrain->Creature)) {
        CThing* creature = PBrain->Creature;
        if (creature->HasInterface(8)) {
            CThingCreatureBase* target = ((CTCTargeting*)creature->FindInterface(8))->GetTarget();
            if (target != 0 && IsToAttackTarget(PBrain->Creature, target) && !PBrain->Creature->HasInterface(0x93))
                return true;
        }
    }
    return false;
}
