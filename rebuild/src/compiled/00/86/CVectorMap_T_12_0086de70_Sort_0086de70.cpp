// CVectorMap<T_12_0086de70>::Sort() -- sort the vector map's elements with its Compare and clear the Dirty flag
// Leaf: ?Sort@CVectorMap_T_12_0086de70@
// Renamed (fistp2, 2026-10-24) from CGuiVarTransferBool::TransferToGui @ 00/86/CGuiVarTransferBool_TransferToGui_0086de70.cpp: body = `sort(begin (+0), end (+4), Compare (+0xC)) ; Dirty (+0xD) = false` on the retail sorted-vector-map layout (GOTCHAS: CVectorMap = 12-byte vector + empty Compare at +0xC + bool Dirty at +0xD); the callee is labelled `_global::Sort_SElement12_0086dd20` (element size 12); the CGuiVarTransferBool::TransferToGui label is propagated nonsense; element type not recoverable, method name descriptive
// CVectorMap_T_12_0086de70::Sort  @ 0086de70

// The callee at 0x86dd20 receives ecx and edx as its first two args
// (fastcall), plus one stack byte. Model it as a free __fastcall helper.
extern void __fastcall GuiTransferBoolHelper(void *a, void *b, unsigned char v);

struct CVectorMap_T_12_0086de70
{
    void         *m_pTarget;   // +0x00  -> ecx of callee
    void         *m_pSource;   // +0x04  -> edx of callee
    void         *m_pad08;     // +0x08
    unsigned char m_value;     // +0x0c
    unsigned char m_dirty;     // +0x0d

    void Sort();
};

void CVectorMap_T_12_0086de70::Sort()
{
    GuiTransferBoolHelper(m_pTarget, m_pSource, m_value);
    m_dirty = 0;
}