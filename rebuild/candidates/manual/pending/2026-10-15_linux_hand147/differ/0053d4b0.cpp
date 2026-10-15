// NUISystem::CMenuEntry::Update(float) -- per-frame update: full clickable update while the owning menu accepts input, else only the state animation; fires the on-visible actions when the entry just became visible
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0053D4B0, 144 bytes (padding included).
// Grade: DIFFER(135v139) -- two things: (1) frame: retail keeps the counted-pointer temporary at [esp+8] and the wasVisible byte at [esp+0Fh], ours swaps them ([esp+0Ch] / [esp+0Bh]); (2) the inline release: retail `mov edx,[eax+4] ; mov ecx,eax ; add eax,4 ; dec edx ; test edx,edx ; mov [eax],edx`, ours `dec [eax+4] ; jne` (plain --count), `mov [eax+4],edx` (split statements) or a re-read test (volatile count). Same release shape as the unsolved asm-baked 0x0041AB00 / 0x0055AF60 / 0x00666D10. Base-class fix that DID work: the vptr belongs to CChangingStateComponent so the qualified base Updates take ecx=this
// Shape: the visibility byte (+0x97) is sampled into a frame byte first; the owning menu comes back
// as an intrusive counted pointer by value from vtable slot 108 (+0x1b0), its input flag (+0x221) is
// read straight off the temporary, then the temporary is released inline (count at +4, slot 1 destroys
// the object at zero). Flag set: CClickable::Update (0x0055ACB0); clear: CChangingStateComponent::Update
// (0x0052C7E0), both qualified calls on `this`. Finally, visible now but not before: vtable slot 131
// (+0x20c) runs PActionsOnVisible (+0x1b8, the member list the ctor 0x0053DFE0 zeroes).
struct CActionList;

namespace NUISystem {

struct CMenu {
    virtual void v00();
    virtual void Destroy();                         // slot 1
    long          m_refCount;                       // +0x04
    unsigned char _pad08[0x221 - 8];
    bool          m_acceptsInput;                   // +0x221
};

struct CMenuPointer {                               // intrusive counted pointer
    CMenu* m_p;
    ~CMenuPointer()
    {
        CMenu* p = m_p;
        if (p) {
            long* count = &p->m_refCount;
            long n = *count - 1;
            *count = n;
            if (n == 0)
                p->Destroy();
        }
    }
    CMenu* operator->() const { return m_p; }
};

struct CChangingStateComponent {                    // polymorphic root: its vptr is at +0 of the menu entry
    virtual void v000();
    // vtable slots 1..107 and 109..130 are not used here
    virtual void s001(); virtual void s002(); virtual void s003(); virtual void s004(); virtual void s005(); virtual void s006(); virtual void s007(); virtual void s008(); virtual void s009(); virtual void s010();
    virtual void s011(); virtual void s012(); virtual void s013(); virtual void s014(); virtual void s015(); virtual void s016(); virtual void s017(); virtual void s018(); virtual void s019(); virtual void s020();
    virtual void s021(); virtual void s022(); virtual void s023(); virtual void s024(); virtual void s025(); virtual void s026(); virtual void s027(); virtual void s028(); virtual void s029(); virtual void s030();
    virtual void s031(); virtual void s032(); virtual void s033(); virtual void s034(); virtual void s035(); virtual void s036(); virtual void s037(); virtual void s038(); virtual void s039(); virtual void s040();
    virtual void s041(); virtual void s042(); virtual void s043(); virtual void s044(); virtual void s045(); virtual void s046(); virtual void s047(); virtual void s048(); virtual void s049(); virtual void s050();
    virtual void s051(); virtual void s052(); virtual void s053(); virtual void s054(); virtual void s055(); virtual void s056(); virtual void s057(); virtual void s058(); virtual void s059(); virtual void s060();
    virtual void s061(); virtual void s062(); virtual void s063(); virtual void s064(); virtual void s065(); virtual void s066(); virtual void s067(); virtual void s068(); virtual void s069(); virtual void s070();
    virtual void s071(); virtual void s072(); virtual void s073(); virtual void s074(); virtual void s075(); virtual void s076(); virtual void s077(); virtual void s078(); virtual void s079(); virtual void s080();
    virtual void s081(); virtual void s082(); virtual void s083(); virtual void s084(); virtual void s085(); virtual void s086(); virtual void s087(); virtual void s088(); virtual void s089(); virtual void s090();
    virtual void s091(); virtual void s092(); virtual void s093(); virtual void s094(); virtual void s095(); virtual void s096(); virtual void s097(); virtual void s098(); virtual void s099(); virtual void s100();
    virtual void s101(); virtual void s102(); virtual void s103(); virtual void s104(); virtual void s105(); virtual void s106(); virtual void s107();
    virtual CMenuPointer GetParentMenu();           // slot 108 (+0x1b0)
    virtual void s109(); virtual void s110();
    virtual void s111(); virtual void s112(); virtual void s113(); virtual void s114(); virtual void s115(); virtual void s116(); virtual void s117(); virtual void s118(); virtual void s119(); virtual void s120();
    virtual void s121(); virtual void s122(); virtual void s123(); virtual void s124(); virtual void s125(); virtual void s126(); virtual void s127(); virtual void s128(); virtual void s129(); virtual void s130();
    virtual void PerformActions(CActionList* actions);   // slot 131 (+0x20c)
    void Update(float dt);                          // 0x0052C7E0
};

struct CClickable : CChangingStateComponent {
    void Update(float dt);                          // 0x0055ACB0
};

struct CMenuEntry : CClickable {
    unsigned char _pad04[0x97 - 4];
    bool          m_visible;                        // +0x97
    unsigned char _pad98[0x1b8 - 0x98];
    CActionList*  PActionsOnVisible;                // +0x1b8
    void Update(float dt);
};

void CMenuEntry::Update(float dt)
{
    bool wasVisible = m_visible;
    bool acceptsInput = GetParentMenu()->m_acceptsInput;
    if (acceptsInput)
        CClickable::Update(dt);
    else
        CChangingStateComponent::Update(dt);
    if (m_visible && !wasVisible)
        PerformActions(PActionsOnVisible);
}

}
