// CVectorMap<T_20_00a73770>::Sort() -- sort the vector map's elements with its Compare and clear the Dirty flag
// Leaf: ?Sort@CVectorMap_T_20_00a73770@
// Renamed (fistp2, 2026-10-24) from CGuiVarTransferBool::TransferToGui @ 00/a7/CGuiVarTransferBool_TransferToGui_00a73770.cpp: body = `sort(begin (+0), end (+4), Compare (+0xC)) ; Dirty (+0xD) = false` on the retail sorted-vector-map layout (GOTCHAS: CVectorMap = 12-byte vector + empty Compare at +0xC + bool Dirty at +0xD); the callee is labelled `_global::Sort_SElement20_00a735a0` (element size 20); the CGuiVarTransferBool::TransferToGui label is propagated nonsense; element type not recoverable, method name descriptive
// CVectorMap_T_20_00a73770::Sort @ 0x00a73770

struct GuiTarget;
struct GuiSource;

// callee at 0xa735a0: __fastcall(ecx, edx, bool)
void __fastcall SetGuiBoolValue(GuiTarget *ecxArg, GuiSource *edxArg, bool value);

struct CVectorMap_T_20_00a73770
{
    GuiTarget *m_target;   // +0x00 -> ecx of callee
    GuiSource *m_source;   // +0x04 -> edx of callee
    // +0x08 padding
    int        m_pad08;    // +0x08
    bool       m_value;    // +0x0c -> pushed byte
    bool       m_dirty;    // +0x0d -> cleared

    void Sort();
};

void CVectorMap_T_20_00a73770::Sort()
{
    SetGuiBoolValue(m_target, m_source, m_value);
    m_dirty = false;
}