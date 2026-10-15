// CTCInventoryQuests::ProcessInventoryInputUp() -- with the +0x157 flag set run the quest-list hook 0x0061AA80; else dispatch on the menu mode (+0x1c): 0 -> CTCInventoryBase::ProcessInventoryInputUp, 1 -> virtual slot 72
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0061B530, length 40 bytes (48 with padding).
// Vtable 0x012585B4 (CTCInventoryQuests, rtti-vtables.tsv) slot 38; the catalogue row 0061b530 (96 bytes) held slots 38
// and 39 (skipped ledger, chunk 903). Callees: 0x0061AA80 (a thiscall on `this`, unlanded), CTCInventoryBase::ProcessInventoryInputUp
// @ 0x005BDD8C (landed; the base's slot-38 handler called non-virtually), virtual slot 72 (+0x120).
// Layout: the landed sibling 0x0061B5F0 reads the same bool at +0x157; the menu mode at +0x1c is the CTCInventoryBase word
// the landed ProcessButton* handlers switch on. Retail's containers are 12 bytes, so the class is modelled locally.
// Shape: `mov al,[ecx+157h] ; test al,al ; je ; jmp hook` then the switch (`sub eax,0 ; je ; dec eax ; jne ret`): case 0 a
// tail `jmp` to the base body, case 1 a tail `jmp [eax+0x120]`.
struct CTCInventoryBase_Calls {
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
    virtual void v32();
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
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual void v70();
    virtual void v71();
    virtual void vfunc_72();              // slot 72 (+0x120)
    unsigned char _pad_0x04[0x1c - 0x04];
    long          m_menuMode;                  // +0x1c
    void ProcessInventoryInputUp();                             // 0x005BDD8C
};

struct CTCInventoryQuests : CTCInventoryBase_Calls {     // vtable 0x012585B4
    unsigned char _pad_0x20[0x157 - 0x20];
    bool          m_questListActive;           // +0x157
    void QuestListHook_0061AA80();           // 0x0061AA80
    void ProcessInventoryInputUp();
};

void CTCInventoryQuests::ProcessInventoryInputUp()
{
    if (m_questListActive) {
        QuestListHook_0061AA80();
        return;
    }
    switch (m_menuMode) {
    case 0:
        CTCInventoryBase_Calls::ProcessInventoryInputUp();
        break;
    case 1:
        vfunc_72();
        break;
    }
}
