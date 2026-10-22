// Grade: DIFFER(520v525) -- retail spills this to a frame word (push ecx) and keeps &itemDefName in ebp; VC7.1 here keeps this in ebp
// CGameScriptInterface::GiveThingItemInSlot(const CScriptThing& thing, const CCharString& itemDefName, const CCharString& slotName) const -- with an item def name: create that item at the thing, clear the named carry slot (the primary slot through RemoveThingFromPrimarySlot, any other by removing its thing and putting an inventory item that is no longer carried into limbo) and put the new item there; with an empty name: take the thing out of the named slot and kill it
// Leaf: GiveThingItemInSlot
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00895190, length 0x20D bytes (+ int3 padding), `ret 0xc`.
// Callees: CBasicString<char>::operator==(const char*) 0x004115A0 (inline CCharString == ""), CDefinitionManager::
//   GetDefGlobalIndexFromName 0x009AD410, the component-map LowerBound 0x0040F020, CCharString(const char*, long) 0x0099EBF0 /
//   dtor 0x0099EAE0, CThing::GetPosition 0x004C73D0, CreateThingObject 0x00703210 (__fastcall), CTCCarrying::IsSlotEmpty
//   0x006A1230 (manifest "InventoryFindItemByIDLowerBound") / IsSlotPrimarySlot 0x006A1330 / RemoveThingFromPrimarySlot 0x006A1E00 /
//   GetThingInCarrySlot 0x006A11F0 / RemoveThing 0x006A19B0 / IsCarryingThing 0x006A1060 / RemoveThingInCarrySlot 0x006A16A0 /
//   PutThingInCarrySlot 0x006A1F80, CThing::SetInLimbo 0x004C8CF0 / Kill 0x004C9B80.
// Layout: the landed CGameScriptInterface::MakeHeroCarryItemInHand(const CCharString&) 0x00897E10 model (CCharString data block,
//   component mask +0x20, map +0x44, player +0x90, dead bit +0x91); the inventory item's owning inventory at +0x10.
// Shape: the inline `== ""` as two tests jumping to the empty arm (landed 0x00897E10 form); the component lookups use the
//   `it = last` reassignment (eax join); the "" script name converts to a CCharString temporary before the pushes.
struct C3DVector { float X, Y, Z; };
#include <string.h>
#pragma pack(push, 1)
struct CBasicStringChar {                   // CBasicString<char>, 13 bytes
    char*         PData;                    // +0x00
    unsigned long StringLength;             // +0x04
    unsigned long DataLength : 31;          // +0x08
    bool          UseFastExtend : 1;        // +0x0c
    bool operator==(const char* text) const;    // 0x004115A0
};
struct CCharStringData {                    // 17 bytes
    CBasicStringChar Data;                  // +0x00
    long             NoRefs;                // +0x0d
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;           // +0x00
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0)
            return strcmp(text, "") == 0;
        return PStringData->Data == text;
    }
};
struct CTCBase_Retail;
struct CInterfaceMap_Retail {
    struct SEntry { long type; CTCBase_Retail* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);   // 0x0040F020
};
struct CScriptThing;
struct CThing_Retail {
    unsigned char         _pad00[0x20];
    unsigned long         m_presentBits[9];         // +0x20
    CInterfaceMap_Retail  m_interfaces;             // +0x44
    unsigned char         _pad4c[0x44];
    signed char           PlayerNumber;             // +0x90
    unsigned char         m_dead : 1;               // +0x91 bit 0
    unsigned char         m_91_1 : 7;
    unsigned long HasInterface(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    __forceinline CTCBase_Retail* GetInterface(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap_Retail* map = &m_interfaces;
        CInterfaceMap_Retail::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
    __forceinline CTCBase_Retail* GetTCIfPresent(long interfaceType)
    {
        CTCBase_Retail* tc;                         // deliberately unset when absent (retail reads a dead slot)
        if (HasInterface(interfaceType))
            tc = GetInterface(interfaceType);
        return tc;
    }
    template<class T> __forceinline bool GetInterfaceOut(T** ppOut, long interfaceType)
    {
        if (HasInterface(interfaceType)) {
            *ppOut = (T*)GetInterface(interfaceType);
            return true;
        }
        return false;
    }
    const C3DVector& GetPosition() const;            // 0x004C73D0
    void SetInLimbo(bool limbo);                    // 0x004C8CF0
    void Kill(bool b);                              // 0x004C9B80
};
struct CScriptThing {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing_Retail* GetThing() const;        // slot 11 (+0x2c)
};
struct CCharStringTemp {
    void* PStringData;
    CCharStringTemp(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharStringTemp();                                    // 0x0099EAE0
};
struct CRightHandedSet;
CThing_Retail* __fastcall CreateThingObject(long defIndex, const C3DVector* position, long player, CRightHandedSet* orientation,
    unsigned long flags, const CCharStringTemp& scriptName);   // 0x00703210
struct CInventoryOwner;
struct CTCInventoryItem {
    unsigned char    _pad00[0x10];
    CInventoryOwner* PInventory;                    // +0x10
};
struct CTCCarrying {
    bool IsSlotEmpty(long slot);                    // 0x006A1230
    bool IsSlotPrimarySlot(long slot);              // 0x006A1330
    CThing_Retail* RemoveThingFromPrimarySlot(bool drop);   // 0x006A1E00
    CThing_Retail* GetThingInCarrySlot(long slot);  // 0x006A11F0
    void RemoveThing(CThing_Retail* thing);         // 0x006A19B0
    bool IsCarryingThing(CThing_Retail* thing);     // 0x006A1060
    CThing_Retail* RemoveThingInCarrySlot(long slot, bool drop);   // 0x006A16A0
    void PutThingInCarrySlot(CThing_Retail* thing, long slot, bool flag);   // 0x006A1F80
};
struct CDefinitionManager_Retail { long GetDefGlobalIndexFromName(const CCharString& name); };   // 0x009AD410
struct CGameScriptInterface {
    virtual void v000();
    void*                       World;              // +0x04 (engine/CGameScriptInterface.h)
    void*                       Component;          // +0x08
    void*                       DisplayEngine;      // +0x0c
    CDefinitionManager_Retail*  DefinitionManager;  // +0x10
    void*                       PlayerManager;      // +0x14
    virtual void GiveThingItemInSlot(const CScriptThing& thing, const CCharString& itemDefName, const CCharString& slotName) const;
};

void CGameScriptInterface::GiveThingItemInSlot(const CScriptThing& thing, const CCharString& itemDefName, const CCharString& slotName) const
{
    CThing_Retail* object = thing.GetThing();
    if (!object)
        return;
    if (itemDefName.PStringData == 0) {
        if (strcmp("", "") == 0)
            goto empty;
    } else if (itemDefName.PStringData->Data == "")
        goto empty;
    {
    long defIndex = DefinitionManager->GetDefGlobalIndexFromName(itemDefName);
    if (defIndex <= 0)
        return;
    if (!object->HasInterface(0x46))
        return;
    CTCCarrying* carrying = (CTCCarrying*)object->GetInterface(0x46);
    CThing_Retail* item = CreateThingObject(defIndex, &object->GetPosition(), object->PlayerNumber, 0, 0, "");
    long slot = DefinitionManager->GetDefGlobalIndexFromName(slotName);
    if (slot <= 0)
        return;
    if (!carrying->IsSlotEmpty(slot)) {
        if (carrying->IsSlotPrimarySlot(slot))
            carrying->RemoveThingFromPrimarySlot(false);
        else {
            CThing_Retail* old = carrying->GetThingInCarrySlot(slot);
            carrying->RemoveThing(old);
            if (!carrying->IsCarryingThing(old) && old->HasInterface(0x1a)
                && ((CTCInventoryItem*)old->GetInterface(0x1a))->PInventory)
                old->SetInLimbo(true);
        }
    }
    carrying->PutThingInCarrySlot(item, slot, true);
    return;
    }
empty:
    if (!object->HasInterface(0x46))
        return;
    CTCCarrying* carrying = (CTCCarrying*)object->GetInterface(0x46);
    long slot = DefinitionManager->GetDefGlobalIndexFromName(slotName);
    if (slot <= 0)
        return;
    CThing_Retail* old = carrying->RemoveThingInCarrySlot(slot, false);
    if (old && !old->m_dead)
        old->Kill(false);
}
