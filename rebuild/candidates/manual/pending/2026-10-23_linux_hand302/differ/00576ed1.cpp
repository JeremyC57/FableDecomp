// CTCInventoryExperience::UndoBuyStat(EHeroTrainableStatType stat, bool confirmed) -- asks (yes/no dialog) whether to undo a stat purchase; once confirmed, reverts the stat level and its spent experience and drops the undo record
// Grade: DIFFER(622v622) -- the SetPosition push ecx vs fsubp swap plus one load-order swap (static dialog read vs manager vtable) at the first AddObserver
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00576ED1, 622 bytes, `ret 8`; frame-pointer size TU
// (`optimize("y", off)` + `("s", on)`, as the landed AddSoundAction 0x0056F615 of the same TU).
// Callees: the yes/no dialog's slots 142/143 (+0x238/+0x23C, OK and cancel callbacks 0x005701DA / 0x0056E974), 48 (+0xC0),
// 31 (+0x7C, SetPosition) and 3 (+0x0C, FindChild by value); screen size getters 0x0041CC14/0x0041CC2B,
// CManager::GetUIScale 0x0041CF47, CFrontEndManager::GetInstance 0x0041E5F2 (manager slots 5 and 9), the observer slots
// 3 and 6; SaveExperienceValuesForCountDown 0x00570161, the undo map's find 0x00579114 and erase 0x00579D18,
// GetTCHeroExperience 0x0056FF3D, CTCHeroExperience::ModifyTrainableStatLevel 0x006D6810,
// SExpSpendUndo::RevertExperiencePoints 0x0056F4BA.
// Layout: retail CTCInventoryExperience (landed ctor 0x0057832D): UndoableStatBuys +0x140, pYesNo +0x1C4,
// CreateBecauseOfUndo +0x214, DestroyEverything +0x21C, PMousePointer +0x258. The pending stat / dialog / pointer
// statics live at 0x013B8AFC / 0x013B8B04 / 0x013B8B00.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    CCharString(const CCharString& other);
    ~CCharString();                                   // 0x0099EAE0
};
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) { }
};
float GetScreenWidth();                               // 0x0041CC14
float GetScreenHeight();                              // 0x0041CC2B

namespace NUISystem {
struct CObserver {                                    // the observer interface at +4 of every component
    virtual void o00(); virtual void o01(); virtual void o02();
    virtual void ObserveEvent(long eventType);        // slot 3 (+0x0C)
    virtual void o04(); virtual void o05();
    virtual void TakeFocus_slot6();                   // slot 6 (+0x18)
};
typedef void (__fastcall* TCallback)();
struct CComponentBase {
    virtual void v000(); virtual void v001(); virtual void v002();
    virtual struct CComponent* FindChild(CCharString name);    // slot 3 (+0xC)
    virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021();
    virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030();
    virtual void SetPosition(const C2DVector& pos, float time, long mode); // slot 31 (+0x7C)
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037();
    virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void SetActive(bool active);                       // slot 48 (+0xC0)
    virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054();
    virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060();
    virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066();
    virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072();
    virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078();
    virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084();
    virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089(); virtual void v090();
    virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096();
    virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102();
    virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108();
    virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114();
    virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120();
    virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126();
    virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132();
    virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138();
    virtual void v139(); virtual void v140(); virtual void v141();
    virtual void SetOkCallback(TCallback callback);            // slot 142 (+0x238)
    virtual void SetCancelCallback(TCallback callback);        // slot 143 (+0x23C)

};
struct CComponent : CComponentBase, CObserver {
};
struct CYesNo : CComponent {
};
struct CManager {
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03(); virtual void m04();
    virtual void AddObserver(CObserver* observer);    // slot 5 (+0x14)
    virtual void m06(); virtual void m07(); virtual void m08();
    virtual void SetFocus(CObserver* observer);       // slot 9 (+0x24)
    C2DVector GetUIScale();                           // 0x0041CF47
    static CManager* __fastcall GetInstance();        // 0x0041E5F2
};
}

struct SExpSpendUndo;
struct CTCHeroExperience {
    void ModifyTrainableStatLevel(long stat, long change);   // 0x006D6810
};
struct SExpSpendUndoNode;
struct SExpSpendUndoList {                            // list<...> (one word)
    SExpSpendUndoNode* _M_node;
    void RevertExperiencePoints(CTCHeroExperience* experience);   // 0x0056F4BA (SExpSpendUndo)
    unsigned long size() const;
};
struct SExpSpendUndoNode {
    SExpSpendUndoNode* _M_next;
    SExpSpendUndoNode* _M_prev;
};
inline unsigned long SExpSpendUndoList::size() const
{
    unsigned long n = 0;
    for (SExpSpendUndoNode* p = _M_node->_M_next; p != _M_node; p = p->_M_next)
        ++n;
    return n;
}
struct SStatUndoNode {                                // map<EHeroTrainableStatType, SExpSpendUndo> node
    void*             _M_header[4];
    long              Key;                            // +0x10
    SExpSpendUndoList Undo;                           // +0x14
};
struct SStatUndoIterator {
    SStatUndoNode* _M_node;
    SStatUndoIterator(SStatUndoNode* node) : _M_node(node) { }
    SStatUndoIterator(const SStatUndoIterator& o) : _M_node(o._M_node) { }
};
struct CStatUndoMap {                                 // 12-byte STLport map
    void* _M_header;
    void* _M_count;
    void* _M_compare;
    SStatUndoNode* _M_find(const long& key);          // 0x00579114
    SStatUndoIterator find(const long& key) { return SStatUndoIterator(_M_find(key)); }
    void erase(SStatUndoIterator it);                 // 0x00579D18
};

struct CTCInventoryExperience {
    void*                 __vftable;                  // +0x00
    unsigned char         _pad_0x04[0x13C];
    CStatUndoMap          UndoableStatBuys;           // +0x140
    unsigned char         _pad_0x14c[0x78];
    NUISystem::CYesNo*    pYesNo;                     // +0x1C4
    unsigned char         _pad_0x1c8[0x4C];
    bool                  CreateBecauseOfUndo;        // +0x214
    unsigned char         _pad_0x215[0x07];
    bool                  DestroyEverything;          // +0x21C
    unsigned char         _pad_0x21d[0x3B];
    NUISystem::CComponent* PMousePointer;             // +0x258
    static long                   s_UndoStat;         // 0x013B8AFC
    static NUISystem::CComponent* s_pUndoMousePointer;   // 0x013B8B00
    static NUISystem::CComponent* s_pUndoDialog;      // 0x013B8B04
    static void __fastcall ConfirmUndoStat();         // 0x005701DA
    static void __fastcall CancelUndo();              // 0x0056E974
    void SaveExperienceValuesForCountDown();          // 0x00570161
    CTCHeroExperience* GetTCHeroExperience();         // 0x0056FF3D
    void UndoBuyStat(long stat, bool confirmed);
};

void CTCInventoryExperience::UndoBuyStat(long stat, bool confirmed)
{
    if (!confirmed) {
        s_UndoStat = stat;
        pYesNo->SetOkCallback(ConfirmUndoStat);
        pYesNo->SetActive(true);
        pYesNo->SetPosition(C2DVector(GetScreenWidth() * 0.5f - NUISystem::CManager::GetInstance()->GetUIScale().X * 104.0f,
                                      GetScreenHeight() * 0.5f - NUISystem::CManager::GetInstance()->GetUIScale().Y * 52.0f), 0.0f, 0);
        pYesNo->SetCancelCallback(CancelUndo);
        s_pUndoDialog = pYesNo;
        s_pUndoMousePointer = PMousePointer;
        NUISystem::CManager::GetInstance()->AddObserver(s_pUndoDialog);
        NUISystem::CManager::GetInstance()->SetFocus(s_pUndoDialog);
        s_pUndoDialog->TakeFocus_slot6();
        NUISystem::CManager::GetInstance()->AddObserver(s_pUndoMousePointer);
        NUISystem::CManager::GetInstance()->SetFocus(s_pUndoMousePointer);
        s_pUndoMousePointer->ObserveEvent(0x19);
        if (NUISystem::CComponent* yes = pYesNo->FindChild(CCharString("PC_YESNO_GAME_YESBUTTON", -1))) {
            NUISystem::CManager::GetInstance()->SetFocus(yes);
            yes->ObserveEvent(0x19);
        }
        if (NUISystem::CComponent* no = pYesNo->FindChild(CCharString("PC_YESNO_GAME_NOBUTTON", -1))) {
            NUISystem::CManager::GetInstance()->SetFocus(no);
            no->ObserveEvent(0x19);
        }
    } else {
        CreateBecauseOfUndo = true;
        SaveExperienceValuesForCountDown();
        SStatUndoIterator it = UndoableStatBuys.find(stat);
        GetTCHeroExperience()->ModifyTrainableStatLevel(stat, -1);
        it._M_node->Undo.RevertExperiencePoints(GetTCHeroExperience());
        if (it._M_node->Undo.size() == 0)
            UndoableStatBuys.erase(it);
        DestroyEverything = true;
    }
}
