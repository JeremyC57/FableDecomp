// CTCInventoryQuests::TeleportToBoastingPodium() -- teleport the hero to the boasting podium through its inventory map
// Leaf: ?TeleportToBoastingPodium@CTCInventoryQuests@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0061A360, 76 bytes, thiscall, plain `ret`.
// Identity (proven): Ego_r.pdb ?TeleportToBoastingPodium@CTCInventoryQuests@@QAEXXZ: the body sits in the CTCInventoryQuests TU (between GetContinuedCategoryRepeatDelay 0x0061A100 and DrawPreparePrimitivesForRendering 0x0061B3C0), fetches the hero's inventory map (interface 0x1B, as the landed CInputProcessInventoryMapScreen::Init) and calls the Ego_r.pdb const CTCInventoryMap::TeleportHeroToBoastingArea 0x005FB4B0; its caller (the front-end action handler 0x0041E6D3) reaches it through the hero's interface 0x19 component.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (the inline CThing::GetInterface, interface 0x1B),
//   CTCInventoryMap::TeleportHeroToBoastingArea() 0x005FB4B0.
// Shape (family M169, representative 0x0061A360): `CTCInventoryMap* tc; ParentThing->GetInterface(&tc, 0x1B); tc->TeleportHeroToBoastingArea();` --
//   the GetInterface result is not tested (an absent component calls through the uninitialised `push ecx` word, as retail
//   does); the GetInterface of the landed CTCWillLightning::StartLoopingSound 0x007A0400. Speed TU.
#pragma warning(disable: 4700)
class CTCBase;
class CTCInventoryMap {
public:
    void TeleportHeroToBoastingArea() const;                                   // 0x005FB4B0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                 // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                       // +0x20
    CInterfaceMap m_interfaces;                           // +0x44
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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
};
class CTCInventoryQuests {
public:
    void* __vftable;                                      // +0x00
    CThing* ParentThing;                                  // +0x04 (CTCBase)
    void TeleportToBoastingPodium();
};
void CTCInventoryQuests::TeleportToBoastingPodium()
{
    CTCInventoryMap* tc;
    ParentThing->GetInterface(&tc, 0x1B);
    tc->TeleportHeroToBoastingArea();
}