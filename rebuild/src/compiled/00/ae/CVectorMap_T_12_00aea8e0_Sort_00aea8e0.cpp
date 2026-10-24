// CVectorMap<T_12_00aea8e0>::Sort() -- sort the vector map's elements with its Compare and clear the Dirty flag
// Leaf: ?Sort@CVectorMap_T_12_00aea8e0@
// Renamed (fistp2, 2026-10-24) from CGuiVarTransferBool::TransferToGui @ 00/ae/CGuiVarTransferBool_TransferToGui_00aea8e0.cpp: body = `sort(begin (+0), end (+4), Compare (+0xC)) ; Dirty (+0xD) = false` on the retail sorted-vector-map layout (GOTCHAS: CVectorMap = 12-byte vector + empty Compare at +0xC + bool Dirty at +0xD); the callee is labelled `_global::Sort_SElement12_00aea880` (element size 12); the CGuiVarTransferBool::TransferToGui label is propagated nonsense; element type not recoverable, method name descriptive
// CVectorMap_T_12_00aea8e0::Sort  @ 0x00aea8e0
// Faithful VC7.1 reconstruction.

struct CVectorMap_T_12_00aea8e0
{
    void* m_pTarget;   // +0x00  (ecx to callee)
    int   m_edxArg;    // +0x04  (edx to callee)
    // +0x08 unused here
    int   m_pad08;
    bool  m_value;   // +0x0c  bool pushed as arg
    unsigned char m_dirty;   // +0x0d  cleared after transfer
    void Sort();
};

// The callee at 0xaea880: __fastcall (ecx = target, edx = extra), plus a stack bool arg.
extern void __fastcall CGuiVarTransferBool_Apply_00aea880(void* pTarget, int edxArg, bool value);

void CVectorMap_T_12_00aea8e0::Sort()
{
    CGuiVarTransferBool_Apply_00aea880(this->m_pTarget, this->m_edxArg, this->m_value);
    this->m_dirty = 0;
}