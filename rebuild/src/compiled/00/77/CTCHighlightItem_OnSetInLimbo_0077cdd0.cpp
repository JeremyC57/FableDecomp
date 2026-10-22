// CTCHighlightItem::OnSetInLimbo(bool) -- calls the member method @ 0x0077CCA0 when the flag argument is set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077CDD0, oracle length 16 bytes.
// Grade: candidate -- generated for a repeated body shape and graded in the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); the batch README records the lane verdict.
//
// Retail bytes:  mov al, byte ptr [esp + 4] ; test al, al ; je 0x77cddd ; call 0x77cca0 ; ret 4
// Tiny-body shape `UpdateIf` synthesised from the instruction pattern (the class comes from the
// manifest label when it is a plain class name, else the method is landed under a descriptive
// placeholder); member names are descriptive. 
struct CTCHighlightItem {
    void Update();                         // 0x0077CCA0
    void OnSetInLimbo(bool enable);
};

void CTCHighlightItem::OnSetInLimbo(bool enable)
{
    if (enable)
        Update();
}