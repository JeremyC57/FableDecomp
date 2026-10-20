// Grade: DIFFER(305v306) -- register colouring and one frame slot: retail keeps the hero in ebx and the lookup map/key in edi, the lane swaps them; the sound lookup key lands in the dead `start` slot in retail but in the freed item-def slot [ebp-4] in the lane (and the key 0x10 is kept in a register for the compare). The item-def release (`add eax,4 ; dec [eax] ; cmp [eax],0`, pointee DecRef/GetRefCount) and everything else match. Tried: local-copy release, a condition temporary (fixes the colouring but adds a sete), an early-declared key.
#pragma optimize("y", off)
#pragma optimize("s", on)
// NPlayerGui::CDrawQuickAccessIcons::ToggleCharge(long ability, bool start) -- start charging a quick-access ability (validated against the hero's inventory unless -2), playing the charge-up sound when none is running, or stop charging (sound cleared, no ability)
// Leaf: ToggleCharge
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064D92A, length 304 bytes (`ret 8`).
// Callees: CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetCharacterThing @ 0x00487DC0, IS_THING_ALIVE<CThing*> @ 0x0043DBD0
// (`__fastcall`), std::bitset::test @ 0x00410DE0, the interface map LowerBound const @ 0x004365B0 / non-const @ 0x0040F020,
// CTCInventory::GetQuickAccessItemDef(long) @ 0x00581DDB (CDefPointer<const CInventoryItemDef> by hidden pointer), CCharString(const
// char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CSoundInfo::PlaySound @ 0x0066BD00 (on the sound component),
// CDrawQuickAccessIcons::ClearSoundEffects @ 0x0064B668 (landed); "GUILD_SEAL_CHARGEUP" @ 0x0125A5AC.
// Layout: as the landed ClearSoundEffects 0x0064B668 (PlayerManager +8, ChargingSoundID +0x28; the thing's interface bitset +0x20 and
// map +0x44); ChargingAbility +0x2c, ChargeLevel +0x30, Charging +0x34, ChargeComplete +0x35 (descriptive, from ToggleFlash/Update).
// Shape: frame-pointer size TU; the inventory lookup is `T* p = 0; if (HasTC) p = GetTC;` through the const LowerBound, the sound
// lookup through the non-const one; the item def is released through its address (`dec [eax] ; cmp [eax],0`); the IS_THING_ALIVE
// temporary, the lookup keys and the sound-name string share the dead `start` argument slot.
struct CTCBase;

struct CPresenceBits {
    bool test(long bit);                           // 0x00410DE0 (std::bitset::test)
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);          // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};

struct CThing {
    unsigned char _pad0[0x20];
    CPresenceBits m_present;                       // +0x20
    unsigned char _pad1[0x23];
    CInterfaceMap m_interfaces;                    // +0x44
    bool HasInterface(long type) { return m_present.test(type); }
    template <class T> __forceinline T* GetInterfaceUnchecked(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        if (it == map->last || type < it->type)
            it = map->last;
        return (T*)it->component;
    }
    template <class T> __forceinline T* GetInterfaceUncheckedConst(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        if (it == map->last || type < it->type)
            it = map->last;
        return (T*)it->component;
    }
};

template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x0043DBD0 (CThing*)

struct CThingCreatureBase : CThing { };

struct CPlayer {
    CThingCreatureBase* GetCharacterThing();       // 0x00487DC0
};

struct CPlayerManager {
    CPlayer* GetMainPlayer();                      // 0x00449970
};

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length);    // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};

struct CTCSound {                                  // the thing's interface 0x10
    unsigned long PlaySound(const CCharString& name, bool loop, long a, long b, long c);   // 0x0066BD00
};

struct CInventoryItemDef {
    virtual void v00();
    virtual void Destroy();                        // slot 1
    long RefCount;                                 // +0x04
    __forceinline void DecRef() { --RefCount; }
    __forceinline long GetRefCount() const { return RefCount; }
    __forceinline void Release() { DecRef(); if (GetRefCount() == 0) Destroy(); }
};
struct CItemDefPointer {                           // CDefPointer<const CInventoryItemDef>
    CInventoryItemDef* Object;
    __forceinline ~CItemDefPointer()
    {
        if (Object)
            Object->Release();
    }
};

struct CTCInventory {                              // the thing's interface 0x11
    CItemDefPointer GetQuickAccessItemDef(long ability);   // 0x00581DDB
};

enum { TC_SOUND = 0x10, TC_INVENTORY = 0x11 };

namespace NPlayerGui {

struct CDrawQuickAccessIcons {
    void*           _vtable;                       // +0x00
    void*           PlayerGui;                     // +0x04
    CPlayerManager* PlayerManager;                 // +0x08
    unsigned char   _base0c[0x28 - 0x0c];
    unsigned long   ChargingSoundID;               // +0x28
    long            ChargingAbility;               // +0x2c
    float           ChargeLevel;                   // +0x30
    bool            Charging;                      // +0x34
    bool            ChargeComplete;                // +0x35
    void ClearSoundEffects();                      // 0x0064B668
    void ToggleCharge(long ability, bool start);
};

void CDrawQuickAccessIcons::ToggleCharge(long ability, bool start)
{
    if (start) {
        CThingCreatureBase* hero = PlayerManager->GetMainPlayer()->GetCharacterThing();
        if (!IS_THING_ALIVE<CThing*>(hero))
            return;
        if (ability != -2) {
            CTCInventory* inventory = 0;
            if (hero->HasInterface(TC_INVENTORY))
                inventory = hero->GetInterfaceUncheckedConst<CTCInventory>(TC_INVENTORY);
            CItemDefPointer item = inventory->GetQuickAccessItemDef(ability);
            if (!item.Object)
                return;
        }
        if (ChargingSoundID == 0) {
            if (hero->HasInterface(TC_SOUND)) {
                CTCSound* sound = hero->GetInterfaceUnchecked<CTCSound>(TC_SOUND);
                CCharString name("GUILD_SEAL_CHARGEUP", -1);
                ChargingSoundID = sound->PlaySound(name, true, 0, 0, -1);
            }
        }
        ChargeLevel = 0.0f;
        ChargingAbility = ability;
        Charging = true;
        ChargeComplete = false;
    } else {
        ClearSoundEffects();
        ChargingAbility = -1;
        Charging = false;
    }
}

}
