// CVectorMap<T_12_00a420e0>::Sort() -- sort the vector map's elements with its Compare and clear the Dirty flag
// Leaf: ?Sort@CVectorMap_T_12_00a420e0@
// Renamed (fistp2, 2026-10-24) from CGuiVarTransferBool::TransferToGui @ 00/a4/CGuiVarTransferBool_TransferToGui_00a420e0.cpp: body = `sort(begin (+0), end (+4), Compare (+0xC)) ; Dirty (+0xD) = false` on the retail sorted-vector-map layout (GOTCHAS: CVectorMap = 12-byte vector + empty Compare at +0xC + bool Dirty at +0xD); the callee is labelled `_global::Sort_SElement12_00a42060` (element size 12); the CGuiVarTransferBool::TransferToGui label is propagated nonsense; element type not recoverable, method name descriptive
// CVectorMap_T_12_00a420e0::Sort  @ 0x00a420e0
// Faithful VC7.1 reconstruction.

struct GuiTarget;

// callee at 0x00a42060: __fastcall(ecx=obj, edx=idx, stack=bool)
void __fastcall SetGuiValue(GuiTarget *obj, int idx, unsigned char val);

struct CVectorMap_T_12_00a420e0
{
    GuiTarget *m_target;   // +0x00
    int        m_index;    // +0x04
    // +0x08 padding/other
    int        m_pad8;     // +0x08
    unsigned char m_value; // +0x0c
    unsigned char m_done;  // +0x0d

    void Sort();
};

void CVectorMap_T_12_00a420e0::Sort()
{
    SetGuiValue(m_target, m_index, m_value);
    m_done = 0;
}