// CTCChest::vfunc_10() -- forward vtable slot 10 to slot 9 (the frame update, 0x00750780)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00750960, length 5 bytes (16 with padding; slot 23 follows at 0x00750970).
// Vtable 0x0126957C (CTCChest, rtti-vtables.tsv) slot 10; the catalogue row 00750960 held both bodies (skipped
// ledger, chunk 901). Slot 9 is 0x00750780 (CTCChest's frame update, unlanded); CTCDoor has the identical pair at
// 0x007312F0 / 0x00731300 where slot 9 is the landed CTCDoor::FrameUpdate.
// Shape: `mov eax,[ecx] ; jmp [eax+24h]` -- a tail call through the object's own vtable.
struct CTCChest_Calls {                           // vtable 0x0126957C
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void FrameUpdate();                // slot 9 (+0x24), 0x00750780
    virtual void vfunc_10();
};

void CTCChest_Calls::vfunc_10()
{
    FrameUpdate();
}
