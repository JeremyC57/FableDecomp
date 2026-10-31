// Grade: DIFFER(346v365) (sec1b 2026-10-31) -- retail calls GetUIScale and GetSize twice: an unused scale/size pair first, then scale.Y copied to a float slot and a second GetSize (108->79 difflines); left: ours caches the vptr in edi across GetStyleDef (retail reloads, esi only), PScrollBar/cursor register swap, x87 operand order scale.Y*size.Y*barSize.Y and frame 0x3c vs 0x40 (tried operand reorder, optimize s)
// CScrollBarOutside::OnLeftClicked() -- page the scrolled list by the click side of the thumb
struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector operator*(const C2DVector& v) const { return C2DVector(X * v.X, Y * v.Y); }
};
struct CIVCountedPointeeBase {
    virtual void Destroy(int flags);               // slot 0
    virtual void Delete();                         // slot 1
    long IVRefCount;                               // +0x04
};
struct CStyleDef : CIVCountedPointeeBase {
    unsigned char _pad08[0x20c - 0x08];
    long          ClickSound;                      // +0x20c
};
struct CStyleDefPointer {
    CStyleDef* Object;
    ~CStyleDefPointer()
    {
        CIVCountedPointeeBase* old = Object;
        if (old && --old->IVRefCount == 0)
            old->Delete();
    }
};
struct CManager {
    C2DVector GetUIScale();                        // 0x0041CF47
};
struct CMouseCursor {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual C2DVector GetPosition();               // slot 16 (+0x40)
};
struct CFrontEndManager {
    static CManager* __fastcall GetInstance();     // 0x0041E5F2
};
struct CFrontEndManagerData { unsigned char _pad[0xb8]; CMouseCursor* Cursor; };   // +0xb8
struct CActionList;
struct CObserver {
    virtual void a00(); virtual void a01(); virtual void a02();
    virtual void ObserveEvent(long eventType);     // slot 3 (+0x0c)
    unsigned char _pad04[0x10];
};
struct CComponent {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022();
    virtual C2DVector GetSize();                   // slot 23 (+0x5c)
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void PlaySound(long sound);            // slot 48 (+0xc0)
    virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056();
    virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072();
    virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080();
    virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088();
    virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096();
    virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104();
    virtual void v105(); virtual void v106(); virtual void v107();
    virtual CStyleDefPointer GetStyleDef();        // slot 108 (+0x1b0)
    virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116();
    virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121();
    virtual C2DVector GetRenderPosition();         // slot 122 (+0x1e8)
    virtual C2DVector GetRenderSize();             // slot 123 (+0x1ec)
    virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130();
    virtual void RunActions(CActionList* actions); // slot 131 (+0x20c)
};
struct CScrollBarOutside : CComponent, CObserver {
    unsigned char _pad18[0x148 - 0x18];
    long          State;                           // +0x148
    unsigned char _pad14c[0x16c - 0x14c];
    long          ClickState;                      // +0x16c
    unsigned char _pad170[0x194 - 0x170];
    CComponent*   PScrollBar;                      // +0x194
    CActionList*  PActionsOnLeftClickedAbove;      // +0x198
    CActionList*  PActionsOnLeftClickedUnder;      // +0x19c
    void OnLeftClicked();
};

void CScrollBarOutside::OnLeftClicked()
{
    ClickState = State;
    PlaySound(GetStyleDef().Object->ClickSound);
    CMouseCursor* cursor = ((CFrontEndManagerData*)CFrontEndManager::GetInstance())->Cursor;
    if (PScrollBar && cursor) {
        C2DVector mouse = cursor->GetPosition();
        C2DVector barPos = PScrollBar->GetRenderPosition();
        C2DVector barSize = PScrollBar->GetRenderSize();
        long top = (long)barPos.Y;
        C2DVector uiScale = CFrontEndManager::GetInstance()->GetUIScale();   // unused (calls kept)
        C2DVector size = PScrollBar->GetSize();                             // unused
        float scaleY = CFrontEndManager::GetInstance()->GetUIScale().Y;
        long bottom = (long)(scaleY * PScrollBar->GetSize().Y * barSize.Y + barPos.Y);
        if (bottom < mouse.Y) {
            RunActions(PActionsOnLeftClickedUnder);
            ObserveEvent(0x1c);
            return;
        }
        if (top > mouse.Y)
            RunActions(PActionsOnLeftClickedAbove);
    }
    ObserveEvent(0x1c);
}
