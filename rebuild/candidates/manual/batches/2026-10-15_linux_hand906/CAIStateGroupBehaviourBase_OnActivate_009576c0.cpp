// CAIStateGroupBehaviourBase::OnActivate() -- a behaviour state group starting up clears its timers and then runs the per-behaviour activation hook
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009576C0, length 13 bytes (vtable 0x0128C444 slot 2, also reached from the
// vtable at 0x0128D11C; the catalogue row 009576c0 also held the unreferenced predicate at 0x009576D0, split via the
// skipped-row ledger, chunk 906).
// Callees: the group's own virtual slot 16 (+0x40; the base implementation @ 0x009009CC is an empty `ret`).
// Shape: `xor eax,eax` once and three dword stores through it (+0x28, +0x30, +0x2c, in source order: the float members
// are assigned the integer literal 0, which shares the zero register), then `mov eax,[ecx] ; jmp [eax+40h]`.
// Model: local. Ego_r.pdb CAIStateGroupBehaviourBase (0x34 bytes): PTargetThing +0x14, PTCPerception +0x1c,
// PTCOpinion +0x20, PTCCreatureMode +0x24, TriggerTime +0x28, RandomWaitTime +0x2c, ThisSpeechDuration +0x30.
struct CAIStateGroupBehaviourBase {
    virtual void v00(); virtual void v01();
    virtual void OnActivate();                     // slot 2 (+0x08)
    virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual void v13(); virtual void v14(); virtual void v15();
    virtual void OnBehaviourActivated();           // slot 16 (+0x40): per-behaviour hook, empty in the base
    unsigned char _pad04[0x24];                    // +0x04 CAIStateGroupBase members, PTargetThing, the three component pointers
    float TriggerTime;                             // +0x28
    float RandomWaitTime;                          // +0x2c
    float ThisSpeechDuration;                      // +0x30
};

void CAIStateGroupBehaviourBase::OnActivate()
{
    TriggerTime = 0;
    ThisSpeechDuration = 0;
    RandomWaitTime = 0;
    OnBehaviourActivated();
}
