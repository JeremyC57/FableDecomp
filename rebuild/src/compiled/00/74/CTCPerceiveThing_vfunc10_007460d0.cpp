// CTCPerceiveThing::vfunc_10() -- forward vtable slot 10 to the component's own slot 9 (its frame update)
// Retail: Fable.exe (TLC, VC7.1) @ 0x007460D0, 5 bytes. Vtable 0x012688CC (CTCPerceiveThing, rtti-vtables.tsv)
// slot 10; slot 9 is 0x00745C70 (CTCPerceiveThing's frame update, unlanded). Same pair as CTCChest 0x00750960 and
// CTCDoor 0x007312F0 (landed): the catalogue row held three tiny bodies (skipped ledger, chunk 902).
// Leaf: vfunc_10
// Shape: `mov eax,[ecx] ; jmp [eax+24h]` -- a call to the object's own virtual as the whole body is tail-jumped.
struct CTCPerceiveThing_Calls {                    // vtable 0x012688CC
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual bool v08();
    virtual void FrameUpdate();                    // slot 9 (+0x24), 0x00745C70
    virtual void vfunc_10();
};

void CTCPerceiveThing_Calls::vfunc_10()
{
    FrameUpdate();
}