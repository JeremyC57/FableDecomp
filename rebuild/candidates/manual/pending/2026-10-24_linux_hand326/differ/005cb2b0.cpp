// CTCInventoryWeapons::SetThingAsActiveWeapon(long defIndex, NUISystem::CComponent* menuEntry) -- makes a freshly created inventory object of `defIndex` the active melee or ranged weapon (unless it already is), moves the menu's assigned-weapon icon to `menuEntry`, retires the previous weapon (unwielded, its sheathe object taken off the carrier and killed), and re-dresses the carrier
// Grade: DIFFER(1176v1170) -- dead-argument-slot colouring: retail homes the weapon key / previous weapon / weapon-def out slot in [ebp+8] and the bits spill, melee item key and icon strings in [ebp-4] (ours swaps them, the weapon key lands in [ebp-0xc]); retail keeps the carrying component in its frame slot [ebp+0xc] and the def holder in esi, ours promotes carrying to ebx and spills the holder
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005CB2B0, 1168 bytes, `ret 8`. Frame-pointer size TU:
//   `#pragma optimize("y", off)` + `("s", on)` (the weapons module). FableWin.pdb
//   ?SetThingAsActiveWeapon@CTCInventoryWeapons@@QAEXJPAVCComponent@NUISystem@@@Z.
// Callees: NUISystem::CFrontEndManager::GetInstance 0x0041E5F2, IS_THING_ALIVE(const CIntelligentPointer&) 0x004CC340,
//   IS_THING_ALIVE(CThing* const&) 0x0040DCE0, CBaseIntelligentPointer::GetPItem 0x00A01B50, CTCInventoryBase::
//   CreateInventoryObject 0x005BE8F3, CThing::SetInLimbo 0x004C8CF0, std::bitset::test 0x00410DE0, CVectorMap LowerBound
//   0x0040F020 / const 0x004365B0, CTCWeapon::IsMeleeWeapon 0x005DAA00, CIntelligentPointer<CThing>(CThing*) 0x004A96F0 /
//   operator= 0x004A9760 / ~CBaseIntelligentPointer 0x00A01C10, the {unsigned long, list<CComponent*>} action-parameter
//   pair 0x0042BED4 / copy 0x0042CD84 / list dtor 0x0042ABCA, CTCInventoryItem::GetEngineGraphic 0x005D92D0,
//   CCharString 0x0099EBF0 / ~ 0x0099EAE0, CManager::CreateComponent 0x0041DB1D, CCountedPointer<CComponent> 0x00429C15 /
//   ~ 0x004291DE, CDefClassBase::GetSubDef<CWeaponDef> 0x0042B1A6, CTCCarrying::IsCarryingThing(long) 0x006A10A0 /
//   GetCarriedThing(long) 0x006A1120 / RemoveThing 0x006A19B0, CThing::Kill 0x004C9B80,
//   CTCInventoryWeapons::SetupActiveWeaponsOnCarrier 0x005CA68E; component slots 62 (+0xF8, action parameter by value),
//   145 (+0x244, graphic index), 60 (+0xF0, contents) and 157 (+0x274, on the weapon list, name descriptive).
// Shape: the two weapon kinds are two full blocks (VC7.1 cross-jumps their list-notify tails); the previous weapon is one
//   local assigned in each block (homed in the dead defIndex slot); the active pointer takes the new thing through the
//   implicit CIntelligentPointer conversion temporary; the icon pair and the counted icon share one frame temporary;
//   the item lookup ignores the GetInterface result (zero-initialised out pointer), the old weapon's is HasTC + GetTC;
//   the previous weapon is a CBaseObject* (GetPItem's type, bound directly by IS_THING_ALIVE<CBaseObject*>) while the new
//   CThing* binds through a converted temporary (it stays in esi); the weapon def is read once from its out slot into a
//   holder whose dtor releases it (one register for every read and the release, no null test after the dereference).
// Layout: CTCInventoryWeapons retail offsets as the landed 0x005C39F0 (InventoryOpen +0x58, packed pointers +0x134/+0x141,
//   icons/menu entries/lists +0x180..+0x194); CThing PBaseDef +0x70, DefIndex +0x8C; CTCWeapon Wielded +0x24;
//   CWeaponDef SheatheObject +0x38; CTCCarrier (interface 0x29) weapons-sheathed flag +0x0C.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CTCBase;

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

struct CPresenceBits { bool test(long kind) const; };   // std::bitset::test 0x00410DE0
struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};

struct CDefBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    __forceinline CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
    __forceinline ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
struct CWeaponDef : CDefBase {
    unsigned char _pad08[0x38 - 0x08];
    long SheatheObject;                                 // +0x38
};
template <class T> struct SDefSlot {                    // the GetSubDef out slot: filled, never released itself
    T* Object;
};
template <class T> struct CDefHolder {                  // takes over the out slot's reference
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    __forceinline ~CDefHolder() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
struct CThingBaseDef : CDefBase {
    bool GetSubDef(SDefSlot<CWeaponDef>* out);          // 0x0042B1A6 (CDefClassBase::GetSubDef<CWeaponDef>)
};

struct CBaseObject { };
struct CThing : CBaseObject {
    unsigned char _pad00[0x20];
    CPresenceBits m_present;                            // +0x20
    unsigned char _pad21[0x44 - 0x21];
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefPointer<CThingBaseDef> PBaseDef;                // +0x70
    unsigned char _pad74[0x8c - 0x74];
    unsigned short DefIndex;                            // +0x8C
    void SetInLimbo(bool inLimbo);                      // 0x004C8CF0
    void Kill(bool immediately);                        // 0x004C9B80
    bool HasTC(long kind) const { return m_present.test(kind); }
    template <class T> __forceinline bool GetInterface(T** out, long kind)
    {
        if (m_present.test(kind)) {
            long key = kind;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            *out = (T*)((it == map->last || kind < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    template <class T> __forceinline T* GetTC(long kind, T*)
    {
        long key = kind;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || kind < it->type) ? map->last : it)->component;
    }
    template <class T> __forceinline const T* GetTC(long kind, const T*) const
    {
        long key = kind;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (const T*)((it == map->last || kind < it->type) ? map->last : it)->component;
    }
};

#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                               // +0x04
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    ~CBaseIntelligentPointer();                         // 0x00A01C10
};
struct CIntelligentPointer_CThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>
    CIntelligentPointer_CThing(CThing* thing);          // 0x004A96F0
    CIntelligentPointer_CThing& operator=(const CIntelligentPointer_CThing& other);   // 0x004A9760
    CThing* operator->() const { return static_cast<CThing*>(GetPItem()); }
};
#pragma pack(pop)
bool __fastcall IS_THING_ALIVE(const CIntelligentPointer_CThing& thing);   // 0x004CC340
bool __fastcall IS_THING_ALIVE(CBaseObject* const& thing);                // 0x0040DCE0

struct CTCWeapon {                                      // interface 0x48
    unsigned char _pad00[0x24];
    bool Wielded;                                       // +0x24
    bool IsMeleeWeapon() const;                         // 0x005DAA00
};
struct CTCInventoryItem {                               // interface 0x1A
    const long& GetEngineGraphic() const;               // 0x005D92D0
};
struct CTCCarrying {                                    // interface 0x46
    bool IsCarryingThing(long defIndex) const;          // 0x006A10A0
    CThing* GetCarriedThing(long defIndex) const;       // 0x006A1120
    void RemoveThing(CThing* thing);                    // 0x006A19B0
};
struct CTCCarrier {                                     // interface 0x29
    unsigned char _pad00[0xc];
    bool m_weaponsSheathed;                             // +0x0C
};

namespace NUISystem { struct CComponent; }
struct SComponentListNode;
struct CComponentList {                                 // STLport list<NUISystem::CComponent*>: the node pointer only
    SComponentListNode* m_pNode;
    ~CComponentList();                                  // 0x0042ABCA
};
struct SActionParamPair {                               // pair<unsigned long, list<NUISystem::CComponent*> >
    unsigned long  first;                               // +0x00
    CComponentList second;                              // +0x04
    SActionParamPair();                                 // 0x0042BED4
    SActionParamPair(const SActionParamPair& other);    // 0x0042CD84
};
template <class T> struct CCountedPointer {
    T*    m_pObject;
    void* m_pCount;
    CCountedPointer(T* object);                         // 0x00429C15
    ~CCountedPointer();                                 // 0x004291DE
    T* operator->() const { return m_pObject; }
};
namespace NUISystem {
struct CComponent {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void SetContents(const CCountedPointer<CComponent>& child);   // slot 60 (+0xF0)
    virtual void v61();
    virtual void SetActionParameter(SActionParamPair parameter);         // slot 62 (+0xF8)
    virtual void v63(); virtual void v64();
    virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74();
    virtual void v75(); virtual void v76(); virtual void v77(); virtual void v78(); virtual void v79();
    virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83(); virtual void v84();
    virtual void v85(); virtual void v86(); virtual void v87(); virtual void v88(); virtual void v89();
    virtual void v90(); virtual void v91(); virtual void v92(); virtual void v93(); virtual void v94();
    virtual void v95(); virtual void v96(); virtual void v97(); virtual void v98(); virtual void v99();
    virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104();
    virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109();
    virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114();
    virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119();
    virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124();
    virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129();
    virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134();
    virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139();
    virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144();
    virtual void SetGraphicIndex(long graphic);         // slot 145 (+0x244)
    virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149();
    virtual void v150(); virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154();
    virtual void v155(); virtual void v156();
    virtual void SetActiveEntry(CComponent* entry);     // slot 157 (+0x274), name descriptive
};
struct CManager {
    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D
};
struct CFrontEndManager : CManager {
    static CFrontEndManager* __fastcall GetInstance();  // 0x0041E5F2
};
}

struct CTCInventoryWeapons {
    void*                      __vftable;               // +0x000
    CThing*                    ParentThing;             // +0x004 (CTCBase)
    unsigned char              _pad008[0x58 - 0x08];
    bool                       InventoryOpen;           // +0x058 (CTCInventoryBase)
    unsigned char              _pad059[0x134 - 0x59];
    CIntelligentPointer_CThing PActiveMeleeWeapon;      // +0x134
    long                       ActiveMeleeWeaponDefIndex;   // +0x13C
    bool                       MeleeWeaponCarriedOnLoad;    // +0x140
    CIntelligentPointer_CThing PActiveRangedWeapon;     // +0x141
    long                       ActiveRangedWeaponDefIndex;  // +0x14C
    bool                       RangedWeaponCarriedOnLoad;   // +0x150
    unsigned char              _pad151[0x180 - 0x151];
    NUISystem::CComponent*     PAssignedRangedIcon;     // +0x180
    NUISystem::CComponent*     PAssignedMeleeIcon;      // +0x184
    NUISystem::CComponent*     PAssignedRangedMenuEntry;    // +0x188
    NUISystem::CComponent*     PAssignedMeleeMenuEntry;     // +0x18C
    NUISystem::CComponent*     PRangedList;             // +0x190
    NUISystem::CComponent*     PMeleeList;              // +0x194
    CThing* CreateInventoryObject(long defIndex);       // 0x005BE8F3 (CTCInventoryBase)
    void SetupActiveWeaponsOnCarrier();                 // 0x005CA68E
    void SetThingAsActiveWeapon(long defIndex, NUISystem::CComponent* menuEntry);
};

void CTCInventoryWeapons::SetThingAsActiveWeapon(long defIndex, NUISystem::CComponent* menuEntry)
{
    NUISystem::CFrontEndManager* manager = NUISystem::CFrontEndManager::GetInstance();
    if (IS_THING_ALIVE(PActiveMeleeWeapon) && PActiveMeleeWeapon->DefIndex == defIndex)
        return;
    if (IS_THING_ALIVE(PActiveRangedWeapon) && PActiveRangedWeapon->DefIndex == defIndex)
        return;
    CThing* thing = CreateInventoryObject(defIndex);
    if (!IS_THING_ALIVE(thing))
        return;
    thing->SetInLimbo(true);
    CTCWeapon* weapon;
    if (!thing->GetInterface(&weapon, 0x48))
        return;
    CBaseObject* previous;
    if (weapon->IsMeleeWeapon()) {
        previous = PActiveMeleeWeapon.GetPItem();
        PActiveMeleeWeapon = thing;
        if (menuEntry) {
            PAssignedMeleeMenuEntry = menuEntry;
            if (PAssignedMeleeIcon) {
                SActionParamPair parameter;
                parameter.first = 1;
                PAssignedMeleeIcon->SetActionParameter(parameter);
            }
            CTCInventoryItem* item = 0;
            thing->GetInterface(&item, 0x1a);
            if (item->GetEngineGraphic() != 0) {
                CCountedPointer<NUISystem::CComponent> icon(manager->CreateComponent("UI_ASSIGNED_WEAPON_ICON", false));
                icon->SetGraphicIndex(item->GetEngineGraphic());
                menuEntry->SetContents(icon);
                PAssignedMeleeIcon = icon.m_pObject;
            }
            if (PMeleeList)
                PMeleeList->SetActiveEntry(menuEntry);
        }
    } else {
        previous = PActiveRangedWeapon.GetPItem();
        PActiveRangedWeapon = thing;
        if (menuEntry) {
            PAssignedRangedMenuEntry = menuEntry;
            if (PAssignedRangedIcon) {
                SActionParamPair parameter;
                parameter.first = 1;
                PAssignedRangedIcon->SetActionParameter(parameter);
            }
            CTCInventoryItem* item = 0;
            thing->GetInterface(&item, 0x1a);
            if (item->GetEngineGraphic() != 0) {
                CCountedPointer<NUISystem::CComponent> icon(manager->CreateComponent("UI_ASSIGNED_WEAPON_ICON", false));
                icon->SetGraphicIndex(item->GetEngineGraphic());
                menuEntry->SetContents(icon);
                PAssignedRangedIcon = icon.m_pObject;
            }
            if (PRangedList)
                PRangedList->SetActiveEntry(menuEntry);
        }
    }
    if (IS_THING_ALIVE(previous)) {
        CThing* old = static_cast<CThing*>(previous);
        CTCWeapon* oldWeapon = 0;
        if (old->HasTC(0x48))
            oldWeapon = old->GetTC(0x48, (CTCWeapon*)0);
        SDefSlot<CWeaponDef> weaponDefSlot;
        weaponDefSlot.Object = 0;
        oldWeapon->Wielded = false;
        {
            CDefPointer<CThingBaseDef> baseDef = old->PBaseDef;
            baseDef->GetSubDef(&weaponDefSlot);
        }
        CTCCarrying* carrying = 0;
        ParentThing->GetInterface(&carrying, 0x46);
        CDefHolder<CWeaponDef> weaponDef(weaponDefSlot.Object);
        if (weaponDef->SheatheObject > 0 && carrying->IsCarryingThing(weaponDef->SheatheObject)) {
            CThing* sheathe = carrying->GetCarriedThing(weaponDef->SheatheObject);
            carrying->RemoveThing(sheathe);
            sheathe->Kill(true);
        }
    }
    weapon->Wielded = true;
    if (!InventoryOpen) {
        const CThing* owner = ParentThing;
        if (!owner->HasTC(0x29) || owner->GetTC(0x29, (const CTCCarrier*)0)->m_weaponsSheathed)
            SetupActiveWeaponsOnCarrier();
    }
}
