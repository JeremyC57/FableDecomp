// CTCInventoryClothing::OnItemAdded(long item, bool assignQuickAccess, bool silent) -- after the base hook, when the added def is an OBJECT whose appearance-modifier sub-def is a clothing modifier (type 2), gives the owner's hero the clothing tutorial (unless silent)
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad_0x21[0x23] removed.
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005B3C95, length 229 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: NInventory::CTCInventoryBase::OnItemAdded(long, bool, bool) @ 0x005BE028, CTCBase::GetDefinitionManager() const @ 0x006869D0,
// CDefinitionManager::GetDefClassTypeNameFromGlobalIndex(long) @ 0x009ACCE0 (CDefString by value), CDefString::operator==(const char*)
// @ 0x005B3440 on "OBJECT" @ 0x01235FF4, CDefinitionManager::GetDef<CThingObjectDef>(long, CDefPointer<const ..>*) const @ 0x0042B040,
// CDefClassBase::GetSubDef<CAppearanceModifierDef>(CDefPointer<const ..>*) const @ 0x0057FF92, std::bitset::test @ 0x00410DE0, CVectorMap::LowerBound
// @ 0x0040F020 (the inline GetInterface, type 0x29 = CTCHero), CTCHero::GiveTutorialForCategory(ETutorialCategory, bool) @ 0x006C9FA0
// (category 0x29), the defs' virtual slot 1 on release.
// Shape: both def out-slots are FUNCTION-scope locals zeroed by hand at entry (`and [..],0`, the size-TU immediate form; an
// inline destructor's null tests would pull a zero register into every compare): the object def is homed in the dead
// `assignQuickAccess` slot ([ebp+0Ch]), the sub-def in [ebp-4]. The releases are hand-written inside their blocks, as in the
// landed CTCInventoryTrade/Weapons::OnCreate twins (`dec [p+4] ; jne ; call [vt+4]`), the sub-def held in ebx as a named local. The CDefString temporary (one word, no destructor) lives in [ebp-8]. The interface type and the tutorial category are the
// same immediate (0x29), so one register (edi) serves both. The sub-def is held in ebx across the tutorial call.
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
class CDefString { public:                                  // one table position, no destructor
    long TablePos;
    bool operator==(const char* text) const;         // 0x005B3440
};
class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                          // slot 1
    long m_refCount;                                 // +0x04
};
template <class T> class CDefPointer { public:                // the out-parameter slot of the def lookups (released by hand, as the landed OnCreate twins)
    CDefPointeeBase* Object;                         // the pointee as its non-const base (engine/CDefPointer.h)
    T* operator->() const { return static_cast<T*>(Object); }
};
class CDefClassBase : public CDefPointeeBase { public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x0057FF92 GetSubDef<CAppearanceModifierDef> (Ego_r / FableWin template)
};
enum EAppearanceModifierType { APPEARANCE_MODIFIER_CLOTHING = 2 };
class CAppearanceModifierDef : public CDefClassBase { public:
    unsigned char _pad08[0x50 - 0x08];
    long Type;                                       // +0x50
};
class CThingObjectDef : public CDefClassBase { public: };
void* FindDef_CObjectDef_GetSubDef_1();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const
{
    void* def = FindDef_CObjectDef_GetSubDef_1();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
class CDefinitionManager { public:
    template <class T> bool GetDef(long globalIndex, CDefPointer<const T>* out) const;   // 0x0042B040 GetDef<CThingObjectDef> (FableWin.pdb template)
public:
    CDefString GetDefClassTypeNameFromGlobalIndex(long) const;   // 0x009ACCE0
};
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefinitionManager::GetDef(long globalIndex, CDefPointer<const T>* out) const
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
class CGameDefinitionManager : public CDefinitionManager { public: };   // RTTI CGameDefinitionManager : CDefinitionManager
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
enum ETutorialCategory { TUTORIAL_CATEGORY_CLOTHING = 0x29 };
class CTCHero { public:                                     // interface type 0x29
    bool GiveTutorialForCategory(ETutorialCategory category, bool force);   // 0x006C9FA0
};
class CTCBase { public:                              // the component base (members this row reads)
    virtual ~CTCBase();
    CThing* ParentThing;                             // +0x04
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0 (Ego_r.pdb / FableWin.pdb IBE)
};
namespace NInventory {
class CTCInventoryBase : public CTCBase {
protected:
    virtual void OnItemAdded(long item, bool assignQuickAccess, bool silent);   // 0x005BE028 (Ego_r.pdb MAE; called qualified)
};
}
class CTCInventoryClothing : public NInventory::CTCInventoryBase {
protected:
    virtual void OnItemAdded(long item, bool assignQuickAccess, bool silent);   // Ego_r.pdb ?OnItemAdded@CTCInventoryClothing@@MAEXJ_N0@Z
};
void CTCInventoryClothing::OnItemAdded(long item, bool assignQuickAccess, bool silent)
{
    NInventory::CTCInventoryBase::OnItemAdded(item, assignQuickAccess, silent);
    CDefPointer<const CThingObjectDef> objectDef;
    objectDef.Object = 0;
    CDefPointer<const CAppearanceModifierDef> modifierDef;
    modifierDef.Object = 0;
    if (GetDefinitionManager().GetDefClassTypeNameFromGlobalIndex(item) == "OBJECT") {
        if (GetDefinitionManager().GetDef(item, &objectDef)) {
            if (objectDef->GetSubDef(&modifierDef) && !silent && modifierDef->Type == APPEARANCE_MODIFIER_CLOTHING) {
                CTCHero* hero;
                if (ParentThing->GetInterface(&hero, 0x29))
                    hero->GiveTutorialForCategory(TUTORIAL_CATEGORY_CLOTHING, false);
            }
            if (modifierDef.Object && --modifierDef.Object->m_refCount == 0)
                modifierDef.Object->Destroy();
        }
        if (objectDef.Object && --objectDef.Object->m_refCount == 0)
            objectDef.Object->Destroy();
    }
}