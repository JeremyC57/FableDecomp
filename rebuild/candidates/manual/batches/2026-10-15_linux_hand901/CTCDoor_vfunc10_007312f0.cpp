// CTCDoor::vfunc_10() -- forward vtable slot 10 to slot 9 (CTCDoor::FrameUpdate, 0x007311F0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007312F0, length 5 bytes (16 with padding; slot 23 follows at 0x00731300).
// Vtable 0x01266DE4 (CTCDoor, rtti-vtables.tsv) slot 10; the catalogue row 007312f0 held both bodies (skipped
// ledger, chunk 901). Slot 9 is the landed CTCDoor::FrameUpdate (no arguments), so this is the per-frame hook that
// simply runs the frame update. CTCChest has the identical pair at 0x00750960 / 0x00750970.
// Shape: `mov eax,[ecx] ; jmp [eax+24h]` -- a tail call through the object's own vtable.
struct CTCDoor_Calls {                           // vtable 0x01266DE4
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void FrameUpdate();                // slot 9 (+0x24), 0x007311F0
    virtual void vfunc_10();
};

void CTCDoor_Calls::vfunc_10()
{
    FrameUpdate();
}
