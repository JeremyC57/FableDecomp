// CTCPerceiveThing::vfunc_8() -- constant-true virtual (slot 8 of the CTCPerceiveThing vtable)
// Retail: Fable.exe (TLC, VC7.1) @ 0x007460E0, 3 bytes. Vtable 0x012688CC slot 8 (rtti-vtables.tsv); the catalogue
// row 007460d0 held this body too (skipped ledger, chunk 902). Other CTC components override slot 8 with their own
// predicates; the perception component simply answers yes.
// Shape: `mov al,1 ; ret` -- a bool return with no `this` use.
// Leaf: vfunc_8
struct CTCPerceiveThing_Slot8 {                    // vtable 0x012688CC
    virtual bool vfunc_8();
};

bool CTCPerceiveThing_Slot8::vfunc_8()
{
    return true;
}
