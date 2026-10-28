// CTCInventoryExperience::RecheckAvailabilities() -- drop the upgrade highlight (a deletion of method 1) from every stat and ability that can no longer be upgraded with the experience available, refresh the general/strength/skill/will experience values, and close the screen when nothing is left to upgrade
// Grade: DIFFER(824v824) -- residue (4 mnemonic diffs): in both erase loops the deletion.Method = 1 store sits above the component vtable load and the copy-ctor argument setup where retail writes it just before the call to CDeletion(const CDeletion&) 0x0042CD84; lane gap: store scheduling | tried: real names (RecheckAvailabilities; the catalog's UpdateAbilityDisplay is in neither PDB), the old 'CAction' respelled as NUISystem::CDeletion with its list member, real STLport _Rb_tree/map with the out-of-line erase, CText::SetValue slot 144, CTCInventoryExperience::AddCloseInventoryEvent slot 32 (vtable 0x0124F11C), it->second->SetDeletion(deletion) without a component local (8 -> 4), (*it).second form, component declared first (16), the SetMethod-in-argument form, same-TU CDeletion ctor bodies (inert); 1 RTM grade this pass (13 total) | next: none in source; 0x00584B86 (CDeletion on a register-held component) does schedule the store after the vtable load, so the difference is the map-node load of it->second
// Leaf: ?RecheckAvailabilities@CTCInventoryExperience@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005726A0, 824 bytes, thiscall, frame-pointer size TU
//   (`#pragma optimize("y", off)` + `optimize("s", on)`, as the landed siblings 0x00572636 / 0x005747A3), `leave ; ret`.
// Identity: Ego_r.pdb and FableWin.pdb ?RecheckAvailabilities@CTCInventoryExperience@@AAEXXZ -- the class's only private
//   no-argument member that re-tests upgrade availability (the body compares GetCostToUpgradeTrainableStat /
//   GetExperienceCostToUpgradeAbility with the stat experience plus the general pool and drops the entries that no longer
//   qualify). The catalog label "UpdateAbilityDisplay" is in neither PDB. Retail has no caller of 0x005726A0.
// Callees: GetTCHeroExperience 0x0056FF3D / GetTCInventoryAbilities 0x0056FFC5 (private), CTCHeroExperience
//   GetTrainableStatMaxLevel 0x006D6770 / GetTrainableStatLevel 0x006D6520 / GetCategoryTrainableStatIsIn 0x006D6850 /
//   GetStatExperience 0x006D6510 / GetCostToUpgradeTrainableStat 0x006D67B0, CTCInventoryAbilities
//   IsAbilityPotentiallyAvailable 0x005EE02E / GetAbilityMaximumLevel 0x005EDEA3 / GetAbilityLevel 0x005EDE93 /
//   GetAbilityStatType 0x005EDEEB / GetExperienceCostToUpgradeAbility 0x005EE77C, NUISystem::CDeletion() 0x0042BED4 /
//   CDeletion(const CDeletion&) 0x0042CD84 into CComponent::SetDeletion(CDeletion) (slot 62), the list destructor
//   0x0042ABCA (inline ~CDeletion), _Rb_global<bool>::_M_increment 0x004292C0, the two map erases 0x00579300 (stats) /
//   0x0057926B (abilities), CComponent::FindChild(CCharString) (slot 3) with CCharString(const char*, long) 0x0099EBF0,
//   GFIntToWideString 0x00405F00 and CText::SetValue(const CWideString&, bool, bool) (slot 144), ~CWideString
//   0x0099B510, and the vtable slot 32 AddCloseInventoryEvent (vtable 0x0124F11C slot 32 = 0x005BC527).
// Layout: NInventory::CTCInventoryBase::Components +0x0C (Ego_r); CTCInventoryExperience UpgradableStats +0x224 /
//   UpgradableAbilities +0x230 (engine/CTCInventoryExperience.h). Local model: the generated header flattens the vptr
//   into a data member, so the virtual slot 32 call needs a polymorphic local model (header debt).
#pragma optimize("y", off)
#pragma optimize("s", on)
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
class CWideString {
public:
    void* PStringData;
    ~CWideString();                                     // 0x0099B510
};
CWideString __fastcall GFIntToWideString(long value);  // 0x00405F00

namespace NUISystem { class CComponent; }

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    void* _M_node;                                      // +0x00 (STLport, 4 bytes)
    ~list();                                            // 0x0042ABCA
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Pair> struct _Select1st { };
template <class _Tp> struct less { };
template <class _Tp> struct _Nonconst_traits { };
struct _Rb_tree_node_base {
    bool                _M_color;                       // +0x00
    _Rb_tree_node_base* _M_parent;                      // +0x04
    _Rb_tree_node_base* _M_left;                        // +0x08
    _Rb_tree_node_base* _M_right;                       // +0x0c
};
template <class _Dummy> class _Rb_global {
public:
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* __x);   // 0x004292C0
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                              // +0x10
};
template <class _Value, class _Traits> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) : _M_node(__it._M_node) { }
    _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    _Value* operator->() const { return &(operator*()); }
    _Rb_tree_iterator& operator++() { _M_node = _Rb_global<bool>::_M_increment(_M_node); return *this; }
    bool operator!=(const _Rb_tree_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc> class _Rb_tree {
public:
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    _Rb_tree_node_base* _M_header;                      // +0x00
    unsigned int        _M_node_count;                  // +0x04
    unsigned char       _M_key_compare;                 // +0x08
    void erase(iterator __position);                    // out of line (0x00579300 / 0x0057926B)
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    iterator begin() { return iterator(_M_t._M_header->_M_left); }
    iterator end() { return iterator(_M_t._M_header); }
    unsigned int size() const { return _M_t._M_node_count; }
    void erase(iterator __position) { _M_t.erase(__position); }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    _Tp& operator[](unsigned int __n) { return *(_M_start + __n); }
};
}

namespace NUISystem {
enum EDeletionMethod { DELETION_METHOD_NONE, DELETION_METHOD_1 };
class CDeletion {
public:
    EDeletionMethod        Method;                      // +0x00
    std::list<CComponent*> AssociatedParents;           // +0x04
    CDeletion();                                        // 0x0042BED4
    CDeletion(const CDeletion& other);                  // 0x0042CD84
};
class CText;
class CComponent {
public:
    virtual void v000(); virtual void v001(); virtual void v002();
    virtual CComponent* FindChild(CCharString name);    // slot 3 (+0x0c)
    virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021();
    virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033();
    virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045();
    virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057();
    virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061();
    virtual void SetDeletion(CDeletion deletion);       // slot 62 (+0xf8)
};
class CText : public CComponent {
public:
    virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075();
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
    virtual void v142(); virtual void v143();
    virtual void SetValue(const CWideString& value, bool a, bool b);   // slot 144 (+0x240)
};
}

enum EHeroTrainableStatType { EHeroTrainableStatType_Force32 = 0x7fffffff };
enum EHeroAbility { EHeroAbility_Force32 = 0x7fffffff };
enum EHeroExperienceStatCategory { HERO_STAT_STRENGTH = 0, HERO_STAT_SKILL = 1, HERO_STAT_WILL = 2 };

class CTCHeroExperience {
public:
    unsigned char _pad_0x00[0x14];
    long GeneralExperience;                                                                 // +0x14
    long GetStatExperience(EHeroExperienceStatCategory category) const;                     // 0x006D6510
    long GetTrainableStatLevel(EHeroTrainableStatType stat) const;                          // 0x006D6520
    long GetTrainableStatMaxLevel(EHeroTrainableStatType stat) const;                       // 0x006D6770
    long GetCostToUpgradeTrainableStat(EHeroTrainableStatType stat) const;                  // 0x006D67B0
    EHeroExperienceStatCategory GetCategoryTrainableStatIsIn(EHeroTrainableStatType stat) const;   // 0x006D6850
};
class CTCInventoryAbilities {
public:
    bool IsAbilityPotentiallyAvailable(EHeroAbility ability) const;                         // 0x005EE02E
    long GetAbilityMaximumLevel(EHeroAbility ability) const;                                // 0x005EDEA3
    long GetAbilityLevel(EHeroAbility ability) const;                                       // 0x005EDE93
    EHeroExperienceStatCategory GetAbilityStatType(EHeroAbility ability) const;             // 0x005EDEEB
    long GetExperienceCostToUpgradeAbility(EHeroAbility ability, long levels, bool a) const;   // 0x005EE77C
};

// Layout: see the header comment (local polymorphic model of engine/CTCInventoryExperience.h).
class CTCInventoryExperience {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31();
    virtual void AddCloseInventoryEvent();                                                  // slot 32 (+0x80)
    unsigned char _pad_0x04[0x08];
    std::vector<NUISystem::CComponent*> Components;                                         // +0x0c
    unsigned char _pad_0x18[0x224 - 0x18];
    std::map<EHeroTrainableStatType, NUISystem::CComponent*> UpgradableStats;               // +0x224
    std::map<EHeroAbility, NUISystem::CComponent*> UpgradableAbilities;                     // +0x230
private:
    CTCHeroExperience* GetTCHeroExperience();                                               // 0x0056FF3D
    CTCInventoryAbilities* GetTCInventoryAbilities();                                       // 0x0056FFC5
    void RecheckAvailabilities();
};

void CTCInventoryExperience::RecheckAvailabilities()
{
    {
    long general = GetTCHeroExperience()->GeneralExperience;
    for (std::map<EHeroTrainableStatType, NUISystem::CComponent*>::iterator it = UpgradableStats.begin(); it != UpgradableStats.end(); ) {
        if (GetTCHeroExperience()->GetTrainableStatLevel(it->first) < GetTCHeroExperience()->GetTrainableStatMaxLevel(it->first) &&
            GetTCHeroExperience()->GetCostToUpgradeTrainableStat(it->first) >
                GetTCHeroExperience()->GetStatExperience(GetTCHeroExperience()->GetCategoryTrainableStatIsIn(it->first)) + general) {
            NUISystem::CDeletion deletion;
            deletion.Method = NUISystem::DELETION_METHOD_1;
            it->second->SetDeletion(deletion);
            std::map<EHeroTrainableStatType, NUISystem::CComponent*>::iterator next = it;
            ++next;
            UpgradableStats.erase(it);
            it = next;
        } else
            ++it;
    }
    for (std::map<EHeroAbility, NUISystem::CComponent*>::iterator it = UpgradableAbilities.begin(); it != UpgradableAbilities.end(); ) {
        if (GetTCInventoryAbilities()->IsAbilityPotentiallyAvailable(it->first) &&
            GetTCInventoryAbilities()->GetAbilityLevel(it->first) < GetTCInventoryAbilities()->GetAbilityMaximumLevel(it->first) &&
            GetTCInventoryAbilities()->GetExperienceCostToUpgradeAbility(it->first, 1, true) >
                GetTCHeroExperience()->GetStatExperience(GetTCInventoryAbilities()->GetAbilityStatType(it->first)) + general) {
            NUISystem::CDeletion deletion;
            deletion.Method = NUISystem::DELETION_METHOD_1;
            it->second->SetDeletion(deletion);
            std::map<EHeroAbility, NUISystem::CComponent*>::iterator next = it;
            ++next;
            UpgradableAbilities.erase(it);
            it = next;
        } else
            ++it;
    }
    }
    NUISystem::CComponent* screen = Components[1];
    {
        NUISystem::CText* text = (NUISystem::CText*)screen->FindChild("UI_TEXT_EXPERIENCE_GENERAL_VALUE");
        long general = GetTCHeroExperience()->GeneralExperience;
        text->SetValue(GFIntToWideString(general), true, true);
    }
    ((NUISystem::CText*)screen->FindChild("UI_TEXT_EXPERIENCE_STRENGTH_VALUE"))->SetValue(GFIntToWideString(GetTCHeroExperience()->GetStatExperience(HERO_STAT_STRENGTH)), true, true);
    ((NUISystem::CText*)screen->FindChild("UI_TEXT_EXPERIENCE_DEXTERITY_VALUE"))->SetValue(GFIntToWideString(GetTCHeroExperience()->GetStatExperience(HERO_STAT_SKILL)), true, true);
    ((NUISystem::CText*)screen->FindChild("UI_TEXT_EXPERIENCE_WILL_VALUE"))->SetValue(GFIntToWideString(GetTCHeroExperience()->GetStatExperience(HERO_STAT_WILL)), true, true);
    if (UpgradableStats.size() == 0 && UpgradableAbilities.size() == 0)
        AddCloseInventoryEvent();
}
