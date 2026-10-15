// CTCInventoryQuests::vfunc_42() -- slot 42 float getter: 0.5f while the test-quest list mode is active, else the base inventory value (from the inventory def +0x3C)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0061A0E0, 23 bytes (+9 int3). rtti-vtables: CTCInventoryQuests 0x012585B4 slot 42.
// Callee: CTCInventoryBase slot-42 implementation @ 0x005BD404 (reads a float from the PDef inventory def at +0x3C under a
//         CDefPointer copy; also CTCInventoryClothing's slot 42). Float constant @ 0x0122F59C = 0.5f.
//
// Shape: `mov al,[ecx+157h] ; test al,al ; je ; fld [const] ; ret ; call base ; ret` -- the base result is taken into a
// NAMED float local: a plain `return Base::f();` becomes a sibling-call `jmp` (GOTCHAS batch 5: a named float local
// blocks the tail jump). +0x157 is ActiveTestQuestMode (PDB +0x177; the retail layout sits 0x20 lower: eight
// 12-byte vectors before it instead of 16-byte ones). Local model (no engine header for CTCInventoryQuests). The
// real method name is unknown (catalog label kept).
struct CTCInventoryBase_Retail {              // primary base, retail size 0x130
    virtual void v00();
    unsigned char _pad_0x04[0x130 - 4];
    float vfunc_42();                         // 0x005BD404
};

struct CTCInventoryQuests : CTCInventoryBase_Retail {
    unsigned char _pad_0x130[0x157 - 0x130];
    bool ActiveTestQuestMode;                 // +0x157
    float vfunc_42();
};

float CTCInventoryQuests::vfunc_42()
{
    if (ActiveTestQuestMode)
        return 0.5f;
    float value = CTCInventoryBase_Retail::vfunc_42();
    return value;
}
