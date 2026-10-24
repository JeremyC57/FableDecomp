// CVectorMap<T_24_00cb77e0>::Sort() -- sort the vector map's elements with its Compare and clear the Dirty flag
// Leaf: ?Sort@CVectorMap_T_24_00cb77e0@
// Renamed (fistp2, 2026-10-24) from CGuiVarTransferBool::TransferToGui @ 00/cb/CGuiVarTransferBool_TransferToGui_00cb77e0.cpp: body = `sort(begin (+0), end (+4), Compare (+0xC)) ; Dirty (+0xD) = false` on the retail sorted-vector-map layout (GOTCHAS: CVectorMap = 12-byte vector + empty Compare at +0xC + bool Dirty at +0xD); the callee is labelled `_global::Sort_SElement24_00cb7780` (element size 24); the CGuiVarTransferBool::TransferToGui label is propagated nonsense; element type not recoverable, method name descriptive
// CVectorMap_T_24_00cb77e0::Sort  @ 0x00cb77e0
// Faithful VC7.1 reconstruction.

struct GuiVarTarget;

// External helper at 0x00cb7780. It is invoked as a __fastcall taking the
// target pointer in ecx, an int in edx, and a bool pushed on the stack.
void __fastcall GuiVarBoolSet(GuiVarTarget* target, int index, bool value);

struct CVectorMap_T_24_00cb77e0
{
    GuiVarTarget* m_target;   // +0x00 -> ecx
    int           m_index;    // +0x04 -> edx
    // +0x08 padding / other
    int           m_pad08;    // +0x08
    bool          m_value;    // +0x0c
    bool          m_dirty;    // +0x0d

    void Sort();
};

void CVectorMap_T_24_00cb77e0::Sort()
{
    GuiVarBoolSet(this->m_target, this->m_index, this->m_value);
    this->m_dirty = false;
}