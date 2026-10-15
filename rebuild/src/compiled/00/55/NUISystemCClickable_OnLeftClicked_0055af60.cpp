// NUISystem::CClickable::OnLeftClicked() -- remembers the current state, switches to the def's left-clicked state, runs the left-click actions and starts observing event 0x1c
// Leaf: OnLeftClicked
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055AF60, 107 bytes of code.
// Callees: the component's virtual GetDef (slot 108, +0x1b0: a CDefPointer returned through the hidden pointer into the
//          `push ecx` frame word), ChangeState (slot 48, +0xc0), ExecuteActions (slot 131, +0x20c), and the CObserver
//          base's ObserveEvent (slot 3 on the +4 subobject); the def's virtual Destroy (slot 1) when the temporary
//          drops the last reference.
// Shape: `ChangeState(GetDef()->LeftClickedState)` reads the def through the call's returned pointer (`mov eax,[eax]`)
//          and the temporary dies at the end of the statement: the landed CDefPointer destructor shape (0x0083E0D0:
//          DecRef + GetRefCount through the count base at +4, the def kept in ecx for the virtual call).
// Retail NUISystem component model of the landed CClickable::OnDeactivate (0x0055AF30): Ego_r.pdb minus 0x30; the
// current state is the CChangingStateComponent word at +0x148.
struct CRefCounted {                       // second base of a def: the count sits at +4 behind the vptr
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};

struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                // slot 1
};

struct CClickableDef : CDefVirtual, CRefCounted {
    unsigned char _pad08[0x20c - 8];
    long LeftClickedState;                 // +0x20c
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};

struct CDefPointer {                       // CDefPointer<const CClickableDef>
    CClickableDef* m_p;
    CDefPointer() { }
    ~CDefPointer()
    {
        if (m_p)
            m_p->ReleaseTemporary();
    }
    CClickableDef* operator->() const { return m_p; }
};

struct CComponentInterface {                                 // primary vtable (CClickable: 0x0124BD2C, CDraggable: 0x0124BA94)
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void v003();
    virtual void v004();
    virtual void v005();
    virtual void v006();
    virtual void v007();
    virtual void v008();
    virtual void v009();
    virtual void v010();
    virtual void v011();
    virtual void v012();
    virtual void v013();
    virtual void v014();
    virtual void v015();
    virtual void v016();
    virtual void v017();
    virtual void v018();
    virtual void v019();
    virtual void v020();
    virtual void v021();
    virtual void v022();
    virtual void v023();
    virtual void v024();
    virtual void v025();
    virtual void v026();
    virtual void v027();
    virtual void v028();
    virtual void v029();
    virtual void v030();
    virtual void v031();
    virtual void v032();
    virtual void v033();
    virtual void v034();
    virtual void v035();
    virtual void v036();
    virtual void v037();
    virtual void v038();
    virtual void v039();
    virtual void v040();
    virtual void v041();
    virtual void v042();
    virtual void v043();
    virtual void v044();
    virtual void v045();
    virtual void v046();
    virtual void v047();
    virtual void ChangeState(long state);                  // slot 48 (+0xc0): CChangingStateComponent::ChangeState @ 0x0052CF40
    virtual void v049();
    virtual void v050();
    virtual void v051();
    virtual void v052();
    virtual void v053();
    virtual void v054();
    virtual void v055();
    virtual void v056();
    virtual void v057();
    virtual void v058();
    virtual void v059();
    virtual void v060();
    virtual void v061();
    virtual void v062();
    virtual void v063();
    virtual void v064();
    virtual void v065();
    virtual void v066();
    virtual void v067();
    virtual void v068();
    virtual void v069();
    virtual void v070();
    virtual void v071();
    virtual void v072();
    virtual void v073();
    virtual void v074();
    virtual void v075();
    virtual void v076();
    virtual void v077();
    virtual void v078();
    virtual void v079();
    virtual void v080();
    virtual void v081();
    virtual void v082();
    virtual void v083();
    virtual void v084();
    virtual void v085();
    virtual void v086();
    virtual void v087();
    virtual void v088();
    virtual void v089();
    virtual void v090();
    virtual void v091();
    virtual void v092();
    virtual void v093();
    virtual void v094();
    virtual void v095();
    virtual void v096();
    virtual void v097();
    virtual void v098();
    virtual void v099();
    virtual void v100();
    virtual void v101();
    virtual void v102();
    virtual void v103();
    virtual void v104();
    virtual void v105();
    virtual void v106();
    virtual void v107();
    virtual CDefPointer GetDef();                         // slot 108 (+0x1b0)
    virtual void v109();
    virtual void v110();
    virtual void v111();
    virtual void v112();
    virtual void v113();
    virtual void v114();
    virtual void v115();
    virtual void v116();
    virtual void v117();
    virtual void v118();
    virtual void v119();
    virtual void v120();
    virtual void v121();
    virtual void v122();
    virtual void v123();
    virtual void v124();
    virtual void v125();
    virtual void v126();
    virtual void v127();
    virtual void v128();
    virtual void v129();
    virtual void v130();
    virtual void ExecuteActions(void* actions);           // slot 131 (+0x20c): CComponent::ExecuteActions @ 0x005309B0
    virtual void v132();
    virtual void v133();
};

struct CObserver_Retail {                                    // vtable at +4 (CClickable: 0x0124BD08, CDraggable: 0x0124BA70)
    virtual void v0();
    virtual void ProcessEvent();                             // slot 1: CClickable::ProcessEvent @ 0x0055AD60
    virtual void v2();
    virtual void ObserveEvent(long event);                   // slot 3: CObserver::ObserveEvent @ 0x0052DA20
    virtual void StopObservingEvent(long event);             // slot 4 (@ 0x0055BD50, label propagated)
    virtual void v5();
    unsigned char _body[0x10];                               // EventsToObserve (12-byte set), PreventObservation
};

struct CEventSinkInterface {                                 // vtable at +0x18 (CClickable: 0x0124BD00, CDraggable: 0x0124BA68)
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
};

struct CActionList;                                          // std::list<CCountedPointer<NUISystem::CAction>>

struct CHoverable_Retail : CComponentInterface, CObserver_Retail, CEventSinkInterface {
    unsigned char _component[0x148 - 0x1c];                  // CComponent .. CChangingStateComponent
    long         CurrentState;                               // +0x148
    unsigned char _component2[0x15c - 0x14c];
    long         StateBeforeHovered;                         // +0x15c
    bool         WasHovered;                                 // +0x160
    CActionList* PActionsOnHovered;                          // +0x164
    CActionList* PActionsOnUnhovered;                        // +0x168  (size 0x16c)
    CHoverable_Retail(long owner);                           // 0x0055BA20
    void OnHovered();                                        // 0x0055BAE0
    void OnUnhovered();                                      // 0x0055B9A0
    void OnDeactivate();                                     // 0x0055BDE0
};

struct CClickable_Retail : CHoverable_Retail {               // retail 0x194 bytes (PDB 0x1c4)
    long         StateBeforeLeftClicked;                     // +0x16c
    bool         WasLeftClicked;                             // +0x170
    CActionList* PActionsOnLeftClicked;                      // +0x174
    CActionList* PActionsOnLeftHeld;                         // +0x178
    CActionList* PActionsOnLeftUnclicked;                    // +0x17c
    long         StateBeforeRightClicked;                    // +0x180
    bool         WasRightClicked;                            // +0x184
    CActionList* PActionsOnRightClicked;                     // +0x188
    float        InputDelay;                                 // +0x18c (set by InitialiseInputDelay)
    float        LastInputTime;                              // +0x190
    CClickable_Retail(long owner);                           // 0x0055B460
    void InitialiseInputDelay();                             // 0x0055B040 (label "SetupRumbleAction_Strong" is an auto-name)
    void OnLeftUnclicked();                                  // 0x0055ACF0
    void OnHovered();                                        // 0x0055AEB0
    void OnUnhovered();                                      // 0x0055AEF0
    void OnDeactivate();                                     // 0x0055AF30
    void OnLeftClicked();
};

void CClickable_Retail::OnLeftClicked()
{
    StateBeforeLeftClicked = CurrentState;
    ChangeState(GetDef()->LeftClickedState);
    ExecuteActions(PActionsOnLeftClicked);
    ObserveEvent(0x1c);
}