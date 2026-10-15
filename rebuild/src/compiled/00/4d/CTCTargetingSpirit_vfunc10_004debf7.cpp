// CTCTargetingSpirit::vfunc_10() -- forward vtable slot 10 to slot 9 (0x007F9B90)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DEBF7, length 5 bytes (no padding: slot 29 follows at 0x004DEBFC).
// Vtable 0x012419A4 (CTCTargetingSpirit, rtti-vtables.tsv) slot 10, shared by the vtables at 0x01242834 and 0x01271E9C.
// The catalogue row 004debf7 (48 bytes) held six bodies (slots 10, 29, 31, 32, 22, 8) -- skipped ledger, chunk 903.
// The same slot-10 -> slot-9 pair appears in CTCDoor / CTCChest (0x007312F0 / 0x00750960, landed).
// Shape: `mov eax,[ecx] ; jmp [eax+24h]`.
struct CTCTargetingSpirit_Calls {              // vtable 0x012419A4
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void vfunc_9();                    // slot 9 (+0x24), 0x007F9B90
    virtual void vfunc_10();
};

void CTCTargetingSpirit_Calls::vfunc_10()
{
    vfunc_9();
}