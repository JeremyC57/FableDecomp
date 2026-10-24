// CVectorMap<T_00827830>::Sort() -- sort the vector map's elements with its Compare and clear the Dirty flag
// Leaf: ?Sort@CVectorMap_T_00827830@
// Renamed (fistp2, 2026-10-24) from CGuiVarTransferBool::TransferToGui @ 00/82/CGuiVarTransferBool_TransferToGui_00827830.cpp: body = `sort(begin (+0), end (+4), Compare (+0xC)) ; Dirty (+0xD) = false` on the retail sorted-vector-map layout (GOTCHAS: CVectorMap = 12-byte vector + empty Compare at +0xC + bool Dirty at +0xD); the callee is labelled `None`; the CGuiVarTransferBool::TransferToGui label is propagated nonsense; element type not recoverable, method name descriptive
// CVectorMap_T_00827830::Sort  @ 0x00827830
// __fastcall void Sort(CVectorMap_T_00827830* this)

struct GuiTarget;

// callee @ 0x827710 : __fastcall(ecx=target, edx=field, stack=bool value)
void __fastcall GuiTarget_Set(GuiTarget* self, int edx, bool value);

struct CVectorMap_T_00827830
{
    GuiTarget* m_pTarget;   // +0x00  -> ecx
    int        m_field;     // +0x04  -> edx
    int        m_pad8;      // +0x08
    bool       m_value;     // +0x0c  -> pushed bool
    unsigned char m_done;   // +0x0d  -> set to 0
    void Sort();
};

void CVectorMap_T_00827830::Sort()
{
    GuiTarget_Set(m_pTarget, m_field, m_value);
    m_done = 0;
}