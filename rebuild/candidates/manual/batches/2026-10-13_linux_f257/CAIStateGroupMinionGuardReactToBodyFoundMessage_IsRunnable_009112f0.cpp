// CAIStateGroup_MinionGuardReactToBodyFoundMessage::IsRunnable() -- normalises FindBodyFoundEvent()'s byte result to a bool
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009112F0, oracle length 11 bytes.
// Grade: candidate -- still DIFFER(11v11) in the Linux lane (Wine + DDK 13.10.4035 cl.exe); generated for a
//        repeated body shape, see the batch README for the open question.
//
// Retail bytes:  call FindBodyFoundEvent ; test al,al ; setne al ; ret
// Virtual predicate that forwards to the __thiscall helper @ 0x009112A0 on the same object
// (ecx passes through untouched) and returns its byte result as a normalised bool. Callee name
// kept from the manifest only when the label is unique. One of 6 bodies of this shape.
struct CAIStateGroup_MinionGuardReactToBodyFoundMessage {
    virtual void v00();
    virtual bool IsRunnable();
    unsigned char FindBodyFoundEvent();                // 0x009112A0
};

bool CAIStateGroup_MinionGuardReactToBodyFoundMessage::IsRunnable()
{
    return FindBodyFoundEvent() ? true : false;
}
