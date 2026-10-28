// CTCInventoryExperience::UndoBuyAbility(EHeroAbility ability, bool confirmed) -- asks (yes/no dialog) whether to undo an ability purchase; once confirmed, lowers the ability level, reverts its spent experience and drops the undo record
// Grade: DIFFER(642v642) -- residue (2 mnemonic diffs): the ChangePosition argument build schedules `push ecx` and the x87 fsubp in the opposite order (the same swap as the parked 0x0047E651 / 0x0047F30F / 0x0047E9AE); lane gap: x87 scheduling | tried: full PDB respell (CUIScreenUnitConverter::GetBackBufferWidth/Height, CManager::GetUIScale / GetInstance, CObservable RemoveObserver / AddConcurrentExclusiveObserver, CObserver ObserveEvent / ObserveAllEvents, CYesNo SetYesCallBack / SetNoCallBack, CComponent ChangeState / ChangePosition, EnsureUndoAbility / CancelUndoAbility, CTCInventoryAbilities GetAbilityLevel / SetAbilityLevelTo, RevertExperiencePoints, map find via _M_find + erase), argument-temporary and local forms of the position (inert); 1 RTM grade this pass | next: none in source (shared x87 lane gap)
// Leaf: ?UndoBuyAbility@CTCInventoryExperience@@QAEXW4EHeroAbility@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00576C4F, 642 bytes, `ret 8`; frame-pointer size TU
//   (`optimize("y", off)` + `("s", on)`, as the landed siblings 0x00576C39 / 0x0057713F of the same TU).
// Identity: Ego_r.pdb / FableWin.pdb ?UndoBuyAbility@CTCInventoryExperience@@QAEXW4EHeroAbility@@_N@Z; it installs the
//   callbacks ?EnsureUndoAbility@@YIXXZ 0x00570009 and ?CancelUndoAbility@@YIXXZ 0x0056E866 (landed), and retail places it
//   right before UndoBuyStat 0x00576ED1 as Ego_r does.
// Callees: CYesNo::SetYesCallBack / SetNoCallBack (vtable 0x0122F884 slots 142 / 143), CComponent::ChangeState(unsigned
//   long) (slot 48), CComponent::ChangePosition(const C2DVector&, float, bool) (slot 31), CComponent::FindChild(CCharString)
//   (slot 3) with CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; NUISystem::CUIScreenUnitConverter
//   GetBackBufferWidth / GetBackBufferHeight 0x0041CC14 / 0x0041CC2B, CManager::GetUIScale() const 0x0041CF47,
//   CManager::GetInstance 0x0041E5F2 (CManager vtable 0x01230134: CObservable RemoveObserver 5 /
//   AddConcurrentExclusiveObserver 9), CObserver ObserveEvent 3 / ObserveAllEvents 6; SaveExperienceValuesForCountDown
//   0x00570161, the undo map's _M_find 0x0057914C and erase 0x00579D55, GetTCHeroExperience 0x0056FF3D,
//   GetTCInventoryAbilities 0x0056FFC5, CTCInventoryAbilities::GetAbilityLevel 0x005EDE93 / SetAbilityLevelTo 0x005EE01A,
//   SExpSpendUndo::RevertExperiencePoints 0x0056F4BA.
// Layout: CTCInventoryExperience UndoableAbilityBuys +0x14C, pYesNo +0x1C4, CreateBecauseOfUndo +0x214,
//   DestroyEverything +0x21C, PMousePointer +0x258 (engine/CTCInventoryExperience.h; local polymorphic model: the header
//   flattens the vptr). The pending ability / dialog / pointer globals 0x013B8B08 / 0x013B8B04 / 0x013B8B00 are file
//   statics with no PDB name (bound names kept, descriptive).
#pragma optimize("y", off)
#pragma optimize("s", on)

class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                   // 0x0099EAE0
};
class C2DVector {
public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) { }
};

namespace std {
template <class _Tp> class allocator { };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Pair> struct _Select1st { };
template <class _Tp> struct less { };
template <class _Tp> struct _Nonconst_traits { };
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list {   // STLport 4.6: the sentinel pointer
public:
    _List_node_base* _M_node;
    unsigned int size() const
    {
        unsigned int __n = 0;
        for (_List_node_base* __p = _M_node->_M_next; __p != _M_node; __p = __p->_M_next)
            ++__n;
        return __n;
    }
};
struct _Rb_tree_node_base {
    bool                _M_color;                     // +0x00
    _Rb_tree_node_base* _M_parent;                    // +0x04
    _Rb_tree_node_base* _M_left;                      // +0x08
    _Rb_tree_node_base* _M_right;                     // +0x0c
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                            // +0x10
};
template <class _Value, class _Traits> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) : _M_node(__it._M_node) { }
    _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    _Value* operator->() const { return &(operator*()); }
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc> class _Rb_tree {
public:
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    _Rb_tree_node_base* _M_header;                    // +0x00
    unsigned int        _M_node_count;                // +0x04
    unsigned char       _M_key_compare;               // +0x08
    iterator find(const _Key& __k) { return iterator(_M_find(__k)); }
    void erase(iterator __position);                  // out of line
private:
    _Rb_tree_node<_Value>* _M_find(const _Key& __k) const;   // out of line
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;
    iterator find(const _Key& __x) { return _M_t.find(__x); }
    void erase(iterator __position) { _M_t.erase(__position); }
};
}

namespace NUISystem {
class CUIScreenUnitConverter {
public:
    static float __fastcall GetBackBufferWidth();     // 0x0041CC14
    static float __fastcall GetBackBufferHeight();    // 0x0041CC2B
};
enum EEvent { EVENT_25 = 0x19 };
class CObserver {                                     // at +4 of every component
public:
    virtual void o00(); virtual void o01(); virtual void o02();
    virtual void ObserveEvent(EEvent event);          // slot 3 (+0x0C)
    virtual void o04(); virtual void o05();
    virtual void ObserveAllEvents();                  // slot 6 (+0x18)
};
class CObservable {                                   // CManager vtable 0x01230134
public:
    virtual void b00(); virtual void b01(); virtual void b02(); virtual void b03(); virtual void b04();
    virtual void RemoveObserver(CObserver* observer); // slot 5 (+0x14) 0x0055C9C0
    virtual void b06(); virtual void b07(); virtual void b08();
    virtual void AddConcurrentExclusiveObserver(CObserver* observer);   // slot 9 (+0x24) 0x0055CA90
};
#define V10(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3(); virtual void n##4(); \
    virtual void n##5(); virtual void n##6(); virtual void n##7(); virtual void n##8(); virtual void n##9();
class CUIInterface {
public:
    virtual void u00(); virtual void u01(); virtual void u02();
};
class CComponent : public CUIInterface, public CObserver {   // RTTI: CUIInterface@0, CObserver@4, CActionParam@24
public:
    virtual CComponent* FindChild(CCharString name);  // slot 3 (+0x0C)
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    V10(v1) V10(v2)
    virtual void v30();
    virtual void ChangePosition(const C2DVector& position, float time, bool b);   // slot 31 (+0x7C)
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37();
    virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43();
    virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void ChangeState(unsigned long state);    // slot 48 (+0xC0)
    virtual void v49();
    V10(v5) V10(v6) V10(v7) V10(v8)
};
class CChangingStateComponent : public CComponent {
public:
    V10(v9) V10(w0) V10(w1) V10(w2) V10(w3)
    virtual void w40(); virtual void w41();
};
class CYesNo : public CChangingStateComponent {       // vtable 0x0122F884
public:
    virtual void SetYesCallBack(void (__fastcall* callBack)());   // slot 142 (+0x238)
    virtual void SetNoCallBack(void (__fastcall* callBack)());    // slot 143 (+0x23C)
};
class CManager : public CObservable {
public:
    C2DVector GetUIScale() const;                     // 0x0041CF47
    static CManager* __fastcall GetInstance();        // 0x0041E5F2
};
}

enum EHeroAbility { EHeroAbility_Force32 = 0x7fffffff };
enum EHeroTrainableStatType { EHeroTrainableStatType_Force32 = 0x7fffffff };
class CTCHeroExperience;
struct SExpSpendUndo {                                // Ego_r.pdb: Generals, Strengths, Skills, Wills (list<long>), nine longs
    std::list<long> Generals;                         // +0x00
    void RevertExperiencePoints(CTCHeroExperience* experience);   // 0x0056F4BA
};
class CTCInventoryAbilities {
public:
    long GetAbilityLevel(EHeroAbility ability) const;                // 0x005EDE93
    void SetAbilityLevelTo(EHeroAbility ability, long level);       // 0x005EE01A
};

extern EHeroAbility g_UndoAbility;                                   // 0x013B8B08 (file static, descriptive)
extern NUISystem::CYesNo* g_pUndoYesNoDialog;                        // 0x013B8B04 (file static, descriptive)
extern NUISystem::CComponent* g_pUndoMousePointer;                   // 0x013B8B00 (file static, descriptive)
void __fastcall EnsureUndoAbility();                                 // 0x00570009
void __fastcall CancelUndoAbility();                                 // 0x0056E866

// Layout: see the header comment.
class CTCInventoryExperience {
public:
    virtual void v00();
    unsigned char _pad_0x04[0x13C];
    std::map<EHeroTrainableStatType, SExpSpendUndo> UndoableStatBuys;   // +0x140
    std::map<EHeroAbility, SExpSpendUndo> UndoableAbilityBuys;          // +0x14C
    unsigned char _pad_0x158[0x6C];
    NUISystem::CYesNo* pYesNo;                                           // +0x1C4
    unsigned char _pad_0x1c8[0x4C];
    bool CreateBecauseOfUndo;                                            // +0x214
    unsigned char _pad_0x215[0x07];
    bool DestroyEverything;                                              // +0x21C
    unsigned char _pad_0x21d[0x3B];
    NUISystem::CComponent* PMousePointer;                                // +0x258
    void SaveExperienceValuesForCountDown();                             // 0x00570161
    void UndoBuyAbility(EHeroAbility ability, bool confirmed);
private:
    CTCHeroExperience* GetTCHeroExperience();                            // 0x0056FF3D
    CTCInventoryAbilities* GetTCInventoryAbilities();                    // 0x0056FFC5
};

void CTCInventoryExperience::UndoBuyAbility(EHeroAbility ability, bool confirmed)
{
    if (!confirmed) {
        g_UndoAbility = ability;
        pYesNo->SetYesCallBack(EnsureUndoAbility);
        pYesNo->ChangeState(1);
        pYesNo->ChangePosition(C2DVector(NUISystem::CUIScreenUnitConverter::GetBackBufferWidth() * 0.5f - NUISystem::CManager::GetInstance()->GetUIScale().X * 104.0f,
                                         NUISystem::CUIScreenUnitConverter::GetBackBufferHeight() * 0.5f - NUISystem::CManager::GetInstance()->GetUIScale().Y * 52.0f), 0.0f, false);
        pYesNo->SetNoCallBack(CancelUndoAbility);
        g_pUndoYesNoDialog = pYesNo;
        g_pUndoMousePointer = PMousePointer;
        NUISystem::CManager::GetInstance()->RemoveObserver(g_pUndoYesNoDialog);
        NUISystem::CManager::GetInstance()->AddConcurrentExclusiveObserver(g_pUndoYesNoDialog);
        NUISystem::CManager::GetInstance()->RemoveObserver(g_pUndoMousePointer);
        NUISystem::CManager::GetInstance()->AddConcurrentExclusiveObserver(g_pUndoMousePointer);
        g_pUndoMousePointer->ObserveEvent(NUISystem::EVENT_25);
        g_pUndoYesNoDialog->ObserveAllEvents();
        if (NUISystem::CComponent* yes = pYesNo->FindChild(CCharString("PC_YESNO_GAME_YESBUTTON", -1))) {
            NUISystem::CManager::GetInstance()->AddConcurrentExclusiveObserver(yes);
            yes->ObserveEvent(NUISystem::EVENT_25);
        }
        if (NUISystem::CComponent* no = pYesNo->FindChild(CCharString("PC_YESNO_GAME_NOBUTTON", -1))) {
            NUISystem::CManager::GetInstance()->AddConcurrentExclusiveObserver(no);
            no->ObserveEvent(NUISystem::EVENT_25);
        }
    } else {
        CreateBecauseOfUndo = true;
        SaveExperienceValuesForCountDown();
        std::map<EHeroAbility, SExpSpendUndo>::iterator it = UndoableAbilityBuys.find(ability);
        GetTCInventoryAbilities()->SetAbilityLevelTo(ability, GetTCInventoryAbilities()->GetAbilityLevel(ability) - 1);
        it->second.RevertExperiencePoints(GetTCHeroExperience());
        if (it->second.Generals.size() == 0)
            UndoableAbilityBuys.erase(it);
        DestroyEverything = true;
    }
}
