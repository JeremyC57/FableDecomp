// CTCInventoryAbilities::UnAssignAbilityPC(unsigned long slot, NUISystem::CComponent* container) -- empties a PC spell quick-access slot: when the ability is no longer assigned anywhere its menu entry gets a fresh unassigned icon; the old unassigned icon's action parameter is reset, the container's assignment icon is hidden and a fresh empty spell component goes into the container
// Grade: DIFFER(530v518) -- the lane keeps the twice-used "PC_UNASSIGNED_SPELL_ICON" literal address in ebx (retail pushes it twice), which spills the loop index; a distinct literal object ("...\\0") gets 521v518 with only frame-slot choices left (ability homed in the dead slot word, retail [ebp-4])
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F1320, 518 bytes, `ret 8`; frame-pointer size TU
// (`optimize("y", off)` + `("s", on)`, as the landed CTCInventory::UnAssignThingPC 0x00584F15 this mirrors).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CFrontEndManager::GetInstance 0x0041E5F2,
// CManager::CreateComponent 0x0041DB1D, CCountedPointer<CComponent>(CComponent*) 0x00429C15 / dtor 0x004291DE,
// map<EHeroAbility, CComponent*>::operator[] 0x005F9EA5, ::find 0x005F8444 and ::erase(iterator) 0x0057926B, the
// {unsigned long, list<CComponent*>} action-parameter pair ctor 0x0042BED4 / copy ctor 0x0042CD84 / list dtor 0x0042ABCA;
// component slots 3 (FindChild), 13/14 (SetColour/GetColour), 60 (+0xF0) and 62 (+0xF8).
// Layout: retail CTCInventoryAbilities (Ego_r.pdb with 12-byte containers): AssignedAbilities +0x17C (PDB +0x1A4),
// AssignedAbilitiesSave +0x188, UnassignedIcons +0x1A0, AbilitiesMenuEntries +0x1AC.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1); // 0x0099EBF0
    CCharString(const CCharString& other);           // (declared: by-value arguments are built in place)
    ~CCharString();                                  // 0x0099EAE0
};

struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour() {}
};

namespace NUISystem { struct CComponent; }
struct SComponentListNode;
struct CComponentList {                              // STLport list<NUISystem::CComponent*>: the node pointer only
    SComponentListNode* m_pNode;
    ~CComponentList();                               // 0x0042ABCA
};
struct SActionParamPair {                            // pair<unsigned long, list<NUISystem::CComponent*> >
    unsigned long  first;                            // +0x00
    CComponentList second;                           // +0x04
    SActionParamPair();                              // 0x0042BED4
    SActionParamPair(const SActionParamPair& other); // 0x0042CD84
};

template<class T> struct CCountedPointer {
    T*    m_pObject;
    void* m_pCount;
    CCountedPointer(T* object);                      // 0x00429C15
    ~CCountedPointer();                              // 0x004291DE
};

namespace NUISystem {
struct CComponent {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual CComponent* FindChild(CCharString name); // slot 3 (+0x0C)
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08();
    virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual void SetColour(const CRGBColour& colour);   // slot 13 (+0x34)
    virtual CRGBColour GetColour() const;            // slot 14 (+0x38)
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void SetContents_slot60(const CCountedPointer<CComponent>& child);   // slot 60 (+0xF0)
    virtual void v61();
    virtual void SetActionParameter_slot62(SActionParamPair parameter);   // slot 62 (+0xF8)
};
struct CManager {
    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D
};
struct CFrontEndManager : CManager {
    static CFrontEndManager* __fastcall GetInstance();   // 0x0041E5F2
};
}

struct CLongVector {                                 // retail std::vector<long> {first, last, end}
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    long& operator[](unsigned long n) { return _M_start[n]; }
};
struct SComponentMapNode {                           // map<EHeroAbility, CComponent*> node
    void*                  _M_links[4];
    long                   first;                    // +0x10
    NUISystem::CComponent* second;                   // +0x14
};
struct CComponentMapIterator {
    SComponentMapNode* _M_node;
    CComponentMapIterator(SComponentMapNode* node) : _M_node(node) { }
    CComponentMapIterator(const CComponentMapIterator& o) : _M_node(o._M_node) { }
    bool operator!=(const CComponentMapIterator& o) const { return _M_node != o._M_node; }
    SComponentMapNode* operator->() const { return _M_node; }
};
struct CComponentMap {                               // retail 12-byte STLport map<EHeroAbility, NUISystem::CComponent*>
    SComponentMapNode* _M_header;
    unsigned long      _M_node_count;
    unsigned long      _M_compare;
    NUISystem::CComponent*& operator[](const long& key);   // 0x005F9EA5
    SComponentMapNode* _M_find(const long& key);     // 0x005F8444
    CComponentMapIterator find(const long& key) { return CComponentMapIterator(_M_find(key)); }
    CComponentMapIterator end() { return CComponentMapIterator(_M_header); }
    void erase(CComponentMapIterator it);            // 0x0057926B
};

struct CTCInventoryAbilities {
    void*         __vftable;                         // +0x000
    unsigned char _pad_0x004[0x178];
    CLongVector   AssignedAbilities;                 // +0x17C (PDB +0x1A4)
    CLongVector   AssignedAbilitiesSave;             // +0x188
    unsigned char _pad_0x194[0x0C];
    CComponentMap UnassignedIcons;                   // +0x1A0
    CComponentMap AbilitiesMenuEntries;              // +0x1AC
    void UnAssignAbilityPC(unsigned long slot, NUISystem::CComponent* container);
};

void CTCInventoryAbilities::UnAssignAbilityPC(unsigned long slot, NUISystem::CComponent* container)
{
    long ability = AssignedAbilities[slot];
    AssignedAbilities[slot] = 0;
    AssignedAbilitiesSave[slot] = 0;
    bool stillAssigned = false;
    for (unsigned long i = 0; i < AssignedAbilities.size(); ++i) {
        if (AssignedAbilities[i] == ability) {
            stillAssigned = true;
            break;
        }
    }
    if (!stillAssigned && ability) {
        CCountedPointer<NUISystem::CComponent> icon(NUISystem::CFrontEndManager::GetInstance()->CreateComponent("PC_UNASSIGNED_SPELL_ICON", false));
        AbilitiesMenuEntries[ability]->SetContents_slot60(icon);
        UnassignedIcons[ability] = icon.m_pObject;
    }
    if (AssignedAbilities[slot]) {
        CComponentMapIterator it = UnassignedIcons.find(AssignedAbilities[slot]);
        if (it != UnassignedIcons.end()) {
            NUISystem::CComponent* icon = it->second->FindChild(CCharString("PC_UNASSIGNED_SPELL_ICON", -1));
            if (icon) {
                SActionParamPair parameter;
                parameter.first = 1;
                icon->SetActionParameter_slot62(parameter);
            }
            UnassignedIcons.erase(it);
        }
    }
    NUISystem::CComponent* assignment = container->FindChild(CCharString("PC_SPELL_ASSIGNMENT_ICON_INSIDE_CONTAINER", -1));
    if (assignment) {
        SActionParamPair parameter;
        parameter.first = 1;
        assignment->SetActionParameter_slot62(parameter);
        CRGBColour colour = assignment->GetColour();
        colour.A = 0;
        assignment->SetColour(colour);
    }
    NUISystem::CFrontEndManager* manager = NUISystem::CFrontEndManager::GetInstance();
    CCountedPointer<NUISystem::CComponent> item(manager->CreateComponent("PC_SPELL_INSIDE_CONTAINER", false));
    container->SetContents_slot60(item);
}
