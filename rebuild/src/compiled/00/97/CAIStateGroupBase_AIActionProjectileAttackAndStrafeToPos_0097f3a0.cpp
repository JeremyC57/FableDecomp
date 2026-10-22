// CAIStateGroupBase::AIAction_ProjectileAttackAndStrafeToPos(EAIActionRepeat, C3DVector, CIntelligentPointer<const CThing>, bool&, EMovementType, bool) -- strafe to a position (creature mode 0x19 while the will-power indicator allows it), then fire the missile weapon at the target if it is still alive
// Retail: Fable.exe (TLC, VC7.1) @ 0x0097F3A0, 528 bytes (ret 24h).
// Callees: CThingCreatureBase::GetCombatTypeDef 0x00666D10, CDefClassBase::GetSubDef<CAICreatureWillPowerIndicatorDef>
//   0x006696B0 (same-TU stand-in), CVectorMap::LowerBound 0x0040F020 (same-TU stand-in; inlined GetInterface 0x31 =
//   CTCCreatureModeManager, bit 17 of +0x24), CTCCreatureModeManager::HasMode 0x0073A8A0 / AddMode 0x0073AE10 /
//   RemoveMode 0x0073ABA0, CAIStateGroupBase::AIAction_MoveToPos 0x0097EC00, CBaseIntelligentPointer::GetPItem 0x00A01B50 /
//   ~CBaseIntelligentPointer 0x00A01C10, CCreatureAction_FireMissileWeapon(CThingCreatureBase*, const CThing*) 0x008D5110,
//   CThingCreatureBase::SetCurrentAction 0x006644F0, ~CCreatureActionBase 0x00693EF0 (catalog "~CPhysicsMeshInfo", propagated).
// Layout: the state group follows engine/CAIStateGroupBase.h (PBrain +4), modelled locally to declare the members; the
// brain's creature at +0x18; the indicator def's flag at +0x40; CCreatureAction_FireMissileWeapon is the landed retail
// 0xC8-byte model (0x008D6320: PTarget +0xA8, PSpecialShotDef +0xC4).
// Shape: the mode lookup is the bitset::test GetInterface (the test bool is stored); the strafe flag is a named bool set
// true then cleared; the action is a temporary built inside the SetCurrentAction call and destroyed inline (def
// release + null, PTarget dtor, base dtor); the indicator def is released by hand at the end.
struct CTCBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
#pragma auto_inline(off)
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)

struct CDefCounted {
    virtual void v00();
    virtual void DeleteThis();                  // slot 1 (+4)
    long m_refCount;                            // +4
    void DecRef() { --m_refCount; }
    long GetRefCount() const { return m_refCount; }
    void Release() { DecRef(); if (GetRefCount() == 0) DeleteThis(); }
};
struct CAICreatureWillPowerIndicatorDef : CDefCounted {
    unsigned char _pad08[0x38];
    bool UseStrafe;                             // +0x40
};
struct CMeleeCombatAbilityDef : CDefCounted { };
template <class T> struct CDefPointer {
    T* m_pDef;
    ~CDefPointer() { if (m_pDef) { m_pDef->Release(); m_pDef = 0; } }
};
struct CDefClassBase {
    bool GetSubDef(CAICreatureWillPowerIndicatorDef** out) const;   // 0x006696B0
};
CAICreatureWillPowerIndicatorDef* FindWillPowerIndicatorDef(const CDefClassBase* def);
#pragma auto_inline(off)
bool CDefClassBase::GetSubDef(CAICreatureWillPowerIndicatorDef** out) const
{
    CAICreatureWillPowerIndicatorDef* old = *out;
    CAICreatureWillPowerIndicatorDef* def = FindWillPowerIndicatorDef(this);
    if (!def)
        return false;
    if (old != def)
        *out = def;
    return true;
}
#pragma auto_inline(on)
struct CCombatTypeDef : CDefClassBase { };

struct CTCCreatureModeManager {                 // interface type 0x31
    bool HasMode(unsigned long mode);           // 0x0073A8A0
    bool AddMode(unsigned long mode);           // 0x0073AE10
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    bool TestTC(long interfaceType) const
    {
        return ((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1) != 0;
    }
    template<class T> __forceinline bool GetInterfaceT(T** ppOut, long interfaceType)
    {
        bool has = TestTC(interfaceType);
        if (has) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
        return has;
    }
};
struct CCreatureActionBase;
struct CThingCreatureBase : CThing {
    CCombatTypeDef* GetCombatTypeDef();         // 0x00666D10
    bool SetCurrentAction(CCreatureActionBase* action);   // 0x006644F0
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CCreatureActionBase {                    // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    ~CCreatureActionBase();                     // 0x00693EF0
};
struct CCreatureAction_FireMissileWeapon : CCreatureActionBase {   // vtable 0x01285424
    CIntelligentPointerConstThing PTarget;                  // +0xa8
    float ShotAccuracyPercentage;                           // +0xb0
    float ShotDamageMultiplier;                             // +0xb4
    float ShotBuildupTime;                                  // +0xb8
    bool Decapitate;                                        // +0xbc
    long DelayTimer;                                        // +0xc0
    CDefPointer<CMeleeCombatAbilityDef> PSpecialShotDef;    // +0xc4
    CCreatureAction_FireMissileWeapon(CThingCreatureBase* creature, const CThing* target);   // 0x008D5110
};

struct CAIBrain {
    unsigned char _pad00[0x18];
    CThingCreatureBase* Creature;               // +0x18
};
enum EAIActionRepeat { AI_ACTION_REPEAT_ONCE = 0, AI_ACTION_REPEAT_UNTIL_DONE = 1, AI_ACTION_REPEAT_POLL = 2 };
enum EAIActionReturn { AI_ACTION_STOPPING = 1, AI_ACTION_DONE = 2, AI_ACTION_ABORTED = 3, AI_ACTION_RUNNING = 4 };
enum EMovementType { MOVEMENT_DEFAULT };

struct CAIStateGroupBase {
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->Creature; }
    EAIActionReturn AIAction_MoveToPos(EAIActionRepeat repeat, C3DVector pos, bool& speed, EMovementType anim, bool strafe,
                                       bool extra, const CThing* target, bool b3, float f);   // 0x0097EC00
    EAIActionReturn AIAction_ProjectileAttackAndStrafeToPos(EAIActionRepeat repeat, C3DVector pos,
                                                           CIntelligentPointerConstThing target, bool& speed,
                                                           EMovementType anim, bool extra);
};

EAIActionReturn CAIStateGroupBase::AIAction_ProjectileAttackAndStrafeToPos(EAIActionRepeat repeat, C3DVector pos,
                                                                          CIntelligentPointerConstThing target, bool& speed,
                                                                          EMovementType anim, bool extra)
{
    bool useStrafe = false;
    CAICreatureWillPowerIndicatorDef* indicator = 0;
    if (GetCreature()->GetCombatTypeDef()->GetSubDef(&indicator))
        useStrafe = indicator->UseStrafe;
    CTCCreatureModeManager* modes;
    GetCreature()->GetInterfaceT(&modes, 0x31);
    if (useStrafe && !modes->HasMode(0x19))
        modes->AddMode(0x19);
    bool notStrafing = true;
    if (modes->HasMode(0x19))
        notStrafing = false;
    EAIActionReturn result;
    do
        result = AIAction_MoveToPos(AI_ACTION_REPEAT_POLL, pos, speed, anim, notStrafing, extra, 0, true, 0.0f);
    while (repeat == AI_ACTION_REPEAT_UNTIL_DONE && result == AI_ACTION_RUNNING);
    if (useStrafe) {
        if (result != AI_ACTION_RUNNING)
            modes->RemoveMode(0x19);
        if (IS_THING_ALIVE(target)) {
            CThingCreatureBase* creature = GetCreature();
            creature->SetCurrentAction(&CCreatureAction_FireMissileWeapon(creature, target.GetPItem()));
        }
    }
    if (indicator)
        if (--indicator->m_refCount == 0)
            indicator->DeleteThis();
    return result;
}