// Grade: DIFFER(1365v1361) -- frame 0x50 v 0x48: retail overlays the TC-key temps on the combat / target slots and the __true_type tag on the dead event slot, and homes the locked target at [esp+0x14] right after the call (ours at the IS_THING_ALIVE call); otherwise the same instructions
// CInputProcessCombat::ProcessCombatInput(const CInputEvent& event, CProcessedInput& processed) -- the hero's melee input:
//   attacks are allowed unless he carries a projectile weapon (then only against a live locked target whose TC 9 has the
//   0x4 flag) or the attack/flourish action (6, 9) fires while a talk / use target is in range; on a button press queue
//   the flourish (action 8, gated on the alternate-controls action-9 queue test), break-block (action 8) or attack
//   (action 9) ability event 0x16, then action 0xe -> event 0x22(true), 0xd -> 0x21(true), 0xf / 0x10 -> 0x20
// Leaf: ProcessCombatInput
// Flags: /GS
// Retail: Fable.exe @ 0x0068F470, 1376 bytes, `ret 8` (/GS cookie 0x0139C8A8, __security_check_cookie 0x00BFE9F9).
// Callees: CPlayer::GetHeroCreature 0x00487DC0, the const TC LowerBound 0x004365B0, CTCCarrying::GetThingInPrimarySlot
//   0x006A1440, CTCWeapon::IsProjectileWeapon 0x005DAA40, IS_THING_ALIVE<CThing*> 0x0040DCE0, vector<long>
//   _M_insert_overflow 0x0068FD80, the player interface's action query 0x00446D10 and alternate-controls test 0x00445BC0
//   (descriptive), CTCTargeting::IsTargetedThingOfType 0x007F5A70, IsEventButtonPressed 0x00687A90, IsGameActionEventInQueue
//   0x00687D50, IsEventGameAction 0x00687CF0, GetButtonPressureForEvent 0x00687D10, CTCCombat::IsMeleeFlourishValid
//   0x00733EE0 / IsBreakBlockValid 0x00733290, GetDefGlobalIndexFromName 0x009AD410, CMemoryBuffer::Copy 0x009F1760,
//   CProcessedInput::AddGameEvent 0x00A0D340, free 0x00BFEA14.
// Layout: the landed siblings CInputProcessCombat::ProcessArmedMeleeInput 0x0068ECA0 (CGameEvent, GetInterface,
//   engine/CInputProcessGameBase.h fields) and CInputProcessSpecialAbilities::Update 0x0068FF50; InputLocked +0x218 as
//   CInputProcessUseEnvironment::ProcessInput 0x0068DD00.
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
class CInputEvent;
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);   // 0x009F1760
};
struct CGameEvent : CMemoryBuffer {
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddUWORD(long value) { Copy(2, &value); }
    void AddFloat(float value) { Copy(sizeof(value), &value); }
    void AddBool(bool value) { Copy(1, &value); }
};
class CProcessedInput {
public:
    void AddGameEvent(const CGameEvent& event);    // 0x00A0D340
};
struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                // +0x20
    CInterfaceMap m_interfaces;                    // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                       // +0x91 (bit 0: being destroyed)
    __forceinline unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    __forceinline CTCBase* GetTCC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
class CThingCreatureBase : public CThing { };
bool __fastcall IS_THING_ALIVE(CThing* const& thing);  // 0x0040DCE0
struct CTCCombat {
    bool IsMeleeFlourishValid() const;             // 0x00733EE0
    bool IsBreakBlockValid() const;                // 0x00733290 (descriptive)
};
struct CTCCarrying {
    CThing* GetThingInPrimarySlot();               // 0x006A1440
};
struct CTCWeapon {
    bool IsProjectileWeapon();                     // 0x005DAA40
};
struct CTCBlockable {                              // TC 9 (descriptive)
    unsigned char _pad00[0x0c];
    unsigned char Flags;                           // +0x0c (bit 2: may be shot at)
};
struct CTCTargeting {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual CThingCreatureBase* GetLockedTarget(); // slot 28 (+0x70) descriptive
    virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32();
    virtual CThing* GetTargetedThing();            // slot 33 (+0x84)
    virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37();
    virtual bool IsTargetedThingInRange(long type);   // slot 38 (+0x98)
    bool IsTargetedThingOfType(long type);         // 0x007F5A70
};
struct __true_type { __true_type() { } };
struct CLongVector {                               // STLport vector<long>
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    CLongVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CLongVector() { if (_M_start) free(_M_start); }
    void _M_insert_overflow(long* position, const long& value, const __true_type&, unsigned int count, bool atEnd);   // 0x0068FD80
    void push_back(const long& value)
    {
        if (_M_finish != _M_end_of_storage) {
            if (_M_finish) *_M_finish = value;
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, value, __true_type(), 1, true);
    }
};
class CPlayer {
public:
    unsigned char _pad00[0x218];
    bool          InputLocked;                     // +0x218 (descriptive)
    CThing* GetHeroCreature();                     // 0x00487DC0
};
struct CGamePlayerInterface {
    bool IsAlternateControlScheme();               // 0x00445BC0 (descriptive)
    bool IsAnyActionActive(const CLongVector& actions);   // 0x00446D10 (descriptive)
};
struct CWorld; struct CMainGameComponent;
class CInputProcessGameBase {                      // engine/CInputProcessGameBase.h layout
public:
    virtual void v00();
    unsigned char           _pad04[0x10];          // +0x04 CAInputProcess
    CWorld*                 World;                 // +0x14
    CPlayer*                MyPlayer;              // +0x18
    CDefinitionManager*     DefinitionManager;     // +0x1c
    CMainGameComponent*     Component;             // +0x20
    CGamePlayerInterface*   PGamePlayerInterface;  // +0x24
    bool IsEventButtonPressed(const CInputEvent& event);              // 0x00687A90
    bool IsEventGameAction(long action, const CInputEvent& event);    // 0x00687CF0
    float GetButtonPressureForEvent(const CInputEvent& event);        // 0x00687D10
    bool IsGameActionEventInQueue(long action, long buttonEventType); // 0x00687D50
};
class CInputProcessCombat : public CInputProcessGameBase {
public:
    void ProcessCombatInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessCombat::ProcessCombatInput(const CInputEvent& event, CProcessedInput& processed)
{
    if (MyPlayer->InputLocked)
        return;
    CThing* hero = MyPlayer->GetHeroCreature();
    CTCCombat* combat = 0;
    hero->GetInterface(&combat, 0x30);
    CTCTargeting* targeting = 0;
    hero->GetInterface(&targeting, 8);
    CThingCreatureBase* target = targeting->GetLockedTarget();
    bool canAttack = true;
    if (hero->HasTC(0x46)) {
        CThing* item = ((CTCCarrying*)hero->GetTCC(0x46))->GetThingInPrimarySlot();
        if (item && !(item->m_flags91 & 1) && item->HasTC(0x48)
            && ((CTCWeapon*)item->GetTCC(0x48))->IsProjectileWeapon()) {
            canAttack = false;
            if (IS_THING_ALIVE(target) && target->HasTC(9) && (((CTCBlockable*)target->GetTCC(9))->Flags & 4))
                canAttack = true;
        }
    }
    CLongVector actions;
    actions.push_back(6);
    actions.push_back(9);
    if (PGamePlayerInterface->IsAnyActionActive(actions)) {
        targeting->GetTargetedThing();
        if (!(targeting->IsTargetedThingOfType(0x10) && targeting->IsTargetedThingInRange(0x10))) {
            if ((targeting->IsTargetedThingOfType(2) && targeting->IsTargetedThingInRange(2))
                || (targeting->IsTargetedThingOfType(1) && targeting->IsTargetedThingInRange(1)))
                canAttack = false;
        }
    }
    if (!IsEventButtonPressed(event))
        return;
    if (canAttack) {
        if ((!PGamePlayerInterface->IsAlternateControlScheme() || IsGameActionEventInQueue(9, 2))
            && IsEventGameAction(8, event) && combat->IsMeleeFlourishValid()) {
            CGameEvent gameEvent(0x16);
            gameEvent.AddUWORD(DefinitionManager->GetDefGlobalIndexFromName("CREATURE_ABILITY_FLOURISH"));
            gameEvent.AddFloat(GetButtonPressureForEvent(event));
            processed.AddGameEvent(gameEvent);
        } else if (IsEventGameAction(8, event) && combat->IsBreakBlockValid()) {
            CGameEvent gameEvent(0x16);
            gameEvent.AddUWORD(DefinitionManager->GetDefGlobalIndexFromName("CREATURE_ABILITY_BREAK_BLOCK"));
            gameEvent.AddFloat(GetButtonPressureForEvent(event));
            processed.AddGameEvent(gameEvent);
        } else if (IsEventGameAction(9, event)) {
            CGameEvent gameEvent(0x16);
            gameEvent.AddUWORD(DefinitionManager->GetDefGlobalIndexFromName("CREATURE_ABILITY_ATTACK"));
            gameEvent.AddFloat(GetButtonPressureForEvent(event));
            processed.AddGameEvent(gameEvent);
        }
    }
    if (IsEventGameAction(0xe, event)) {
        CGameEvent gameEvent(0x22);
        gameEvent.AddBool(true);
        processed.AddGameEvent(gameEvent);
    } else if (IsEventGameAction(0xd, event)) {
        CGameEvent gameEvent(0x21);
        gameEvent.AddBool(true);
        processed.AddGameEvent(gameEvent);
    } else if (IsEventGameAction(0xf, event) || IsEventGameAction(0x10, event)) {
        CGameEvent gameEvent(0x20);
        processed.AddGameEvent(gameEvent);
    }
}
