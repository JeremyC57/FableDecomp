// CVectorMap<T_8_009b8990>::Sort() -- sort the vector map's elements with its Compare and clear the Dirty flag
// Leaf: ?Sort@CVectorMap_T_8_009b8990@
// Renamed (fistp2, 2026-10-24) from CGuiVarTransferBool::TransferToGui @ 00/9b/CGuiVarTransferBool_TransferToGui_009b8990.cpp: body = `sort(begin (+0), end (+4), Compare (+0xC)) ; Dirty (+0xD) = false` on the retail sorted-vector-map layout (GOTCHAS: CVectorMap = 12-byte vector + empty Compare at +0xC + bool Dirty at +0xD); the callee is labelled `_global::Sort_SElement8_009b8520` (element size 8); the CGuiVarTransferBool::TransferToGui label is propagated nonsense; element type not recoverable, method name descriptive
// CVectorMap_T_8_009b8990::Sort  @ 009b8990
// __fastcall, void(CVectorMap_T_8_009b8990*)

// The engine callee is a __fastcall: ecx = target object, edx = a second
// pointer/int, plus one stack byte argument.
void __fastcall GuiVarTransferBoolCommit(void* target, int edx, unsigned char value);

struct CVectorMap_T_8_009b8990
{
    void*         m_target;   // +0x00  -> ecx of the callee
    int           m_edxArg;   // +0x04  -> edx of the callee
    int           m_pad8;     // +0x08
    unsigned char m_value;    // +0x0C  bool value to push
    unsigned char m_dirty;    // +0x0D  cleared after commit

    void Sort();
};

void CVectorMap_T_8_009b8990::Sort()
{
    GuiVarTransferBoolCommit(this->m_target, this->m_edxArg, this->m_value);
    this->m_dirty = 0;
}