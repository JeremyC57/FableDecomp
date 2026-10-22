// Grade: DIFFER(875v902) -- VC CSEs the -1 constant into ebp from the start (retail pushes ebx/ebp lazily: ebx after the first test, ebp only on the charge path, and keeps the special flag in the dead event slot); the rest of the flow is modelled
// CInputProcessQuickAccessItems::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- on a press of a quick-access button (or the special-move action 0x4e, slot -2) start charging the slot: cancel any pending charge, then queue game event 0x29 (charge, with the slot and a true byte) when the slot's item has an activation time (2.0 for guild seals, which need one in the inventory and the world flag +0xd5) or event 0x27 (use now); an assignable special-move button queues event 0x27 with its index mod 4
// Retail: Fable.exe (TLC, VC7.1) @ 0x0068E510, 902 bytes, `ret 8` (/GS). Callees: IsEventButtonPressed 0x00687A90, IsEventGameAction
//   0x00687CF0, IsEventQuickAccessItemButton 0x00687C80 / IsOtherQuickAccessItemButtonHeld 0x00687C90 (forwarders to
//   CGamePlayerInterface 0x00446000 / 0x00446060), 0x00687CE0 (forwarder to 0x00446110), CPlayer::GetControlledCreature
//   0x00487DD0, CPlayerGui 0x004374F0 on 0x013B8790, GetNumContextSensitiveSlots 0x00445960, CancelQuickAccessCharge 0x0068E3C0,
//   CTCInventory GetQuickAccessItem 0x00581E33 / GetQuickAccessItemDef 0x00581DDB / GetNumberOfItemsOfTypeInInventory 0x005BDF08,
//   GetDefGlobalIndexFromName 0x009AD410 ("OBJECT_GUILD_SEAL_1" 0x01251B4C), Copy 0x009F1760, AddGameEvent 0x00A0D340.
// Leaf: ProcessInput
// Flags: /GS
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
    CGameEvent() : EndPos(0), Valid(true), Replacement(false) { }
    void AddLong(long value) { Copy(sizeof(value), &value); }
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
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                        // slot 1
};
struct CInventoryItemDef : CDefVirtual, CRefCounted {
    unsigned char _def[0x40];                      // +0x08
    float ActivationTime;                          // +0x48
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CDefPointer {
    CInventoryItemDef* m_p;
    __forceinline ~CDefPointer()
    {
        if (m_p)
            m_p->ReleaseTemporary();
    }
    CInventoryItemDef* operator->() const { return m_p; }
};
struct CTCInventory {
    long GetQuickAccessItem(unsigned long index);  // 0x00581E33
    CDefPointer GetQuickAccessItemDef(long slot);  // 0x00581DDB
    long GetNumberOfItemsOfTypeInInventory(long itemType);   // 0x005BDF08 (CTCInventoryBase)
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                // +0x20
    CInterfaceMap m_interfaces;                    // +0x44
    unsigned char _pad4c[0x90 - 0x4c];
    signed char   PlayerNumber;                    // +0x90
    unsigned char Dead : 1;                        // +0x91 bit 0
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
class CPlayer {
public:
    CThing* GetControlledCreature();               // 0x00487DD0
};
struct CPlayerGui {
    bool IsOpen() const;                           // 0x004374F0
};
extern CPlayerGui* g_pPlayerGui;                   // 0x013B8790
struct CGamePlayerInterface {
    unsigned long GetNumContextSensitiveSlots() const;   // 0x00445960
};
struct CWorld {
    unsigned char       _pad00[0x10];
    CDefinitionManager* DefinitionManager;         // +0x10 (descriptive)
    unsigned char       _pad14[0xd5 - 0x14];
    bool                GuildSealUsable;           // +0xD5 (descriptive)
};
extern long g_QuickAccessChargeSlot;               // 0x0137EE78
extern long g_QuickAccessChargeButton;             // 0x0137EE7C
struct CMainGameComponent;
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
    bool IsEventQuickAccessItemButton(const CInputEvent& event, long& button);   // 0x00687C80
    bool IsOtherQuickAccessItemButtonHeld(long button);               // 0x00687C90
    bool IsEventAssignableSpecialMoveButton(const CInputEvent& event, long& button);   // 0x00687CE0 (descriptive)
};
class CInputProcessQuickAccessItems : public CInputProcessGameBase {
public:
    void CancelQuickAccessCharge();                // 0x0068E3C0
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessQuickAccessItems::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    if (!IsEventButtonPressed(event))
        return;
    long button = -1;
    bool special = IsEventGameAction(0x4e, event);
    if (special)
        button = -2;
    if (!IsEventQuickAccessItemButton(event, button) && !special) {
        if (!IsEventAssignableSpecialMoveButton(event, button))
            return;
        button %= 4;
        CThing* creature = MyPlayer->GetControlledCreature();
        if (!creature || creature->Dead)
            return;
        CGameEvent gameEvent;
        gameEvent.Type = 0x27;
        gameEvent.Player = creature->PlayerNumber;
        gameEvent.AddLong(button);
        processed.AddGameEvent(gameEvent);
        return;
    }
    {
        long originalButton = button;
        CThing* creature = MyPlayer->GetControlledCreature();
        if (!creature || creature->Dead)
            return;
        if (IsOtherQuickAccessItemButtonHeld(button))
            return;
        if (!g_pPlayerGui->IsOpen())
            return;
        if (!special)
            button += PGamePlayerInterface->GetNumContextSensitiveSlots();
        if (g_QuickAccessChargeSlot != -1)
            CancelQuickAccessCharge();
        g_QuickAccessChargeSlot = -1;
        g_QuickAccessChargeButton = -1;
        CTCInventory* inventory = 0;
        creature->GetInterface(&inventory, 0x11);
        bool guildSeal = special;
        if (!guildSeal) {
            long itemType = inventory->GetQuickAccessItem(button);
            if (itemType > 0) {
                long sealType = World->DefinitionManager->GetDefGlobalIndexFromName("OBJECT_GUILD_SEAL_1");
                if (itemType == sealType)
                    guildSeal = true;
            }
        }
        float chargeTime = 2.0f;
        if (guildSeal) {
            long sealType = World->DefinitionManager->GetDefGlobalIndexFromName("OBJECT_GUILD_SEAL_1");
            if (inventory->GetNumberOfItemsOfTypeInInventory(sealType) <= 0)
                return;
            if (!World->GuildSealUsable)
                return;
        } else {
            CDefPointer def = inventory->GetQuickAccessItemDef(button);
            if (!def.m_p)
                return;
            chargeTime = def->ActivationTime;
        }
        CGameEvent gameEvent;
        if (chargeTime > 0.0001f) {
            gameEvent.Type = 0x29;
            gameEvent.Player = creature->PlayerNumber;
            gameEvent.AddLong(button);
            gameEvent.AddBool(true);
            processed.AddGameEvent(gameEvent);
            g_QuickAccessChargeSlot = button;
            g_QuickAccessChargeButton = originalButton;
        } else {
            gameEvent.Type = 0x27;
            gameEvent.Player = creature->PlayerNumber;
            gameEvent.AddLong(button);
            processed.AddGameEvent(gameEvent);
        }
    }
}
