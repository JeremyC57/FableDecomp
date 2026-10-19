#pragma optimize("y", off)
#pragma optimize("s", on)
// NPlayerGui::CDrawQuickAccessIcons::GetActivationTimeForButton(long button) -- the activation time of the item in a quick-access slot of the living hero's inventory (2.0 for the special button -2, 0 when there is no hero or no item)
// Grade: DIFFER(169v167) -- retail loads the looked-up inventory straight into ecx on both arms (`mov ecx,[eax+4]` / `mov ecx,[ebp+8]`) and forms the hidden result pointer in eax; the lane lands it in eax and copies (accessor, early-return accessor, function-scope local and reference-bound forms tried; the GOTCHAS 280/286 lane gap)
// Leaf: GetActivationTimeForButton
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064B5C1, length 165 bytes (+ padding), `ret 4`.
// Callees: CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetControlledCreature @ 0x00487DD0,
// IS_THING_ALIVE<T*>(const T*&) @ 0x0043DBF0 (`__fastcall`), std::bitset::test @ 0x00410DE0, CVectorMap<ETCInterfaceType,
// CTCBase*>::LowerBound @ 0x004365B0, CTCInventory::GetQuickAccessItemDef(long) @ 0x00581DDB (CDefPointer by hidden
// pointer), the def's virtual slot 1 when its count reaches zero. Float constants 2.0f @ 0x0122DCDC, 0.0f @ 0x0122DEDC.
// Layout: CDrawQuickAccessIcons player manager +8 (CDrawBase); interface 0x11 = the inventory (CThing bits +0x20, map
// +0x44); the item def's activation time (float) at +0x48, its reference count a base at +4.
// Shape: a frame-pointer size TU; the alive test binds the hero through a converted temporary; the inventory comes from
// an inline accessor that leaves its result unset when the interface is absent (VC loads the dead `button` slot there);
// the def is a named CDefPointer whose destructor runs the inline ReleaseTemporary (`add eax,4 ; dec [eax] ; cmp
// [eax],0`); the time is a named float local parked in the dead argument slot across the release.
struct C2DVector { float X, Y; };
struct CTCBase;
struct CPresenceBits {
    bool test(long bit);                           // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);          // 0x004365B0
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
};
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x0043DBF0
struct CThingCreatureBase : CThing {
    bool IsChild();                                // 0x00661F70
};
struct CPlayer {
    CThingCreatureBase* GetControlledCreature();   // 0x00487DD0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                      // 0x00449970
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
    ~CDefPointer()
    {
        if (m_p)
            m_p->ReleaseTemporary();
    }
    CInventoryItemDef* operator->() const { return m_p; }
};
struct CTCInventory {
    CDefPointer GetQuickAccessItemDef(long slot);  // 0x00581DDB
};
__forceinline CTCInventory* GetInventory(CThing* thing)
{
    CTCInventory* inventory;
    if (thing->HasInterface(0x11))
        inventory = thing->GetInterfaceUnchecked<CTCInventory>(0x11);
    return inventory;
}

namespace NPlayerGui {
struct CDrawQuickAccessIcons {
    unsigned char _pad[8];
    CPlayerManager* PlayerManager;                 // +0x08
    float GetActivationTimeForButton(long button);
};

float CDrawQuickAccessIcons::GetActivationTimeForButton(long button)
{
    CThingCreatureBase* hero = PlayerManager->GetMainPlayer()->GetControlledCreature();
    if (IS_THING_ALIVE<CThing*>(hero)) {
        if (button == -2)
            return 2.0f;
        CDefPointer def = GetInventory(hero)->GetQuickAccessItemDef(button);
        if (def.m_p) {
            float time = def->ActivationTime;
            return time;
        }
    }
    return 0.0f;
}
}
