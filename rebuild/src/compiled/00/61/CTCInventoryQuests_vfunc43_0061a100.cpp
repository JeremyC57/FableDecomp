// CTCInventoryQuests::vfunc_43() -- slot 43 float getter: 0.035f while the test-quest list mode is active, else the base inventory value (from the inventory def +0x40)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0061A100, 23 bytes (+9 int3). rtti-vtables: CTCInventoryQuests 0x012585B4 slot 43.
// Callee: CTCInventoryBase slot-43 implementation @ 0x005BD427 (reads a float from the PDef inventory def at +0x40 under a
//         CDefPointer copy; also CTCInventoryClothing's slot 43). Float constant @ 0x01258598 = 0.035f.
//
// Shape: `mov al,[ecx+157h] ; test al,al ; je ; fld [const] ; ret ; call base ; ret` -- the base result is taken into a
// NAMED float local: a plain `return Base::f();` becomes a sibling-call `jmp` (GOTCHAS batch 5: a named float local
// blocks the tail jump). +0x157 is ActiveTestQuestMode (PDB +0x177; the retail layout sits 0x20 lower: eight
// 12-byte vectors before it instead of 16-byte ones). Local model (no engine header for CTCInventoryQuests). The
// real method name is unknown (catalog label kept).
struct CTCInventoryBase_Retail {              // primary base, retail size 0x130
    virtual void v00();
    unsigned char _pad_0x04[0x130 - 4];
    float vfunc_43();                         // 0x005BD427
};

struct CTCInventoryQuests : CTCInventoryBase_Retail {
    unsigned char _pad_0x130[0x157 - 0x130];
    bool ActiveTestQuestMode;                 // +0x157
    float vfunc_43();
};

float CTCInventoryQuests::vfunc_43()
{
    if (ActiveTestQuestMode)
        return 0.035f;
    float value = CTCInventoryBase_Retail::vfunc_43();
    return value;
}