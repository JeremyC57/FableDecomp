// CTCInventoryExperience::UndoBuyStat(EHeroTrainableStatType stat, bool confirmed) -- asks (yes/no dialog) whether to undo a stat purchase; once confirmed, lowers the stat level, reverts its spent experience and drops the undo record
// Grade: DIFFER(622v622) -- residue (4 mnemonic diffs): the ChangePosition `push ecx` / fsubp swap (as 0x00576C4F) plus one load-order swap after GetInstance (retail reads g_pUndoMousePointer before the manager vtable); lane gap: scheduling | tried: the 0x00576C4F respell with EnsureUndoStat / CancelUndoStat / ModifyTrainableStatLevel / the stat map's _M_find and erase, dialog/pointer locals, casts, callback-pointer and dialog-type variants (inert); 1 RTM grade this pass | next: none in source
// Leaf: ?UndoBuyStat@CTCInventoryExperience@@QAEXW4EHeroTrainableStatType@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00576ED1, 622 bytes, `ret 8`; frame-pointer size TU
//   (`optimize("y", off)` + `("s", on)`, as the landed siblings 0x00576C39 / 0x0057713F of the same TU).
// Identity: Ego_r.pdb / FableWin.pdb ?UndoBuyStat@CTCInventoryExperience@@QAEXW4EHeroTrainableStatType@@_N@Z; it installs
//   the callbacks ?EnsureUndoStat@@YIXXZ 0x005701DA and ?CancelUndoStat@@YIXXZ 0x0056E974, and follows UndoBuyAbility
//   0x00576C4F in retail as in Ego_r.
// Callees: as UndoBuyAbility 0x00576C4F (CYesNo slots 142 / 143, CComponent slots 48 / 31 / 3, CUIScreenUnitConverter
//   0x0041CC14 / 0x0041CC2B, CManager::GetUIScale 0x0041CF47 / GetInstance 0x0041E5F2 with CObservable slots 5 / 9,
//   CObserver slots 3 / 6, CCharString 0x0099EBF0 / 0x0099EAE0, SaveExperienceValuesForCountDown 0x00570161,
//   GetTCHeroExperience 0x0056FF3D, SExpSpendUndo::RevertExperiencePoints 0x0056F4BA), plus the stat undo map's _M_find
//   0x00579114 / erase 0x00579D18 and CTCHeroExperience::ModifyTrainableStatLevel 0x006D6810.
// Layout: CTCInventoryExperience UndoableStatBuys +0x140, pYesNo +0x1C4, CreateBecauseOfUndo +0x214, DestroyEverything
//   +0x21C, PMousePointer +0x258 (engine/CTCInventoryExperience.h; local polymorphic model: the header flattens the vptr).
//   The pending stat / dialog / pointer globals 0x013B8AFC / 0x013B8B04 / 0x013B8B00 are file statics with no PDB name
//   (bound names kept, descriptive).
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
class CTCHeroExperience {
public:
    void ModifyTrainableStatLevel(EHeroTrainableStatType stat, long change);   // 0x006D6810
};
struct SExpSpendUndo {                                // Ego_r.pdb: Generals, Strengths, Skills, Wills (list<long>), nine longs
    std::list<long> Generals;                         // +0x00
    void RevertExperiencePoints(CTCHeroExperience* experience);   // 0x0056F4BA
};

extern EHeroTrainableStatType g_UndoStat;                            // 0x013B8AFC (file static, descriptive)
extern NUISystem::CYesNo* g_pUndoYesNoDialog;                        // 0x013B8B04 (file static, descriptive)
extern NUISystem::CComponent* g_pUndoMousePointer;                   // 0x013B8B00 (file static, descriptive)
void __fastcall EnsureUndoStat();                                    // 0x005701DA
void __fastcall CancelUndoStat();                                    // 0x0056E974

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
    void UndoBuyStat(EHeroTrainableStatType stat, bool confirmed);
private:
    CTCHeroExperience* GetTCHeroExperience();                            // 0x0056FF3D
};

void CTCInventoryExperience::UndoBuyStat(EHeroTrainableStatType stat, bool confirmed)
{
    if (!confirmed) {
        g_UndoStat = stat;
        pYesNo->SetYesCallBack(EnsureUndoStat);
        pYesNo->ChangeState(1);
        pYesNo->ChangePosition(C2DVector(NUISystem::CUIScreenUnitConverter::GetBackBufferWidth() * 0.5f - NUISystem::CManager::GetInstance()->GetUIScale().X * 104.0f,
                                         NUISystem::CUIScreenUnitConverter::GetBackBufferHeight() * 0.5f - NUISystem::CManager::GetInstance()->GetUIScale().Y * 52.0f), 0.0f, false);
        pYesNo->SetNoCallBack(CancelUndoStat);
        g_pUndoYesNoDialog = pYesNo;
        g_pUndoMousePointer = PMousePointer;
        NUISystem::CManager::GetInstance()->RemoveObserver(g_pUndoYesNoDialog);
        NUISystem::CManager::GetInstance()->AddConcurrentExclusiveObserver(g_pUndoYesNoDialog);
        g_pUndoYesNoDialog->ObserveAllEvents();
        NUISystem::CManager::GetInstance()->RemoveObserver(g_pUndoMousePointer);
        NUISystem::CManager::GetInstance()->AddConcurrentExclusiveObserver(g_pUndoMousePointer);
        g_pUndoMousePointer->ObserveEvent(NUISystem::EVENT_25);
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
        std::map<EHeroTrainableStatType, SExpSpendUndo>::iterator it = UndoableStatBuys.find(stat);
        GetTCHeroExperience()->ModifyTrainableStatLevel(stat, -1);
        it->second.RevertExperiencePoints(GetTCHeroExperience());
        if (it->second.Generals.size() == 0)
            UndoableStatBuys.erase(it);
        DestroyEverything = true;
    }
}
