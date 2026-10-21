// CAIStateGroup_MoveGoodsBase::Cleanup() -- finish the pick-up action, drop the carried goods (stop them moving, flag them, make them non-solid to the creature and the hero) and leave the carry mode.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009162F0, length 498 bytes. Vtable slot 9 (?Cleanup@CAIStateGroup_MoveGoodsBase@@UAEXXZ).
// Callees: CThingCreatureBase::IsPerformingAnyAction @ 0x006622A0, the action's ID getter @ 0x00692620 (`mov eax,[ecx+28h]`;
// manifest "CWorld::DrawGetEnvironment" is propagated), CThingCreatureBase::FinishCurrentAction @ 0x00662650,
// CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface: 0x46 carrying, 0x47 goods, 0x4d solid-to-thing, 0x31 mode
// manager), CTCCarrying::GetThingInPrimarySlot @ 0x006A1440 / RemoveThingFromPrimarySlot @ 0x006A1E00,
// CTCPhysicsBase::SetAsMoving @ 0x006B0130, CCharString(const char*, long) @ 0x0099EBF0 / dtor @ 0x0099EAE0,
// CThing::AddTC @ 0x004C9D60, CTCSolidToThing::AddThingNotSolidTo @ 0x00770FA0, CAIStateGroupBase::GetPPlayerManager
// @ 0x008FCF90, CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetHeroThing @ 0x00487DC0,
// CTCCreatureModeManager::RemoveMode @ 0x0073ABA0.
// Layout: engine/CAIStateGroup_MoveGoodsBase.h (DroppingGoods +0x14, PickUpGoodsActionID +0x18) over the landed local
// CAIStateGroupBase model (the generated header flattens the vptr); thing/creature members as the landed AI bodies.
// Shape: the carrying and mode-manager lookups are out-parameter GetInterface (miss path reads the never-written key
// slot; retail tail-duplicates the RemoveMode epilogue); the goods and solid lookups are a `GetTC<T>()` that returns 0 on
// a miss (VC threads the 0 straight past the null test). The physics component is read through an inline const getter
// (keeps `push 1` after the object expression); the CTCSolidToThing name is a named CCharString in the else block
// (built before the AddTC pushes, sharing the key slot: frame = one `push ecx`).
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length); // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};
struct CTCParamsBase;
struct CTCPhysicsBase {
    void SetAsMoving(bool moving);              // 0x006B0130
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
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
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        T* p;
        if (GetInterface(&p, interfaceType))
            return p;
        return 0;
    }
    CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
    CTCBase* AddTC(const CCharString& name, bool b, CTCParamsBase* params);   // 0x004C9D60
};
struct CActionBase {
    unsigned char _pad[0x28];
    long ID;                                    // +0x28
    long GetID() const;                         // 0x00692620
};
struct CThingCreatureBase : CThing {
    unsigned char _pad2[0x100 - 0x64];
    CActionBase* PCurrentAction;                // +0x100
    bool IsPerformingAnyAction();               // 0x006622A0
    void FinishCurrentAction();                 // 0x00662650
};
struct CTCCarrying {
    CThing* GetThingInPrimarySlot();            // 0x006A1440
    CThing* RemoveThingFromPrimarySlot(bool b); // 0x006A1E00
};
struct CTCGoods {                               // interface 0x47
    unsigned char _pad[0x0c];
    bool Dropped;                               // +0x0c
};
struct CTCSolidToThing {
    void AddThingNotSolidTo(CThing* thing);     // 0x00770FA0
};
struct CTCCreatureModeManager {
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
};
struct CPlayer { CThing* GetHeroThing(); };     // 0x00487DC0
struct CPlayerManager { CPlayer* GetMainPlayer(); };   // 0x00449970
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    CPlayerManager* GetPPlayerManager();        // 0x008FCF90
};
struct CAIStateGroup_MoveGoodsBase : CAIStateGroupBase {
    bool DroppingGoods;                         // +0x14
    long PickUpGoodsActionID;                   // +0x18
    virtual void Cleanup();
};

void CAIStateGroup_MoveGoodsBase::Cleanup()
{
    if (PBrain->PCreature->IsPerformingAnyAction()
        && PBrain->PCreature->PCurrentAction->GetID() == PickUpGoodsActionID)
        PBrain->PCreature->FinishCurrentAction();
    CTCCarrying* carrying;
    PBrain->PCreature->GetInterface(&carrying, 0x46);
    if (carrying->GetThingInPrimarySlot()) {
        if (carrying->GetThingInPrimarySlot()->PhysicsTC)
            carrying->GetThingInPrimarySlot()->GetPhysicsTC()->SetAsMoving(true);
        CTCGoods* goods = carrying->GetThingInPrimarySlot()->GetTC<CTCGoods>(0x47);
        if (goods)
            goods->Dropped = true;
        CTCSolidToThing* solid;
        if (carrying->GetThingInPrimarySlot()->HasTC(0x4d))
            solid = carrying->GetThingInPrimarySlot()->GetTC<CTCSolidToThing>(0x4d);
        else {
            CCharString name("CTCSolidToThing", -1);
            solid = (CTCSolidToThing*)carrying->GetThingInPrimarySlot()->AddTC(name, true, 0);
        }
        if (solid) {
            solid->AddThingNotSolidTo(PBrain->PCreature);
            solid->AddThingNotSolidTo(GetPPlayerManager()->GetMainPlayer()->GetHeroThing());
        }
        carrying->RemoveThingFromPrimarySlot(false);
        CTCCreatureModeManager* modes;
        PBrain->PCreature->GetInterface(&modes, 0x31);
        modes->RemoveMode(6);
    }
    DroppingGoods = false;
}