#pragma optimize("y", off)
#pragma optimize("s", on)
// NPlayerGui::CDrawQuickAccessIcons::GetActivationTimeForButton(long button) -- the activation time of the item in a quick-access slot of the living hero's inventory (2.0 for the special button -2, 0 when there is no hero or no item)
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- the interface lookup written as the ternary entry pick (`((it == last || type < it->type) ? last : it)->component`, the landed 0x00584157 idiom): the component loads straight into ecx
// Leaf: GetActivationTimeForButton
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad1[0x23] removed.
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064B5C1, length 165 bytes (+ padding), `ret 4`.
// Callees: CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::PeekPControlledCreature @ 0x00487DD0,
// IS_THING_ALIVE<T*>(const T*&) @ 0x0043DBF0 (`__fastcall`), std::bitset::test @ 0x00410DE0, CVectorMap<ETCInterfaceType,
// CTCBase*>::LowerBound @ 0x004365B0, CTCInventory::GetQuickAccessItemDef(long) @ 0x00581DDB (CDefPointer by hidden
// pointer), the def's virtual slot 1 when its count reaches zero. Float constants 2.0f @ 0x0122DCDC, 0.0f @ 0x0122DEDC.
// Layout: CDrawQuickAccessIcons player manager +8 (CDrawBase); interface 0x11 = the inventory (CThing bits +0x20, map
// +0x44); the item def's activation time (float) at +0x48, its reference count a base at +4.
// Shape: a frame-pointer size TU; the alive test binds the hero through a converted temporary; the inventory comes from
// an inline accessor that leaves its result unset when the interface is absent (VC loads the dead `button` slot there);
// the def is a named CDefPointer whose destructor runs the inline ReleaseTemporary (`add eax,4 ; dec [eax] ; cmp
// [eax],0`); the time is a named float local parked in the dead argument slot across the release.
class C2DVector { public: float X, Y; };
class CTCBase;
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad0[0x20];
    std::bitset<274> m_present;                       // +0x20
    CInterfaceMap m_interfaces;                    // +0x44
    bool HasInterface(long type) const { return m_present.test(type); }
    template <class T> __forceinline T* GetInterfaceUnchecked(long type) const
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::value_type* it = map->LowerBound(key);
        return (T*)((it == map->end() || type < it->first) ? map->end() : it)->second;
    }
};
template <class T> __declspec(noinline) bool __fastcall IS_THING_ALIVE(const T& thing)   // 0x0043DBF0
{
    return thing != 0 && !(reinterpret_cast<const unsigned char*>(thing)[0x91] & 1);   // CThing dead bit +0x91
}
class CThingCreatureBase : public CThing { public:
    bool IsChild() const;                                // 0x00661F70
};
class CThingPlayerCreature : public CThingCreatureBase { };
class CPlayer { public:
    const CThingPlayerCreature* PeekPControlledCreature() const;   // 0x00487DD0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                        // slot 1
};
class CInventoryItemDef : public CDefVirtual, public CRefCounted { public:
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
class CTCInventory { public:
    CDefPointer GetQuickAccessItemDef(long slot);  // 0x00581DDB
};
__forceinline CTCInventory* GetInventory(const CThing* thing)
{
    CTCInventory* inventory;
    if (thing->HasInterface(0x11))
        inventory = thing->GetInterfaceUnchecked<CTCInventory>(0x11);
    return inventory;
}

namespace NPlayerGui {
class CDrawQuickAccessIcons { public:
    unsigned char _pad[8];
    CPlayerManager* PlayerManager;                 // +0x08
    float GetActivationTimeForButton(long button);
};

float CDrawQuickAccessIcons::GetActivationTimeForButton(long button)
{
    const CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().PeekPControlledCreature();
    if (IS_THING_ALIVE(hero)) {
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