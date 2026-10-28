// CTCInventoryAbilities::AssignAbilityPC(long, unsigned long, NUISystem::CComponent*) -- puts a spell into a quick-access slot: refuses a duplicate in the paired slot, returns the displaced spell's unassigned icon to its menu entry, removes the new spell's unassigned icon, and fills the container with a fresh assignment icon carrying the spell graphic and its drop and cast actions
// Grade: DIFFER(996v996) -- residue: (1) old-contents name ternary is a register select (retail pushes the two literals in the arms after push -1, with the reused literal CSE'd in edi), (2) the first use-param new-temp is homed in the dead container arg slot [ebp+0x10] where retail packs it into the dead ability slot [ebp+8]; lane gap: frame-slot packing for (2) | tried: eq/ne ternaries, implicit-conversion ternaries, two-temp ternaries, if/else of whole FindChild calls (26-31), named single/two param vars (107-134), scoped temps (inert), push_back by value (26) | next: a source form that reaches push-in-arms without moving the manager slot
#pragma optimize("y", off)
#pragma optimize("s", on)

enum EHeroAbility { HERO_ABILITY_NULL };

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1); // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                  // 0x0099EAE0
};

namespace NUISystem { struct CComponent; }
struct SComponentListNode;
struct CComponentList {                              // STLport list<NUISystem::CComponent*>
    SComponentListNode* m_pNode;
    ~CComponentList();                               // 0x0042ABCA
};
struct SActionParamPair {                            // pair<unsigned long, list<NUISystem::CComponent*> >
    unsigned long  first;
    CComponentList second;
    SActionParamPair();                              // 0x0042BED4
    SActionParamPair(const SActionParamPair& other); // 0x0042CD84
};

template<class T> struct CCountedPointer {
    T*    m_pObject;
    void* m_pCount;
    CCountedPointer(T* object);                      // 0x00429C15
    ~CCountedPointer();                              // 0x004291DE
    T* operator->() const { return m_pObject; }
};

namespace NUISystem {
struct CActionParam {
    long Type;                                       // +0x04
    CActionParam(long type) : Type(type) {}
    virtual void v00();
};
struct CActionParamEvent : CActionParam {            // vtable 0x0122F844, type 5
    unsigned long Value;                             // +0x08
    CActionParamEvent(unsigned long value) : CActionParam(5), Value(value) {}
    virtual void v00();
};
struct CActionParamLong : CActionParam {             // vtable 0x012366F4, type 3
    long Value;                                      // +0x08
    CActionParamLong(long value) : CActionParam(3), Value(value) {}
    virtual void v00();
};
struct CActionParamList {                            // STLport list<CActionParam*>
    void* _M_node;
    void push_back(CActionParam* const& value);      // 0x0042B646
};
struct CActionPointer;
struct CAction {                                     // retail 0x10 bytes
    long             ActionType;                     // +0x00
    CActionParamList UIActionParams;                 // +0x04
    void*            ThingActionParams;              // +0x08
    bool             Valid;                          // +0x0C
    CAction();                                       // 0x0042BE50
    void SetTarget(CActionParam* target, const CActionPointer& self);   // 0x0053FDE0
    void AddUIActionParam(CActionParam* param);      // 0x0053FDB0
};
struct CActionPointer {
    CAction* m_pObject;
    void*    m_pCount;
    CActionPointer(CAction* object);                 // 0x0042AA29
    ~CActionPointer();                               // 0x00429160
    CAction* operator->() const { return m_pObject; }
};

struct CComponentBase {
    virtual void v000(); virtual void v001(); virtual void v002();
    virtual CComponent* FindChild(CCharString name);           // slot 3 (+0xC)
    virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021();
    virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033();
    virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045();
    virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057();
    virtual void v058(); virtual void v059();
    virtual void SetContents_slot60(const CCountedPointer<CComponent>& child); // slot 60 (+0xF0)
    virtual void v061();
    virtual void SetActionParameter_slot62(SActionParamPair parameter); // slot 62 (+0xF8)
    virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068();
    virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073();
    virtual void AddAction_slot74(const CActionPointer& action); // slot 74 (+0x128)
    virtual void AddAction_slot75(const CActionPointer& action); // slot 75 (+0x12C)
    virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081();
    virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087();
    virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093();
    virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099();
    virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105();
    virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111();
    virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117();
    virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123();
    virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129();
    virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135();
    virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141();
    virtual void v142(); virtual void v143(); virtual void v144();
    virtual void SetGraphic_slot145(unsigned long bankIndex);  // slot 145 (+0x244)
};
struct CObserver {
    virtual void o00(); virtual void o01(); virtual void o02();
    virtual void ObserveEvent(long eventType);       // slot 3 (+0x0C)
    unsigned char _pad08[0x18 - 0x08];
};
struct CComponent : CComponentBase, CObserver, CActionParam {   // CObserver +0x04, action target +0x18
};
struct CManager {
    CComponent* CreateComponent(const CCharString& definition, bool b);   // 0x0041DB1D
};
struct CFrontEndManager : CManager {
    static CFrontEndManager* __fastcall GetInstance();   // 0x0041E5F2
};
}

struct SIconNode {                                   // STLport _Rb_tree_node<pair<const EHeroAbility, CComponent*> >
    unsigned char          _links[0x10];
    EHeroAbility           first;                    // +0x10
    NUISystem::CComponent* second;                   // +0x14
};
struct CIconIterator {
    SIconNode* _M_node;
    CIconIterator(SIconNode* node) : _M_node(node) {}
    CIconIterator(const CIconIterator& other) : _M_node(other._M_node) {}
};
struct CIconMap {                                    // map<EHeroAbility, NUISystem::CComponent*> (retail 12 bytes)
    SIconNode* _M_header;
    unsigned char _rest[8];
    SIconNode* find(const EHeroAbility& key);        // 0x005F8444
    void erase(CIconIterator position);              // 0x0057926B
    NUISystem::CComponent*& operator[](const EHeroAbility& key);   // 0x005F9EA5
};

struct CHeroAbilityDef {
    unsigned char _pad00[0x44];
    unsigned long IconBank;                          // +0x44
};
struct SAbilityEntry { long first; CHeroAbilityDef* second; };   // pair<long, CDefPointer<const CHeroAbilityDef> >

template<class T> struct CVector {                   // retail STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    T& operator[](unsigned long n) { return _M_start[n]; }
};

struct CTCInventoryAbilities {
    unsigned char           _pad_0x000[0x160];
    CVector<SAbilityEntry>  Abilities;               // +0x160 (PDB +0x180)
    unsigned char           _pad_0x16c[0x17c - 0x16c];
    CVector<EHeroAbility>   AssignedAbilities;       // +0x17c (PDB +0x1a4)
    unsigned char           _pad_0x188[0x1a0 - 0x188];
    CIconMap                UnassignedIcons;         // +0x1a0 (PDB +0x1d4)
    CIconMap                AbilitiesMenuEntries;    // +0x1ac (PDB +0x1e4)
    void AssignAbilityPC(long ability, unsigned long slot, NUISystem::CComponent* container);
};

void CTCInventoryAbilities::AssignAbilityPC(long ability, unsigned long slot, NUISystem::CComponent* container)
{
    const EHeroAbility& current = AssignedAbilities[slot];
    EHeroAbility previous = current;
    bool assignedElsewhere = false;
    for (unsigned long i = 0; i < AssignedAbilities.size(); ++i)
        if (AssignedAbilities[i] == previous && i != slot)
            assignedElsewhere = true;
    if ((slot & 1) == 0 && slot < AssignedAbilities.size() - 1) {
        if (AssignedAbilities[slot + 1] == ability)
            return;
    } else if (slot > 0 && AssignedAbilities[slot - 1] == ability)
        return;
    if (!assignedElsewhere && ability != previous && previous != 0) {
        CCountedPointer<NUISystem::CComponent> unassigned(NUISystem::CFrontEndManager::GetInstance()->CreateComponent("PC_UNASSIGNED_SPELL_ICON", false));
        AbilitiesMenuEntries[previous]->SetContents_slot60(unassigned);
        NUISystem::CComponent* icon = unassigned.m_pObject;
        UnassignedIcons[previous] = icon;
    }
    AssignedAbilities[slot] = (EHeroAbility)ability;
    SIconNode* it = UnassignedIcons.find((EHeroAbility)ability);
    if (it != UnassignedIcons._M_header) {
        NUISystem::CComponent* unassignedIcon = it->second;
        NUISystem::CComponent* icon = unassignedIcon->FindChild("PC_UNASSIGNED_SPELL_ICON");
        if (icon) {
            SActionParamPair parameter;
            parameter.first = 1;
            icon->SetActionParameter_slot62(parameter);
        }
        UnassignedIcons.erase(it);
    }
    NUISystem::CFrontEndManager* manager = NUISystem::CFrontEndManager::GetInstance();
    NUISystem::CComponent* old = container->FindChild(CCharString(previous != 0 ? "PC_SPELL_ASSIGNMENT_ICON_INSIDE_CONTAINER" : "PC_SPELL_INSIDE_CONTAINER", -1));
    if (old) {
        SActionParamPair parameter;
        parameter.first = 1;
        old->SetActionParameter_slot62(parameter);
    }
    CCountedPointer<NUISystem::CComponent> item(manager->CreateComponent("PC_SPELL_ASSIGNMENT_ICON_INSIDE_CONTAINER", false));
    NUISystem::CComponent* sprite = item->FindChild("PC_SPELL_ASSIGNMENT_ICON_SPRITE");
    if (sprite)
        sprite->SetGraphic_slot145(Abilities[ability].second->IconBank);
    NUISystem::CActionPointer drop(new NUISystem::CAction);
    drop->ActionType = 0xf8;
    drop->SetTarget(container, drop);
    drop->UIActionParams.push_back(new NUISystem::CActionParamEvent(slot));
    item->AddAction_slot75(drop);
    item->ObserveEvent(0x19);
    NUISystem::CActionPointer use(new NUISystem::CAction);
    use->ActionType = 0x148;
    use->UIActionParams.push_back(new NUISystem::CActionParamLong(ability));
    use->UIActionParams.push_back(new NUISystem::CActionParamLong(slot));
    use->AddUIActionParam(container);
    item->AddAction_slot74(use);
    container->SetContents_slot60(item);
}
