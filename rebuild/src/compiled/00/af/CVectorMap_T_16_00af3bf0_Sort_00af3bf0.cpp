// CVectorMap<T_16_00af3bf0>::Sort() -- sort the vector map's elements with its Compare and clear the Dirty flag
// Leaf: ?Sort@CVectorMap_T_16_00af3bf0@
// Renamed (fistp2, 2026-10-24) from CGuiVarTransferBool::TransferToGui @ 00/af/CGuiVarTransferBool_TransferToGui_00af3bf0.cpp: body = `sort(begin (+0), end (+4), Compare (+0xC)) ; Dirty (+0xD) = false` on the retail sorted-vector-map layout (GOTCHAS: CVectorMap = 12-byte vector + empty Compare at +0xC + bool Dirty at +0xD); the callee is labelled `_global::Sort_SElement16_00af3b70` (element size 16); the CGuiVarTransferBool::TransferToGui label is propagated nonsense; element type not recoverable, method name descriptive
// CVectorMap_T_16_00af3bf0::Sort  @ 00af3bf0

struct GuiTarget;

// callee at 0xaf3b70: __fastcall (ecx=target, edx=id, stack bool)
void __fastcall GuiBoolStore(GuiTarget* self, int id, bool value);

struct CVectorMap_T_16_00af3bf0
{
    GuiTarget*    m_target;   // +0x00
    int           m_id;       // +0x04
    int           m_pad8;     // +0x08
    bool          m_value;    // +0x0c
    bool          m_dirty;    // +0x0d

    void Sort();
};

void CVectorMap_T_16_00af3bf0::Sort()
{
    GuiBoolStore(this->m_target, this->m_id, this->m_value);
    this->m_dirty = false;
}