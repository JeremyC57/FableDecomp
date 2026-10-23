// CTCInventoryExperience::UpdateAbilityDisplay() -- drop the upgrade highlight from stats and abilities that can no longer be upgraded with the experience available, refresh the general/strength/skill/will experience values, and report when nothing is left to upgrade
// Grade: DIFFER(824v824) -- everything matches except one store per loop: retail writes action.ActionType = 1 after the vtable load and the copy-ctor argument setup (just before the call to 0x0042CD84); the lane hoists it above the vtable load (tried: component local before/after the action, an inline SetActionType in the argument; 12 grades). Levers found: callee declaration order picks which compare operand VC7.1 evaluates first; general experience in its own block frees [ebp-4] for the text temporaries
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                     // 0x0099EAE0
};
struct CWideString {
    void* PStringData;
    ~CWideString();                                     // 0x0099B510
};
CWideString __fastcall IntToWideString(long value);    // 0x00405F00
struct SComponentListNode;
struct CComponentList {                                 // STLport list<CActionParam*>: the node pointer only
    SComponentListNode* m_pNode;
    ~CComponentList();                                  // 0x0042ABCA
};
namespace NUISystem {
struct CAction {                                        // ActionType +0, parameter list +4 (retail 8 bytes)
    unsigned long  ActionType;
    CComponentList UIActionParams;
    CAction();                                          // 0x0042BED4
    CAction(const CAction& other);                      // 0x0042CD84
};
struct CComponent {
    virtual void c000(); virtual void c001(); virtual void c002();
    virtual CComponent* FindChild(CCharString name);           // slot 3 (+0xC)
    virtual void c004(); virtual void c005(); virtual void c006(); virtual void c007(); virtual void c008(); virtual void c009();
    virtual void c010(); virtual void c011(); virtual void c012(); virtual void c013(); virtual void c014(); virtual void c015();
    virtual void c016(); virtual void c017(); virtual void c018(); virtual void c019(); virtual void c020(); virtual void c021();
    virtual void c022(); virtual void c023(); virtual void c024(); virtual void c025(); virtual void c026(); virtual void c027();
    virtual void c028(); virtual void c029(); virtual void c030(); virtual void c031(); virtual void c032(); virtual void c033();
    virtual void c034(); virtual void c035(); virtual void c036(); virtual void c037(); virtual void c038(); virtual void c039();
    virtual void c040(); virtual void c041(); virtual void c042(); virtual void c043(); virtual void c044(); virtual void c045();
    virtual void c046(); virtual void c047(); virtual void c048(); virtual void c049(); virtual void c050(); virtual void c051();
    virtual void c052(); virtual void c053(); virtual void c054(); virtual void c055(); virtual void c056(); virtual void c057();
    virtual void c058(); virtual void c059(); virtual void c060(); virtual void c061();
    virtual void DoAction(CAction action);                     // slot 62 (+0xF8)
    virtual void c063(); virtual void c064(); virtual void c065(); virtual void c066(); virtual void c067(); virtual void c068();
    virtual void c069(); virtual void c070(); virtual void c071(); virtual void c072(); virtual void c073(); virtual void c074();
    virtual void c075(); virtual void c076(); virtual void c077(); virtual void c078(); virtual void c079(); virtual void c080();
    virtual void c081(); virtual void c082(); virtual void c083(); virtual void c084(); virtual void c085(); virtual void c086();
    virtual void c087(); virtual void c088(); virtual void c089(); virtual void c090(); virtual void c091(); virtual void c092();
    virtual void c093(); virtual void c094(); virtual void c095(); virtual void c096(); virtual void c097(); virtual void c098();
    virtual void c099(); virtual void c100(); virtual void c101(); virtual void c102(); virtual void c103(); virtual void c104();
    virtual void c105(); virtual void c106(); virtual void c107(); virtual void c108(); virtual void c109(); virtual void c110();
    virtual void c111(); virtual void c112(); virtual void c113(); virtual void c114(); virtual void c115(); virtual void c116();
    virtual void c117(); virtual void c118(); virtual void c119(); virtual void c120(); virtual void c121(); virtual void c122();
    virtual void c123(); virtual void c124(); virtual void c125(); virtual void c126(); virtual void c127(); virtual void c128();
    virtual void c129(); virtual void c130(); virtual void c131(); virtual void c132(); virtual void c133(); virtual void c134();
    virtual void c135(); virtual void c136(); virtual void c137(); virtual void c138(); virtual void c139(); virtual void c140();
    virtual void c141(); virtual void c142(); virtual void c143();
    virtual void SetText(const CWideString& text, bool a, bool b); // slot 144 (+0x240)
};
}
enum EHeroTrainableStatType { HERO_TRAINABLE_STAT_NULL };
enum EHeroAbility { HERO_ABILITY_NULL };
enum EHeroStat { HERO_STAT_STRENGTH, HERO_STAT_SKILL, HERO_STAT_WILL };
enum EHeroExperienceStatCategory { HERO_EXPERIENCE_STAT_CATEGORY_NULL };
struct CTCHeroExperience {
    unsigned char _pad00[0x14];
    long GeneralExperience;                             // +0x14
    long GetStatExperience(EHeroStat stat);             // 0x006D6510
    long GetTrainableStatLevel(EHeroTrainableStatType stat);      // 0x006D6520
    long GetTrainableStatMaxLevel(EHeroTrainableStatType stat);   // 0x006D6770
    long GetCostToUpgradeTrainableStat(EHeroTrainableStatType stat);   // 0x006D67B0
    EHeroStat GetCategoryTrainableStatIsIn(EHeroTrainableStatType stat);   // 0x006D6850
};
struct CTCInventoryAbilities {
    bool IsAbilityPotentiallyAvailable(EHeroAbility ability);     // 0x005EE02E
    long GetAbilityMaximumLevel(EHeroAbility ability);            // 0x005EDEA3
    long GetAbilityLevel(EHeroAbility ability);                   // 0x005EDE93
    EHeroStat GetAbilityStatType(EHeroAbility ability);           // 0x005EDEEB
    long GetExperienceCostToUpgradeAbility(EHeroAbility ability, long levels, bool a);   // 0x005EE77C
};
struct SMapNodeBase {
    long _M_color; SMapNodeBase* _M_parent; SMapNodeBase* _M_left; SMapNodeBase* _M_right;
};
extern "C" SMapNodeBase* Successor(SMapNodeBase* node);   // 0x004292C0
template <class K> struct SComponentNode : SMapNodeBase {
    K first;                                            // +0x10
    NUISystem::CComponent* second;                      // +0x14
};
template <class K> struct CComponentMapIterator {
    SComponentNode<K>* _M_node;
    CComponentMapIterator(SComponentNode<K>* node) : _M_node(node) {}
    CComponentMapIterator(const CComponentMapIterator& o) : _M_node(o._M_node) {}
    SComponentNode<K>* operator->() const { return _M_node; }
    bool operator!=(const CComponentMapIterator& o) const { return _M_node != o._M_node; }
    CComponentMapIterator& operator++() { _M_node = (SComponentNode<K>*)Successor(_M_node); return *this; }
    CComponentMapIterator operator++(int) { CComponentMapIterator tmp = *this; _M_node = (SComponentNode<K>*)Successor(_M_node); return tmp; }
};
struct CStatComponentMap {                              // map<EHeroTrainableStatType, CComponent*>
    SComponentNode<EHeroTrainableStatType>* _M_header;
    unsigned long _M_node_count;
    unsigned long _M_compare;
    typedef CComponentMapIterator<EHeroTrainableStatType> iterator;
    iterator begin() { return iterator((SComponentNode<EHeroTrainableStatType>*)_M_header->_M_left); }
    iterator end() { return iterator(_M_header); }
    unsigned long size() const { return _M_node_count; }
    void erase(iterator it);                            // 0x00579300
};
struct CAbilityComponentMap {                           // map<EHeroAbility, CComponent*>
    SComponentNode<EHeroAbility>* _M_header;
    unsigned long _M_node_count;
    unsigned long _M_compare;
    typedef CComponentMapIterator<EHeroAbility> iterator;
    iterator begin() { return iterator((SComponentNode<EHeroAbility>*)_M_header->_M_left); }
    iterator end() { return iterator(_M_header); }
    unsigned long size() const { return _M_node_count; }
    void erase(iterator it);                            // 0x0057926B
};
struct CComponentVector {
    NUISystem::CComponent** _M_start;
    NUISystem::CComponent** _M_finish;
    NUISystem::CComponent** _M_end_of_storage;
    NUISystem::CComponent*& operator[](unsigned long n) { return _M_start[n]; }
};
// Layout: retail CTCInventoryBase Components vector +0x0C; UpgradableStats +0x224 / UpgradableAbilities +0x230
//   (Ego_r.pdb +0x278 / +0x288; retail containers are 12 bytes).
struct CTCInventoryExperience {
    virtual void i000(); virtual void i001(); virtual void i002(); virtual void i003(); virtual void i004(); virtual void i005();
    virtual void i006(); virtual void i007(); virtual void i008(); virtual void i009(); virtual void i010(); virtual void i011();
    virtual void i012(); virtual void i013(); virtual void i014(); virtual void i015(); virtual void i016(); virtual void i017();
    virtual void i018(); virtual void i019(); virtual void i020(); virtual void i021(); virtual void i022(); virtual void i023();
    virtual void i024(); virtual void i025(); virtual void i026(); virtual void i027(); virtual void i028(); virtual void i029();
    virtual void i030(); virtual void i031();
    virtual void OnNoUpgradesLeft();                           // slot 32 (+0x80)
    unsigned char _pad04[0x0c - 0x04];
    CComponentVector Components;                        // +0x0c
    unsigned char _pad18[0x224 - 0x18];
    CStatComponentMap UpgradableStats;                  // +0x224
    CAbilityComponentMap UpgradableAbilities;           // +0x230
    CTCHeroExperience* GetTCHeroExperience();           // 0x0056FF3D
    CTCInventoryAbilities* GetTCInventoryAbilities();   // 0x0056FFC5
    void UpdateAbilityDisplay();
};

void CTCInventoryExperience::UpdateAbilityDisplay()
{
    {
    long general = GetTCHeroExperience()->GeneralExperience;
    for (CStatComponentMap::iterator it = UpgradableStats.begin(); it != UpgradableStats.end(); ) {
        if (GetTCHeroExperience()->GetTrainableStatLevel(it->first) < GetTCHeroExperience()->GetTrainableStatMaxLevel(it->first) &&
            GetTCHeroExperience()->GetCostToUpgradeTrainableStat(it->first) >
                GetTCHeroExperience()->GetStatExperience(GetTCHeroExperience()->GetCategoryTrainableStatIsIn(it->first)) + general) {
            NUISystem::CAction action;
            NUISystem::CComponent* component = it->second;
            action.ActionType = 1;
            component->DoAction(action);
            CStatComponentMap::iterator next = it;
            ++next;
            UpgradableStats.erase(it);
            it = next;
        } else
            ++it;
    }
    for (CAbilityComponentMap::iterator it = UpgradableAbilities.begin(); it != UpgradableAbilities.end(); ) {
        if (GetTCInventoryAbilities()->IsAbilityPotentiallyAvailable(it->first) &&
            GetTCInventoryAbilities()->GetAbilityLevel(it->first) < GetTCInventoryAbilities()->GetAbilityMaximumLevel(it->first) &&
            GetTCInventoryAbilities()->GetExperienceCostToUpgradeAbility(it->first, 1, true) >
                GetTCHeroExperience()->GetStatExperience(GetTCInventoryAbilities()->GetAbilityStatType(it->first)) + general) {
            NUISystem::CAction action;
            NUISystem::CComponent* component = it->second;
            action.ActionType = 1;
            component->DoAction(action);
            CAbilityComponentMap::iterator next = it;
            ++next;
            UpgradableAbilities.erase(it);
            it = next;
        } else
            ++it;
    }
    }
    NUISystem::CComponent* screen = Components[1];
    {
        NUISystem::CComponent* text = screen->FindChild("UI_TEXT_EXPERIENCE_GENERAL_VALUE");
        long general = GetTCHeroExperience()->GeneralExperience;
        text->SetText(IntToWideString(general), true, true);
    }
    screen->FindChild("UI_TEXT_EXPERIENCE_STRENGTH_VALUE")->SetText(IntToWideString(GetTCHeroExperience()->GetStatExperience(HERO_STAT_STRENGTH)), true, true);
    screen->FindChild("UI_TEXT_EXPERIENCE_DEXTERITY_VALUE")->SetText(IntToWideString(GetTCHeroExperience()->GetStatExperience(HERO_STAT_SKILL)), true, true);
    screen->FindChild("UI_TEXT_EXPERIENCE_WILL_VALUE")->SetText(IntToWideString(GetTCHeroExperience()->GetStatExperience(HERO_STAT_WILL)), true, true);
    if (UpgradableStats.size() == 0 && UpgradableAbilities.size() == 0)
        OnNoUpgradesLeft();
}
