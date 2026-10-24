// CVectorMap<T_12_00829950>::Sort() -- sort the vector map's elements with its Compare and clear the Dirty flag
// Leaf: ?Sort@CVectorMap_T_12_00829950@
// Renamed (fistp2, 2026-10-24) from CGuiVarTransferBool::TransferToGui @ 00/82/CGuiVarTransferBool_TransferToGui_00829950.cpp: body = `sort(begin (+0), end (+4), Compare (+0xC)) ; Dirty (+0xD) = false` on the retail sorted-vector-map layout (GOTCHAS: CVectorMap = 12-byte vector + empty Compare at +0xC + bool Dirty at +0xD); the callee is labelled `_global::Sort_SElement12_008298f0` (element size 12); the CGuiVarTransferBool::TransferToGui label is propagated nonsense; element type not recoverable, method name descriptive
// CVectorMap_T_12_00829950::Sort  @ 0x00829950

// The engine call at 0x8298f0 receives:
//   ecx = this->m_a  (field +0)
//   edx = this->m_b  (field +4)
//   stack arg = this->m_value (byte at +0xc), zero-extended
// This is a __fastcall (2 register args + 1 stack arg).
void __fastcall SetGuiBoolValue(void* dst, int idx, bool value);

struct CVectorMap_T_12_00829950
{
    void* m_a;      // +0
    int   m_b;      // +4
    int   m_pad;    // +8
    bool  m_value;  // +0xc
    bool  m_dirty;  // +0xd

    void Sort();
};

void CVectorMap_T_12_00829950::Sort()
{
    SetGuiBoolValue(m_a, m_b, m_value);
    m_dirty = false;
}