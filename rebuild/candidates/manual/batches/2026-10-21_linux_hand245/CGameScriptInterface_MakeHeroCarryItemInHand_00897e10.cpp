// CGameScriptInterface::MakeHeroCarryItemInHand(const CCharString& itemDefName) const -- an empty name empties the hero's primary carry slot; otherwise find an item of the named def in the hero's inventory (TC 0x11) or second store (TC 0x13) and hand it to the script-thing overload
// Leaf: ?MakeHeroCarryItemInHand@CGameScriptInterface@@UBEXABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00897E10, length 383 bytes (`ret 4`).
// Callees: CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetHeroThing @ 0x00487DC0, CBasicString<char>::operator==
//   @ 0x004115A0 (inline CCharString::operator==(const char*)), CDefinitionManager::GetDefGlobalIndexFromName @ 0x009AD410,
//   the component map LowerBound @ 0x0040F020, CTCCarrying::RemoveThingFromPrimarySlot(bool) @ 0x006A1E00, the store
//   component's virtual slot 47 (+0xBC, find an item of a def), CThing::GetScriptThing @ 0x004C7E50, ~CScriptThing
//   @ 0x004AA840, own virtual slot 142 = the MakeHeroCarryItemInHand(const CScriptThing&, bool, bool) overload (0x00897FA0).
// Shape: GetInterface returns `((it == last || key < it->type) ? last : it)->component` (the ternary form loads the store
//   pointer straight into ecx; an `it = last` reassignment joined it in eax); the store components use the GetTCIfPresent form (left unset when absent: retail reads the dead lookup-key
//   word); the key is homed in the dead itemDefName argument slot; `this` is saved in a frame word.
// Layout: CThing UsingComponents +0x20, component map +0x44, dead flags +0x91 bit 0 (landed CloseDoor 0x00895DE0 model).
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
    unsigned char         _pad4c[0x45];
    unsigned char         m_dead : 1;               // +0x91 bit 0
    unsigned char         m_91_1 : 7;
    unsigned long HasInterface(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    __forceinline CTCBase_Retail* GetInterface(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap_Retail* map = &m_interfaces;
        CInterfaceMap_Retail::SEntry* it = map->LowerBound(&type);
        return ((it == map->last || interfaceType < it->type) ? map->last : it)->component;
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
    CScriptThing GetScriptThing();                  // 0x004C7E50
};
struct CScriptThing {
    void* _vptr;
    void* PImp_Data;
    void* PImp_Info;
    ~CScriptThing();                                // 0x004AA840
};
struct CTCInventoryStore {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46();
    virtual CThing_Retail* FindItemOfType(long defIndex);   // slot 47 (+0xBC)
};
__forceinline CThing_Retail* FindItemInStore(CThing_Retail* thing, long interfaceType, long defIndex)
{
    CTCInventoryStore* store;                       // deliberately unset when absent (retail reads a dead slot)
    if (thing->HasInterface(interfaceType))
        store = (CTCInventoryStore*)thing->GetInterface(interfaceType);
    return store->FindItemOfType(defIndex);
}
struct CTCCarrying { CThing_Retail* RemoveThingFromPrimarySlot(bool b); };   // 0x006A1E00
struct CPlayer { CThing_Retail* GetHeroThing(); };      // 0x00487DC0
struct CPlayerManager { CPlayer* GetMainPlayer(); };    // 0x00449970
struct CDefinitionManager_Retail { long GetDefGlobalIndexFromName(const CCharString& name); };   // 0x009AD410

#define VPAD1(a) virtual void a();
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
struct CGameScriptInterface {
    VPAD128(pa) VPAD8(pb) VPAD4(pc) VPAD2(pd)           // slots 0..141
    // slot 142: the PDB overload MakeHeroCarryItemInHand(const CScriptThing&, bool, bool) (0x00897FA0)
    virtual void MakeHeroCarryScriptThingInHand(const CScriptThing& item, bool a, bool b) const;
    virtual void MakeHeroCarryItemInHand(const CCharString& itemDefName) const;
    void*                       World;              // +0x04 (engine/CGameScriptInterface.h)
    void*                       Component;          // +0x08
    void*                       DisplayEngine;      // +0x0c
    CDefinitionManager_Retail*  DefinitionManager;  // +0x10
    CPlayerManager*             PlayerManager;      // +0x14
};

void CGameScriptInterface::MakeHeroCarryItemInHand(const CCharString& itemDefName) const
{
    CThing_Retail* hero = PlayerManager->GetMainPlayer()->GetHeroThing();
    if (!hero || hero->m_dead)
        return;
    // the inline CCharString::operator==("") with its two returns written as two tests that jump to the empty arm
    // (the landed IsPlayerCarryingItemOfType 0x008973A0 form; the returning operator materialises `sete al`)
    if (itemDefName.PStringData == 0) {
        if (strcmp("", "") == 0)
            goto empty;
    } else if (itemDefName.PStringData->Data == "")
        goto empty;
    {
    long index = DefinitionManager->GetDefGlobalIndexFromName(itemDefName);
    if (index <= 0)
        return;
    CTCInventoryStore* inventory;                   // deliberately unset when absent (retail reads a dead slot)
    if (hero->HasInterface(0x11))
        inventory = (CTCInventoryStore*)hero->GetInterface(0x11);
    CThing_Retail* item = inventory->FindItemOfType(index);
    if (!item || item->m_dead) {
        CTCInventoryStore* store;
        if (hero->HasInterface(0x13))
            store = (CTCInventoryStore*)hero->GetInterface(0x13);
        item = store->FindItemOfType(index);
    }
    if (item && !item->m_dead)
        MakeHeroCarryScriptThingInHand(item->GetScriptThing(), true, false);
    return;
    }
empty:
    CTCCarrying* carrying;
    if (hero->GetInterfaceOut(&carrying, 0x46))
        carrying->RemoveThingFromPrimarySlot(false);
}
