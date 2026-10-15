// CTCInventoryMap::ProcessButtonBReleased() -- with no selection in progress the first B release clears the map screen's primitives and closes the screen; otherwise the base inventory handles it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005FD0B0, 54 bytes (+10 int3). rtti/abs ref: vtable entry at 0x01257B88.
// Callees: the map's primitive-handle clear-down @ 0x005FC510 (empties the primitive-handle vectors at +0x1AC, +0x1B8, ...),
//          virtual slot 46 (+0xB8, `Close(true)`), CTCInventoryBase::ProcessButtonBReleased @ 0x005BC66F (tail jump).
//
// Shape: `cmp [esi+1Ch],0 ; jne base ; cmp byte [esi+234h],0 ; jne base ; mov byte [esi+234h],1 ; call ; push 1 ;
// call [eax+0B8h] ; ret ; base: mov ecx,esi ; jmp` -- an `&&` guard with an early `return`, the qualified base call as
// the tail. +0x1C is CTCInventoryBase::InventorySelectionState (PDB +0x20; retail 4 bytes lower, its Components vector
// is 12 bytes); slot 46 is the close request also used by the landed CTCInventoryBase::ProcessButtonAReleased
// (0x005BC649). The byte at +0x234 is a one-shot "closing" latch (name from use). Local model, no engine header.
struct CTCInventoryBase_Retail {
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
    virtual void Close(bool immediate);       // slot 46 (+0xB8)
    unsigned char _pad_0x04[0x1c - 4];
    long InventorySelectionState;             // +0x1C
    unsigned char _pad_0x20[0x130 - 0x20];
    void ProcessButtonBReleased();            // 0x005BC66F
};

struct CTCInventoryMap : CTCInventoryBase_Retail {
    unsigned char _pad_0x130[0x234 - 0x130];
    bool m_closing;                           // +0x234 (name from use)
    void ClearMapScreenPrimitives();          // 0x005FC510
    void ProcessButtonBReleased();
};

void CTCInventoryMap::ProcessButtonBReleased()
{
    if (InventorySelectionState == 0 && !m_closing) {
        m_closing = true;
        ClearMapScreenPrimitives();
        Close(true);
        return;
    }
    CTCInventoryBase_Retail::ProcessButtonBReleased();
}
