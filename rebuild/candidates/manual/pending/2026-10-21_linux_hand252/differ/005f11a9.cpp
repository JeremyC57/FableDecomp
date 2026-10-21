// Grade: DIFFER(358v375) -- tail (find/FindChild/pair/erase) matches save the map's home slot; the head CSEs &AssignedAbilities[slot] and &PreviousAbilities and homes the removed ability in the dead slot word ([ebp+8]) where retail re-reads each begin pointer and homes it at [ebp-4] (string temp in [ebp+8])
// CTCInventoryAbilities::UnAssignAbility(unsigned long slot) -- empty a quick slot (falling back to the slot's previous ability unless that is the one removed); when the removed ability is held in no slot any more, give its container a fresh "unassigned" icon; then reset the icon of the slot's ability and forget it
// Leaf: UnAssignAbility
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F11A9, 375 bytes, `ret 4`. Size TU with a frame pointer:
//   `#pragma optimize("y", off)` + `("s", on)`.
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0 ("UI_UNASSIGNED_SPELL_ICON"
//   @ 0x01256608), NUISystem::CManager::GetInstance @ 0x0041E5F2 / CreateComponent @ 0x0041DB1D,
//   CCountedPointer<CComponent>(CComponent*) @ 0x00429C15 / dtor @ 0x004291DE, map<EHeroAbility, CComponent*>::operator[]
//   @ 0x005F9EA5 / find @ 0x005F8444 / erase(iterator) @ 0x0057926B, the component's virtual slots 3 FindChild,
//   60 (+0xF0, takes the counted pointer) and 62 (+0xF8, takes the action-parameter pair by value), the pair's ctor
//   @ 0x0042BED4 / copy ctor @ 0x0042CD84, list<CComponent*> dtor @ 0x0042ABCA.
// Layout: retail CTCInventoryAbilities (12-byte containers): AssignedAbilities vector<EHeroAbility> +0x17C (as the
//   landed 0x005F4519), the previous-ability vector +0x188, the ability-icon map +0x1A0 and the ability-container map +0x1AC (12-byte STLport maps,
//   header node pointer first).
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1); // 0x0099EBF0
    CCharString(const CCharString& other);           // (declared: by-value arguments are built in place)
    ~CCharString();                                  // 0x0099EAE0
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
    virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33();
    virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38();
    virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43();
    virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48();
    virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58();
    virtual void v59();
    virtual void SetContents_slot60(const CCountedPointer<CComponent>& child);   // slot 60 (+0xF0)
    virtual void v61();
    virtual void SetActionParameter_slot62(SActionParamPair parameter);   // slot 62 (+0xF8)
};
struct CManager {
    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D
    static CManager* __fastcall GetInstance();       // 0x0041E5F2
};
}
using NUISystem::CComponent;
using NUISystem::CManager;

enum EHeroAbility { HERO_ABILITY_NULL = 0 };

struct SAbilityNode {                                // _Rb_tree_node<pair<const EHeroAbility, CComponent*> >
    long color;
    SAbilityNode* parent;
    SAbilityNode* left;
    SAbilityNode* right;
    EHeroAbility first;                              // +0x10
    CComponent* second;                              // +0x14
};
struct CAbilityIterator {
    SAbilityNode* _M_node;
    CAbilityIterator(SAbilityNode* node) : _M_node(node) { }
    CAbilityIterator(const CAbilityIterator& other) : _M_node(other._M_node) { }
};
struct CAbilityMap {                                 // map<EHeroAbility, CComponent*> (retail 12 bytes)
    SAbilityNode* _M_header;
    unsigned long _M_node_count;
    unsigned long _M_compare;
    CComponent*& operator[](const EHeroAbility& key);   // 0x005F9EA5
    SAbilityNode* find(const EHeroAbility& key);     // 0x005F8444 (returns the node)
    void erase(CAbilityIterator position);           // 0x0057926B
};

template <class T> struct CRetailVector {            // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long n) const { return *(begin() + n); }
};

struct CTCInventoryAbilities {
    void*         __vftable;                         // +0x00
    void*         ParentThing;                       // +0x04 CTCBase
    unsigned char _pad_0x08[0x174];
    CRetailVector<EHeroAbility> AssignedAbilities;   // +0x17C
    CRetailVector<EHeroAbility> PreviousAbilities;   // +0x188
    unsigned char _pad_0x194[0xc];
    CAbilityMap   AbilityIcons;                      // +0x1A0
    CAbilityMap   AbilityContainers;                 // +0x1AC

    __forceinline bool IsAbilityAssigned(EHeroAbility ability) const
    {
        for (unsigned long i = 0; i < AssignedAbilities.size(); ++i)
            if (AssignedAbilities[i] == ability)
                return true;
        return false;
    }
    void UnAssignAbility(unsigned long slot);
};

void CTCInventoryAbilities::UnAssignAbility(unsigned long slot)
{
    EHeroAbility removed = AssignedAbilities[slot];
    EHeroAbility previous = PreviousAbilities[slot];
    if (removed != previous) {
        AssignedAbilities[slot] = previous;
    } else {
        AssignedAbilities[slot] = HERO_ABILITY_NULL;
        PreviousAbilities[slot] = HERO_ABILITY_NULL;
    }
    if (!IsAbilityAssigned(removed) && removed != 0) {
        CCountedPointer<CComponent> icon(CManager::GetInstance()->CreateComponent(CCharString("UI_UNASSIGNED_SPELL_ICON", -1), false));
        AbilityContainers[removed]->SetContents_slot60(icon);
        CComponent* newIcon = icon.m_pObject;
        AbilityIcons[removed] = newIcon;
    }
    const EHeroAbility& current = AssignedAbilities[slot];
    if (current != HERO_ABILITY_NULL) {
        SAbilityNode* found = AbilityIcons.find(current);
        if (found != AbilityIcons._M_header) {
            CComponent* oldIcon = found->second;
            CComponent* child = oldIcon->FindChild(CCharString("UI_UNASSIGNED_SPELL_ICON", -1));
            if (child) {
                SActionParamPair parameter;
                parameter.first = 1;
                child->SetActionParameter_slot62(parameter);
            }
            AbilityIcons.erase(found);
        }
    }
}
