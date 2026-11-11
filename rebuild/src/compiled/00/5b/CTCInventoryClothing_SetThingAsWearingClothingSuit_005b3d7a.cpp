// CTCInventoryClothing::SetThingAsWearingClothingSuit(CThing* thing, long suitDefIndex) -- strips the thing's clothing appearance modifiers and dresses it in every part of the hero suit `suitDefIndex` that the inventory actually holds, then refreshes the modifiers
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad_0x21[0x23] removed.
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005B3D7A, length 212 bytes (`ret 8`). Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: std::bitset::test @ 0x00410DE0 and CVectorMap::LowerBound @ 0x0040F020 (the inline GetInterface for type 0x5E),
// CTCHeroAttachableAppearanceModifiers::RemoveAllAppearanceModifiers(long) @ 0x00706640, AddAppearanceModifierWithReplace(long)
// @ 0x00706950 and UploadChangesToGraphics() @ 0x00707930, CTCBase::GetDefinitions() @ 0x006869D0, CDefinitionManager::GetDef(long,
// CDefPointer<T>*) @ 0x0042AF3C (the catalog's GetDef<CSpecialAbilitiesDrunkennessDef> body), CDefClassBase::GetSubDef<CHeroSuitDef>
// @ 0x005935C4 (landed), CTCInventoryBase::GetNumberOfItemsOfTypeInInventory(long) @ 0x005BDF08, the STLport tree successor
// _Rb_global<bool>::_M_increment @ 0x004292C0 (`__cdecl`), the defs' virtual slot 1 on release.
// Layout: CThing presence bits +0x20, interface map +0x44; CHeroSuitDef::SuitParts (map<EClothingSuitPart,long>) at +0x28
// (PDB +0x28; node: part +0x10, item def index +0x14).
// Shape: the GetInterface key lives in the dead `thing` slot ([ebp+8]), which then becomes the zeroed home of the suit's def out
// pointer (`and [ebp+8],0`); the sub-def out pointer takes the dead `suitDefIndex` slot ([ebp+0Ch]); the suit def is read into
// a register (edi) and released by hand without a null test, the owning def through the slot with one (the landed OnCreate
// idiom of this module). The map walk re-reads end() every pass; `this` is spilled to [ebp-4] and 0x5E to edi (`push 5Eh ; pop edi`).
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char _pad_0x00[0x20];
    std::bitset<274> m_present;                         // +0x20
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            ETCInterfaceType type = (ETCInterfaceType)(wanted);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            CInterfaceMap::value_type* found = (it == map->end() || wanted < it->first) ? map->end() : it;
            *out = (T*)((void*)found->second);
            return true;
        }
        return false;
    }
};
enum { APPEARANCE_MODIFIER_CLOTHING = 2 };
class CTCHeroAttachableAppearanceModifiers { public:        // interface type 0x5E
    void RemoveAllAppearanceModifiers(long type);    // 0x00706640
    void AddAppearanceModifierWithReplace(long defIndex);   // 0x00706950
    void UploadChangesToGraphics();                  // 0x00707930 (Ego_r.pdb QAEXXZ)
};
struct SNode {                                       // _Rb_tree_node<pair<const EClothingSuitPart, long>>
    long   _M_color;
    SNode* _M_parent;
    SNode* _M_left;
    SNode* _M_right;
    long   first;                                    // +0x10 EClothingSuitPart
    long   second;                                   // +0x14 item def index
};
SNode* __cdecl _M_increment(SNode* node);            // 0x004292C0 (_Rb_global<bool>::_M_increment)
struct CSuitPartMap {                                // std::map<EClothingSuitPart, long> (12 bytes)
    SNode* _M_header;
    unsigned long _M_node_count;
    long _M_key_compare;
    SNode* begin() { return _M_header->_M_left; }
    SNode* end() { return _M_header; }
};
class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                          // slot 1
    long m_refCount;                                 // +0x04
};
template <class T> class CDefPointer { public:              // the out slot the definition lookups fill (counted reference handed over)
    T* Object;
    T* operator->() const { return Object; }
};
class CHeroSuitDef : public CDefPointeeBase { public:
    unsigned char _pad08[0x28 - 0x08];
    CSuitPartMap  SuitParts;                         // +0x28
};
class CDefClassBase : public CDefPointeeBase { public:
    bool GetSubDef(CDefPointer<CHeroSuitDef>* out);  // 0x005935C4
};
void* FindDef_CDefClassBase_GetSubDef_1();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CHeroSuitDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_1();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
class CDefinitionManager { public:
    bool GetDef(long index, CDefPointer<CDefClassBase>* out);   // 0x0042AF3C
};
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<CDefClassBase>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

namespace NInventory {
class CTCInventoryBase { public:
    unsigned char _pad00[0x130];
    CDefinitionManager* GetDefinitions();            // 0x006869D0 (CTCBase)
    long GetNumberOfItemsOfTypeInInventory(long itemType);   // 0x005BDF08
};
struct CTCInventoryClothing : CTCInventoryBase {
    void SetThingAsWearingClothingSuit(CThing* thing, long suitDefIndex);
};
void CTCInventoryClothing::SetThingAsWearingClothingSuit(CThing* thing, long suitDefIndex)
{
    CTCHeroAttachableAppearanceModifiers* modifiers;
    if (thing->GetInterface(&modifiers, 0x5E)) {
        modifiers->RemoveAllAppearanceModifiers(APPEARANCE_MODIFIER_CLOTHING);
        CDefPointer<CDefClassBase> def;
        def.Object = 0;
        GetDefinitions()->GetDef(suitDefIndex, &def);
        CDefPointer<CHeroSuitDef> suit;
        suit.Object = 0;
        def->GetSubDef(&suit);
        CHeroSuitDef* suitDef = suit.Object;
        for (SNode* it = suitDef->SuitParts.begin(); it != suitDef->SuitParts.end(); it = _M_increment(it)) {
            if (GetNumberOfItemsOfTypeInInventory(it->second) > 0)
                modifiers->AddAppearanceModifierWithReplace(it->second);
        }
        modifiers->UploadChangesToGraphics();
        if (--suitDef->m_refCount == 0)
            suitDef->Destroy();
        CDefClassBase* owner = def.Object;
        if (owner && --owner->m_refCount == 0)
            owner->Destroy();
    }
}
}