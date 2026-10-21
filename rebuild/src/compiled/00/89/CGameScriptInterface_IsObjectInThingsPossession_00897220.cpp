// CGameScriptInterface::IsObjectInThingsPossession(const CCharString& objectDefName, const CScriptThing& thing) const -- true when the thing holds an object of the named def: in its inventory (TC 0x11), carried (TC 0x46), in its two other item stores (TC 0x13 / 0x12) or in its house (TC 0x3C)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00897220, length 371 bytes (`ret 8`, bool in al).
// Callees: CDefinitionManager::GetDefGlobalIndexFromName @ 0x009AD410, the script thing's slot 11 (+0x2c) GetThing,
//   the const component-map LowerBound @ 0x004365B0, CTCInventoryBase::GetNumberOfItemsOfTypeInInventory(long)
//   @ 0x005BDF08 (TC 0x11, 0x13, 0x12), CTCCarrying::IsCarryingThing(long) @ 0x006A10A0 (TC 0x46), and the TC 0x3C
//   component's bool query @ 0x008ED010 (catalog "CTCBuyableHouse::MovedInNotification" is propagated; descriptive name).
// Layout: CThing UsingComponents +0x20 (bitset words), component map +0x44 {first, last} (the landed CloseDoor 0x00895DE0 model).
// Shape: each component test reads its mask word (`test dword` / `test byte` narrowed for TC 0x46), the lookup key is
//   homed in the dead objectDefName argument slot, the map pointer is a named pointer in edi.
struct CCharString;
struct CTCBase_Retail;
struct CInterfaceMap_Retail {                 // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase_Retail* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
struct CThing_Retail {
    unsigned char         _pad00[0x20];
    unsigned long         m_presentBits[9];         // +0x20 std::bitset of the interface types present
    CInterfaceMap_Retail  m_interfaces;             // +0x44
    unsigned long HasInterface(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    __forceinline CTCBase_Retail* GetInterface(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap_Retail* map = &m_interfaces;
        const CInterfaceMap_Retail::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};
struct CTCInventoryBase { long GetNumberOfItemsOfTypeInInventory(long defIndex); };   // 0x005BDF08
struct CTCCarrying { bool IsCarryingThing(long defIndex); };                          // 0x006A10A0
struct CTCHouseContents { bool ContainsItemOfType(long defIndex); };                  // 0x008ED010
struct CScriptThing {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing_Retail* GetThing() const;        // slot 11 (+0x2c)
};
struct CDefinitionManager_Retail { long GetDefGlobalIndexFromName(const CCharString& name); };   // 0x009AD410
struct CGameScriptInterface {
    virtual void v000();
    void*                       World;              // +0x04 (engine/CGameScriptInterface.h)
    void*                       Component;          // +0x08
    void*                       DisplayEngine;      // +0x0c
    CDefinitionManager_Retail*  DefinitionManager;  // +0x10
    bool IsObjectInThingsPossession(const CCharString& objectDefName, const CScriptThing& thing) const;
};

bool CGameScriptInterface::IsObjectInThingsPossession(const CCharString& objectDefName, const CScriptThing& scriptThing) const
{
    long index = DefinitionManager->GetDefGlobalIndexFromName(objectDefName);
    if (index > 0) {
        const CThing_Retail* thing = scriptThing.GetThing();
        if (thing) {
            if ((thing->HasInterface(0x11) && ((CTCInventoryBase*)thing->GetInterface(0x11))->GetNumberOfItemsOfTypeInInventory(index) > 0)
                || (thing->HasInterface(0x46) && ((CTCCarrying*)thing->GetInterface(0x46))->IsCarryingThing(index))
                || (thing->HasInterface(0x13) && ((CTCInventoryBase*)thing->GetInterface(0x13))->GetNumberOfItemsOfTypeInInventory(index) > 0)
                || (thing->HasInterface(0x12) && ((CTCInventoryBase*)thing->GetInterface(0x12))->GetNumberOfItemsOfTypeInInventory(index) > 0)
                || (thing->HasInterface(0x3c) && ((CTCHouseContents*)thing->GetInterface(0x3c))->ContainsItemOfType(index)))
                return true;
        }
    }
    return false;
}