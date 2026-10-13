// CAIStateGroup_MinionReactToAlert::IsRunnable() -- normalises Query_0090f110()'s byte result to a bool
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0090F1B0, oracle length 11 bytes.
// Grade: candidate -- still DIFFER(11v11) in the Linux lane (Wine + DDK 13.10.4035 cl.exe); generated for a
//        repeated body shape, see the batch README for the open question.
//
// Retail bytes:  call Query_0090f110 ; test al,al ; setne al ; ret
// Virtual predicate that forwards to the __thiscall helper @ 0x0090F110 on the same object
// (ecx passes through untouched) and returns its byte result as a normalised bool. Callee name
// kept from the manifest only when the label is unique. One of 6 bodies of this shape.
struct CAIStateGroup_MinionReactToAlert {
    virtual void v00();
    virtual bool IsRunnable();
    unsigned char Query_0090f110();                // 0x0090F110
};

bool CAIStateGroup_MinionReactToAlert::IsRunnable()
{
    return Query_0090f110() ? true : false;
}
