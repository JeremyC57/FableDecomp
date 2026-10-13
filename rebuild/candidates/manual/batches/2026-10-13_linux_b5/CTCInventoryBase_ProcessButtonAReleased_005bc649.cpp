// CTCInventoryBase::ProcessButtonAReleased() (two byte-identical bodies)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005BC649, oracle length 17 bytes.
// Grade: candidate -- authored and graded in the Linux lane (Wine + DDK 13.10.4035 cl.exe); the
//        batch README records the lane verdict.
//
// Retail bytes:  cmp dword ptr [ecx+1Ch],0 ; jne R ; mov eax,[ecx] ; push 1 ; call [eax+0B8h] ; R: ret
// While no transaction/selection object is held at +0x1C the inventory closes itself through virtual
// slot 46 (+0xB8) with `true`. The class is vtable-shaped here: engine/CTCInventoryBase.h is not
// generated and only +0x1C and the slot matter.
struct CTCInventoryBase_Virtuals {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual void v30(); virtual void v34(); virtual void v38(); virtual void v3c();
    virtual void v40(); virtual void v44(); virtual void v48(); virtual void v4c();
    virtual void v50(); virtual void v54(); virtual void v58(); virtual void v5c();
    virtual void v60(); virtual void v64(); virtual void v68(); virtual void v6c();
    virtual void v70(); virtual void v74(); virtual void v78(); virtual void v7c();
    virtual void v80(); virtual void v84(); virtual void v88(); virtual void v8c();
    virtual void v90(); virtual void v94(); virtual void v98(); virtual void v9c();
    virtual void va0(); virtual void va4(); virtual void va8(); virtual void vac();
    virtual void vb0(); virtual void vb4();
    virtual void Close(bool immediate);     // slot 46 (+0xB8)
    unsigned char _pad_0x04[0x18];
    void*         Selection;                // +0x1C (name from use)
    void ProcessButtonAReleased();
};

void CTCInventoryBase_Virtuals::ProcessButtonAReleased()
{
    if (this->Selection == 0)
        Close(true);
}
