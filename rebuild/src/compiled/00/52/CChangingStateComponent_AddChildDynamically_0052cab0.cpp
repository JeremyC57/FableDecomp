// CChangingStateComponent::AddChildDynamically(CCountedPointer<CComponent>&) -- append the child to the
// component's child array (CComponent::AddChild) and, unless the child's type is 8, push the current
// TargetState into it through its virtual SetState (slot 48) -- a sibling jump that reuses the argument slot.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0052CAB0, length 56 bytes.
// Callees: CComponent::AddChild @ 0x00533CC0, child vtable slots 65 (+0x104) and 48 (+0xc0).
// NUISystem::CChangingStateComponent (retail layout). The Ego_r PDB gives the class 0x18c bytes with
// named members from +0x15c; retail containers are smaller (CComponent is 0x134 here, the StatesToDo
// list a 4-byte STLport head), so the named tail sits 0x28 lower: StatesBeingDone +0x134, StatesDone
// +0x138, StatesToDo +0x13c, ParentUpdateTime +0x140, CurrentState +0x144, TargetState +0x148,
// RequestedState +0x14c, UpdateTime +0x150, PreviousUpdateChanged +0x154, PreviousState +0x158.
// The CComponent vtable slots used by this class: 48 (+0xc0) SetState(state), 49 (+0xc4) IsChanged(),
// 65 (+0x104) GetComponentType(), 134 (+0x218) GetStateInfo(state), 135 (+0x21c) OnStateChanged(state).
struct SStateInfo {
    unsigned char Flags;                        // +0 bit 5 (0x20) / bit 6 (0x40) tested by the predicates
};
struct CComponent;
struct CCountedComponentPointer {               // CCountedPointer<NUISystem::CComponent>
    CComponent* Object;                         // +0
    void*       Info;                           // +4
};
struct CComponent {
    virtual void VSlot0();
    virtual void VSlot1();
    virtual void VSlot2();
    virtual void VSlot3();
    virtual void VSlot4();
    virtual void VSlot5();
    virtual void VSlot6();
    virtual void VSlot7();
    virtual void VSlot8();
    virtual void VSlot9();
    virtual void VSlot10();
    virtual void VSlot11();
    virtual void VSlot12();
    virtual void VSlot13();
    virtual void VSlot14();
    virtual void VSlot15();
    virtual void VSlot16();
    virtual void VSlot17();
    virtual void VSlot18();
    virtual void VSlot19();
    virtual void VSlot20();
    virtual void VSlot21();
    virtual void VSlot22();
    virtual void VSlot23();
    virtual void VSlot24();
    virtual void VSlot25();
    virtual void VSlot26();
    virtual void VSlot27();
    virtual void VSlot28();
    virtual void VSlot29();
    virtual void VSlot30();
    virtual void VSlot31();
    virtual void VSlot32();
    virtual void VSlot33();
    virtual void VSlot34();
    virtual void VSlot35();
    virtual void VSlot36();
    virtual void VSlot37();
    virtual void VSlot38();
    virtual void VSlot39();
    virtual void VSlot40();
    virtual void VSlot41();
    virtual void VSlot42();
    virtual void VSlot43();
    virtual void VSlot44();
    virtual void VSlot45();
    virtual void VSlot46();
    virtual void VSlot47();
    virtual void SetState(unsigned long state);               // slot 48  (+0xc0)
    virtual bool IsChanged();                                 // slot 49  (+0xc4)
    virtual void VSlot50();
    virtual void VSlot51();
    virtual void VSlot52();
    virtual void VSlot53();
    virtual void VSlot54();
    virtual void VSlot55();
    virtual void VSlot56();
    virtual void VSlot57();
    virtual void VSlot58();
    virtual void VSlot59();
    virtual void VSlot60();
    virtual void VSlot61();
    virtual void VSlot62();
    virtual void VSlot63();
    virtual void VSlot64();
    virtual long GetComponentType();                          // slot 65  (+0x104)
    virtual void VSlot66();
    virtual void VSlot67();
    virtual void VSlot68();
    virtual void VSlot69();
    virtual void VSlot70();
    virtual void VSlot71();
    virtual void VSlot72();
    virtual void VSlot73();
    virtual void VSlot74();
    virtual void VSlot75();
    virtual void VSlot76();
    virtual void VSlot77();
    virtual void VSlot78();
    virtual void VSlot79();
    virtual void VSlot80();
    virtual void VSlot81();
    virtual void VSlot82();
    virtual void VSlot83();
    virtual void VSlot84();
    virtual void VSlot85();
    virtual void VSlot86();
    virtual void VSlot87();
    virtual void VSlot88();
    virtual void VSlot89();
    virtual void VSlot90();
    virtual void VSlot91();
    virtual void VSlot92();
    virtual void VSlot93();
    virtual void VSlot94();
    virtual void VSlot95();
    virtual void VSlot96();
    virtual void VSlot97();
    virtual void VSlot98();
    virtual void VSlot99();
    virtual void VSlot100();
    virtual void VSlot101();
    virtual void VSlot102();
    virtual void VSlot103();
    virtual void VSlot104();
    virtual void VSlot105();
    virtual void VSlot106();
    virtual void VSlot107();
    virtual void VSlot108();
    virtual void VSlot109();
    virtual void VSlot110();
    virtual void VSlot111();
    virtual void VSlot112();
    virtual void VSlot113();
    virtual void VSlot114();
    virtual void VSlot115();
    virtual void VSlot116();
    virtual void VSlot117();
    virtual void VSlot118();
    virtual void VSlot119();
    virtual void VSlot120();
    virtual void VSlot121();
    virtual void VSlot122();
    virtual void VSlot123();
    virtual void VSlot124();
    virtual void VSlot125();
    virtual void VSlot126();
    virtual void VSlot127();
    virtual void VSlot128();
    virtual void VSlot129();
    virtual void VSlot130();
    virtual void VSlot131();
    virtual void VSlot132();
    virtual void VSlot133();
    virtual SStateInfo* GetStateInfo(unsigned long state);    // slot 134 (+0x218)
    virtual void OnStateChanged(unsigned long state);         // slot 135 (+0x21c)
    unsigned char _pad_0x04[0x94];              // +0x004
    float m_timer0;                             // +0x098  three (progress, end) float pairs compared by
    float m_end0;                               // +0x09c  InternalChanged
    float m_timer1;                             // +0x0a0
    float m_end1;                               // +0x0a4
    float m_timer2;                             // +0x0a8
    float m_end2;                               // +0x0ac
    unsigned char _pad_0xb0[0x84];              // +0x0b0 .. +0x134 (the child array lives at +0xb0)
    void Initialise();                          // 0x005339B0
    void AddChild(CCountedComponentPointer& child); // 0x00533CC0 (manifest: Vector_PushBack_Or_Reallocate)
};
struct CChangingStateComponent : CComponent {
    long          StatesBeingDone;              // +0x134 (EChangingStateComponentState)
    long          StatesDone;                   // +0x138
    void*         StatesToDo;                   // +0x13c (retail 4-byte list head)
    float         ParentUpdateTime;             // +0x140
    unsigned long CurrentState;                 // +0x144
    unsigned long TargetState;                  // +0x148
    unsigned long RequestedState;               // +0x14c
    float         UpdateTime;                   // +0x150
    bool          PreviousUpdateChanged;        // +0x154
    unsigned long PreviousState;                // +0x158
    bool IsIndependentByDefault();              // 0x0052F1A0 (a thunk to vtable slot 101)
    bool IsIndependentByDefault40();            // 0x0052F1B0 (a thunk to vtable slot 101)
    void AddChildDynamically(CCountedComponentPointer& child);
};

void CChangingStateComponent::AddChildDynamically(CCountedComponentPointer& child)
{
    AddChild(child);
    if (child.Object->GetComponentType() != 8)
        child.Object->SetState(TargetState);
}