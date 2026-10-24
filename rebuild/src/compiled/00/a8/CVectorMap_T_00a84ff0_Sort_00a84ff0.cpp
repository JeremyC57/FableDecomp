// CVectorMap<T_00a84ff0>::Sort() -- sort the vector map's elements with its Compare and clear the Dirty flag
// Leaf: ?Sort@CVectorMap_T_00a84ff0@
// Renamed (fistp2, 2026-10-24) from CGuiVarTransferBool::TransferToGui @ 00/a8/CGuiVarTransferBool_TransferToGui_00a84ff0.cpp: body = `sort(begin (+0), end (+4), Compare (+0xC)) ; Dirty (+0xD) = false` on the retail sorted-vector-map layout (GOTCHAS: CVectorMap = 12-byte vector + empty Compare at +0xC + bool Dirty at +0xD); the callee is labelled `None`; the CGuiVarTransferBool::TransferToGui label is propagated nonsense; element type not recoverable, method name descriptive
// CVectorMap_T_00a84ff0::Sort  @ 0x00a84ff0

// Forward-declared external helper (0x00a84cf0). It is invoked with
// ecx = self->m_pTarget, edx = self->m_data, and a pushed byte value.
// Model it as an __fastcall taking (ecx, edx) plus one stack int.
void __fastcall CGuiVarTransferBool_Apply(void *pTarget, int edxData, bool value);

struct CVectorMap_T_00a84ff0
{
    void *m_pTarget;    // +0x00  -> ecx
    int   m_data;       // +0x04  -> edx
    // +0x08 (unused here)
    int   m_pad08;
    bool m_value;            // +0x0c  pushed as int
    unsigned char m_dirty;   // +0x0d  cleared to 0

    void Sort();
};

void CVectorMap_T_00a84ff0::Sort()
{
    CGuiVarTransferBool_Apply(m_pTarget, m_data, m_value);
    m_dirty = 0;
}