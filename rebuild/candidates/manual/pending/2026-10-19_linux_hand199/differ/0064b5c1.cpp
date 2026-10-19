#pragma optimize("y", off)
#pragma optimize("s", on)
// NPlayerGui::CDrawQuickAccessIcons::GetActivationTimeForButton(long button) -- the activation time of the quick-access item bound to a button on the hero's inventory: 2.0 for button -2, the item def's ActivationTime when the slot holds an item, else 0.0 (also when the hero is not alive)
// Grade: DIFFER(169v167) -- the inventory pointer (and its absent-path garbage read of the `button` slot) lands in eax and is moved to ecx after the hidden-result lea; retail loads it straight into ecx (`mov ecx,[eax+4]` / `mov ecx,[ebp+8]`) and builds the result address in eax
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064B5C1, length 167 bytes (ret 4; ebp frame closed with `leave`).
// Pragmas: `#pragma optimize("y", off)` + `#pragma optimize("s", on)` (the HUD TU, as the landed sibling 0x0064B668).
// Callees: CPlayerManager::GetMainPlayer() 0x00449970, CPlayer::GetControlledCreature() 0x00487DD0 (landed names; manifest
// labels on both are propagated junk), IS_THING_ALIVE<CThingPhysical*> 0x0043DBF0 (`__fastcall`, by reference; T is a guess -- the
// manifest's <CThingBuilding*> is a byte-twin label, <CThing*> is 0x0043DBD0), std::bitset::test 0x00410DE0, CVectorMap::LowerBound (const) 0x004365B0
// (interface 0x11 = CTCInventory), CTCInventory::GetQuickAccessItemDef(long) 0x00581DDB (CDefPointer<const CInventoryItemDef>
// by hidden pointer); the def is released through its virtual slot 1. Float pool: 2.0f 0x0122DCDC, 0.0f 0x0122DEDC.
// Layout: CDrawBase PlayerManager +8; CInventoryItemDef ActivationTime +0x48 (PDB).
// Shape: the ClearSoundEffects (0x0064B668) hero/alive/presence shape: the alive test binds its reference to a converted
// pointer temporary in the frame, so the hero itself stays in esi; the inventory is an uninitialised pointer filled only
// on the presence path (the absent path passes whatever its frame word, the dead `button` slot, holds); the def pointer's
// temporary-release (`add eax,4 ; dec [eax] ; cmp [eax],0`) runs with the activation time parked in the `button` slot.
struct CTCBase;

struct CPresenceBits {
    bool test(long bit);                           // 0x00410DE0 (std::bitset::test)
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;    // 0x004365B0 (const copy)
};

struct CThing {
    unsigned char _pad0[0x20];
    CPresenceBits m_present;                       // +0x20
    unsigned char _pad1[0x23];
    CInterfaceMap m_interfaces;                    // +0x44
    bool HasInterface(long type) { return m_present.test(type); }
    template <class T> __forceinline T* GetInterfaceUnchecked(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        if (it == map->last || type < it->type)
            it = map->last;
        return (T*)it->component;
    }
};

struct CThingPhysical : CThing { };
struct CThingCreatureBase : CThingPhysical { };

template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x0043DBF0 (CThingPhysical*)

struct CPlayer {
    CThing* GetControlledCreature();               // 0x00487DD0
};

struct CPlayerManager {
    CPlayer* GetMainPlayer();                      // 0x00449970
};

struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                        // slot 1
};
struct CRefCounted {                               // second base: the count at +4 behind the vptr
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CInventoryItemDef : CDefVirtual, CRefCounted {
    unsigned char _pad08[0x48 - 8];
    float         ActivationTime;                  // +0x48
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> struct CDefPointer {
    T* m_p;
    ~CDefPointer()
    {
        if (m_p)
            m_p->ReleaseTemporary();
    }
    T* operator->() const { return m_p; }
};

struct CTCInventory {                              // the thing's interface 0x11
    CDefPointer<CInventoryItemDef> GetQuickAccessItemDef(long button);   // 0x00581DDB
};

enum { TC_INVENTORY = 0x11 };

namespace NPlayerGui {

struct CDrawQuickAccessIcons {
    void*           _vtable;                       // +0x00
    void*           PlayerGui;                     // +0x04
    CPlayerManager* PlayerManager;                 // +0x08
    float GetActivationTimeForButton(long button);
};

float CDrawQuickAccessIcons::GetActivationTimeForButton(long button)
{
    CThingCreatureBase* hero = static_cast<CThingCreatureBase*>(PlayerManager->GetMainPlayer()->GetControlledCreature());
    if (IS_THING_ALIVE<CThingPhysical*>(hero)) {
        if (button == -2)
            return 2.0f;
        CTCInventory* inventory;
        if (hero->HasInterface(TC_INVENTORY))
            inventory = hero->GetInterfaceUnchecked<CTCInventory>(TC_INVENTORY);
        CDefPointer<CInventoryItemDef> def = inventory->GetQuickAccessItemDef(button);
        if (def.m_p) {
            float time = def->ActivationTime;
            return time;
        }
    }
    return 0.0f;
}

}
