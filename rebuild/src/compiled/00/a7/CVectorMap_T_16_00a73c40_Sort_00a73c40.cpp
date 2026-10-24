// CVectorMap<T_16_00a73c40>::Sort() -- sort the vector map's elements with its Compare and clear the Dirty flag
// Leaf: ?Sort@CVectorMap_T_16_00a73c40@
// Renamed (fistp2, 2026-10-24) from CGuiVarTransferBool::TransferToGui @ 00/a7/CGuiVarTransferBool_TransferToGui_00a73c40.cpp: body = `sort(begin (+0), end (+4), Compare (+0xC)) ; Dirty (+0xD) = false` on the retail sorted-vector-map layout (GOTCHAS: CVectorMap = 12-byte vector + empty Compare at +0xC + bool Dirty at +0xD); the callee is labelled `_global::Sort_SElement16_00a73b20` (element size 16); the CGuiVarTransferBool::TransferToGui label is propagated nonsense; element type not recoverable, method name descriptive
// CVectorMap_T_16_00a73c40::Sort  @ 0x00a73c40  __fastcall(void)
// Callee at 0x00a73b20 is __fastcall(ecx, edx, stack-arg): setter that takes
// the target object (ecx), a secondary value (edx) and the bool value (stack).
extern "C" void __fastcall Transfer_a73b20(void* self, int edxarg, bool val);

struct CVectorMap_T_16_00a73c40
{
    void*         m_target;   // +0x00 -> ecx for the callee
    int           m_edxval;   // +0x04 -> edx for the callee
    int           m_pad08;    // +0x08
    bool          m_value;    // +0x0c -> the bool pushed to the callee
    bool          m_written;  // +0x0d -> cleared after the transfer

    void Sort();
};

void CVectorMap_T_16_00a73c40::Sort()
{
    Transfer_a73b20(m_target, m_edxval, m_value);
    m_written = false;
}