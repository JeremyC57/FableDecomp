// CAIStateGroup_SitForMeal::IsStateGroupStillValid() -- normalises Query_009455b0()'s byte result to a bool
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00945610, oracle length 11 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  call Query_009455b0 ; test al,al ; setne al ; ret
// Virtual predicate that forwards to the __thiscall helper @ 0x009455B0 on the same object
// (ecx passes through untouched) and returns its byte result as a normalised bool. Callee name
// kept from the manifest only when the label is unique. One of 6 bodies of this shape.
struct CAIStateGroup_SitForMeal {
    virtual void v00();
    virtual bool IsStateGroupStillValid();
    unsigned char Query_009455b0();                // 0x009455B0
};

bool CAIStateGroup_SitForMeal::IsStateGroupStillValid()
{
    return Query_009455b0() ? true : false;
}
