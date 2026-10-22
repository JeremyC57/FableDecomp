// Grade: DIFFER(547v550) -- VC keeps the constant 1 in ebx (dead-bit test, Valid/AddBool stores) where retail keeps the event in ebx; the 0x81/0xA7 lookup keys then miss the dead event slot (frame 0x34 vs 0x30) and the float pressure is not routed through the event slot
// CInputProcessUseRangedWeapon::ProcessRangedAttackInput(const CInputEvent& event, CProcessedInput& processed, CThing* weapon) -- drive the hero's ranged attack: on the fire action (0xA) start, charge and continue aiming or fire (unless the inventory or weapon blocks it); otherwise a press of action 0x10 / 0xD queues game event 0x20 / 0x21
// Leaf: ProcessRangedAttackInput
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x0068EF50, 560 bytes, `ret 0Ch` (/GS cookie 0x0139C8A8, __security_check_cookie 0x00BFE9F9).
class CInputEvent;
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
    void AddBool(bool value) { Copy(1, &value); }
};
class CProcessedInput {
public:
    void AddGameEvent(const CGameEvent& event);    // 0x00A0D340
    void SetHandled();                             // 0x00A0D390
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
// Same-TU stand-ins for CVectorMap::LowerBound 0x0040F020 / 0x004365B0 (not the retail bodies): only their
// visibility matters, they let the lookup keys share frame words.
#pragma auto_inline(off)
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - it;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) { it += half + 1; count -= half + 1; }
        else count = half;
    }
    return it;
}
__declspec(noinline) const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* it = first;
    long count = last - it;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) { it += half + 1; count -= half + 1; }
        else count = half;
    }
    return it;
}
#pragma auto_inline(on)
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                // +0x20
    CInterfaceMap m_interfaces;                    // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                        // +0x91 bit 0
    template<class T> __forceinline T* GetConstInterface(long interfaceType) const
    {
        T* result = 0;
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            result = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
        }
        return result;
    }
    template<class T> __forceinline T* GetInterface(long interfaceType)
    {
        T* result = 0;
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            result = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
        }
        return result;
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
struct CTCInventoryBase {
    bool IsBusy();                                 // 0x007331E0 (manifest "GetCanItemBeQuickAssigned", propagated)
};
struct CTCRangedWeaponState {
    unsigned char _pad00[0xc];
    bool          Blocked;                         // +0x0C (descriptive)
};
struct CTCHeroRangedAttacks {
    void StartAimingRangedWeapon();                // 0x007899D0
    void SetButtonPressure(float pressure);        // 0x007899E0
    void ContinueAimingRangedWeapon();             // 0x00789FD0
    void AttemptToFireRangedWeapon();              // 0x0078A5D0
};
struct CPlayer {
    unsigned char _pad00[0x218];
    bool          InRangedWeaponMode;              // +0x218 (descriptive)
    CThing* GetHeroCreature();                     // 0x00487DC0
};
struct CWorld; struct CGameDefinitionManager; struct CMainGameComponent; struct CGamePlayerInterface;
class CInputProcessGameBase {
public:
    virtual void v00();
    unsigned char           _pad04[0x10];
    CWorld*                 World;                 // +0x14
    CPlayer*                MyPlayer;              // +0x18
    CGameDefinitionManager* DefinitionManager;     // +0x1c
    CMainGameComponent*     Component;             // +0x20
    CGamePlayerInterface*   PGamePlayerInterface;  // +0x24
    bool IsEventButtonPressed(const CInputEvent& event);              // 0x00687A90
    bool IsEventButtonHeld(const CInputEvent& event);                 // 0x00687B30
    bool IsEventGameAction(long action, const CInputEvent& event);    // 0x00687CF0
    float GetButtonPressureForEvent(const CInputEvent& event);        // 0x00687D10
};
class CInputProcessUseRangedWeapon : public CInputProcessGameBase {
public:
    void ProcessRangedAttackInput(const CInputEvent& event, CProcessedInput& processed, CThing* weapon);
};

void CInputProcessUseRangedWeapon::ProcessRangedAttackInput(const CInputEvent& event, CProcessedInput& processed, CThing* weapon)
{
    CTCInventoryBase* inventory = 0;
    CThing* hero = MyPlayer->GetHeroCreature();
    if (hero && !hero->Dead)
        hero->GetInterface(&inventory, 0x30);
    if (IsEventGameAction(0xa, event)) {
        CTCHeroRangedAttacks* rangedAttacks = hero->GetInterface<CTCHeroRangedAttacks>(0x81);
        CTCRangedWeaponState* state = (CTCRangedWeaponState*)inventory;
        hero->GetInterface(&state, 0xa7);
        if (inventory->IsBusy() || MyPlayer->InRangedWeaponMode || state->Blocked) {
            processed.SetHandled();
            return;
        }
        if (!IsEventButtonHeld(event) && !IsEventButtonPressed(event)) {
            rangedAttacks->AttemptToFireRangedWeapon();
            processed.SetHandled();
            return;
        }
        if (IsEventButtonPressed(event))
            rangedAttacks->StartAimingRangedWeapon();
        rangedAttacks->SetButtonPressure(GetButtonPressureForEvent(event));
        rangedAttacks->ContinueAimingRangedWeapon();
    } else if (IsEventButtonPressed(event)) {
        if (IsEventGameAction(0x10, event)) {
            CGameEvent gameEvent(0x20);
            processed.AddGameEvent(gameEvent);
        } else if (IsEventGameAction(0xd, event)) {
            CGameEvent gameEvent(0x21);
            gameEvent.AddBool(true);
            processed.AddGameEvent(gameEvent);
        }
    }
}
