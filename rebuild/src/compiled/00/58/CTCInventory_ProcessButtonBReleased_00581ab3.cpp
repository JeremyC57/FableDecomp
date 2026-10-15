// CTCInventory::ProcessButtonBReleased() -- when neither suppress flag (+0x1cc, +0x1cd) is set, dispatch on the menu mode at +0x1c: 0 -> slot 32, 1 -> slot 46(0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00581AB3, length 46 bytes (no padding: slot 60 follows at 0x00581AE1).
// Vtable 0x01250DE4 (CTCInventory, rtti-vtables.tsv) slot 59 (ProcessButtonBReleased, named from the landed slot 58/61
// siblings ProcessButtonAReleased / ProcessButtonYReleased, chunk 91); the row was catalogued as one 78-byte function holding
// slots 59 and 60 (skipped ledger 00581ab3, chunk 901). engine/CTCInventory.h (PDB) has only padding at +0x1c and
// puts a CRightHandedSet at +0x1c8; retail's containers are 4 bytes smaller, so the two bools at +0x1cc/+0x1cd are
// modelled locally.
// A size-optimised TU (#pragma optimize("s", on)): one zero register serves every compare, the switch and the push.
// Shape: `xor edx,edx` shared by the two byte compares, the zero push and the switch lowering (`sub eax,edx ; je`,
// `dec eax ; jne`); case 0 is a tail `jmp [eax+80h]`, case 1 `push edx ; call [eax+0B8h] ; ret`.
#pragma optimize("s", on)
struct CTCInventory {                            // vtable 0x01250DE4
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void vfunc_32();                   // slot 32 (+0x80)
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void vfunc_46(long arg);           // slot 46 (+0xb8)
    unsigned char _pad_0x04[0x1c - 0x04];
    long m_menuMode;                           // +0x1c
    unsigned char _pad_0x20[0x1cc - 0x20];
    bool m_suppressA;                          // +0x1cc
    bool m_suppressB;                          // +0x1cd
    void ProcessButtonBReleased();
};

void CTCInventory::ProcessButtonBReleased()
{
    if (!m_suppressA && !m_suppressB) {
        switch (m_menuMode) {
        case 0:
            vfunc_32();
            break;
        case 1:
            vfunc_46(0);
            break;
        }
    }
}