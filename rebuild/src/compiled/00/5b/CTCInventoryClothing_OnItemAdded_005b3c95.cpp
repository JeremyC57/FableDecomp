// CTCInventoryClothing::OnItemAdded(long item, bool assignQuickAccess, bool silent) -- after the base hook, when the added def is an OBJECT whose appearance-modifier sub-def is a clothing modifier (type 2), gives the owner's hero the clothing tutorial (unless silent)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005B3C95, length 229 bytes. Frame-pointer size TU: optimize("y",off) + ("s",on).
// Callees: CTCInventoryBase::OnItemAdded(long, bool, bool) @ 0x005BE028, CTCBase::GetDefinitionManager() const @ 0x006869D0,
// CDefinitionManager::GetDefClassTypeNameFromGlobalIndex(long) @ 0x009ACCE0 (CDefString by value), CDefString::operator==(const char*)
// @ 0x005B3440 on "OBJECT" @ 0x01235FF4, CDefinitionManager::GetDef<CObjectDef>(long, CDefPointer&) @ 0x0042B040,
// CDefClassBase::GetSubDef<CAppearanceModifierDef>(CDefPointer&) @ 0x0057FF92, std::bitset::test @ 0x00410DE0, CVectorMap::LowerBound
// @ 0x0040F020 (the inline GetInterface, type 0x29 = CTCHero), CTCHero::GiveTutorialForCategory(ETutorialCategory, bool) @ 0x006C9FA0
// (category 0x29), the defs' virtual slot 1 on release.
// Shape: both def out-slots are FUNCTION-scope locals zeroed by hand at entry (`and [..],0`, the size-TU immediate form; an
// inline destructor's null tests would pull a zero register into every compare): the object def is homed in the dead
// `assignQuickAccess` slot ([ebp+0Ch]), the sub-def in [ebp-4]. The releases are hand-written inside their blocks, as in the
// landed CTCInventoryTrade/Weapons::OnCreate twins (`dec [p+4] ; jne ; call [vt+4]`), the sub-def held in ebx as a named local. The CDefString temporary (one word, no destructor) lives in [ebp-8]. The interface type and the tutorial category are the
// same immediate (0x29), so one register (edi) serves both. The sub-def is held in ebx across the tutorial call.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CDefString {                                  // one table position, no destructor
    long TablePos;
    bool operator==(const char* text) const;         // 0x005B3440
};
struct CDefPointeeBase {
    virtual void v00();
    virtual void Destroy();                          // slot 1
    long m_refCount;                                 // +0x04
};
template <class T> struct CDefPointer {                // the out-parameter slot of the def lookups (released by hand, as the landed OnCreate twins)
    T* Object;
    T* operator->() const { return Object; }
};
enum EAppearanceModifierType { APPEARANCE_MODIFIER_CLOTHING = 2 };
struct CAppearanceModifierDef : CDefPointeeBase {
    unsigned char _pad08[0x50 - 0x08];
    long Type;                                       // +0x50
};
struct CObjectDef : CDefPointeeBase {                // CDefClassBase::GetSubDef<T>
    bool GetSubDef(CDefPointer<CAppearanceModifierDef>* out);   // 0x0057FF92
};
void* FindDef_CObjectDef_GetSubDef_1();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CObjectDef::GetSubDef(CDefPointer<CAppearanceModifierDef>* out)
{
    void* def = FindDef_CObjectDef_GetSubDef_1();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
struct CDefinitionManager {
    CDefString GetDefClassTypeNameFromGlobalIndex(long globalIndex);     // 0x009ACCE0
    bool GetDef(long globalIndex, CDefPointer<CObjectDef>& out);          // 0x0042B040
};
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long globalIndex, CDefPointer<CObjectDef>& out)
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)&out = def;
    return true;
}
#pragma auto_inline(on)
struct CPresenceBits {
    bool test(long bit);                             // 0x00410DE0 (std::bitset::test)
};
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;                                   // +0x00
    SEntry* last;                                    // +0x04
    SEntry* LowerBound(const long* type);            // 0x0040F020
};
struct CThing {
    unsigned char _pad_0x00[0x20];
    CPresenceBits m_present;                         // +0x20
    unsigned char _pad_0x21[0x23];
    CInterfaceMap m_interfaces;                      // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            CInterfaceMap::SEntry* found = (it == map->last || wanted < it->type) ? map->last : it;
            *out = (T*)found->component;
            return true;
        }
        return false;
    }
};
enum ETutorialCategory { TUTORIAL_CATEGORY_CLOTHING = 0x29 };
struct CTCHero {                                     // interface type 0x29
    bool GiveTutorialForCategory(ETutorialCategory category, bool force);   // 0x006C9FA0
};
struct CTCInventoryBase_Retail {
    void*   __vftable;                               // +0x00
    CThing* ParentThing;                             // +0x04
    CDefinitionManager* GetDefinitionManager() const;              // 0x006869D0 (CTCBase)
    void OnItemAdded(long item, bool assignQuickAccess, bool silent);   // 0x005BE028
};
struct CTCInventoryClothing : CTCInventoryBase_Retail {
    void OnItemAdded(long item, bool assignQuickAccess, bool silent);
};
void CTCInventoryClothing::OnItemAdded(long item, bool assignQuickAccess, bool silent)
{
    CTCInventoryBase_Retail::OnItemAdded(item, assignQuickAccess, silent);
    CDefPointer<CObjectDef> objectDef;
    objectDef.Object = 0;
    CDefPointer<CAppearanceModifierDef> modifierDef;
    modifierDef.Object = 0;
    if (GetDefinitionManager()->GetDefClassTypeNameFromGlobalIndex(item) == "OBJECT") {
        if (GetDefinitionManager()->GetDef(item, objectDef)) {
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