// CVectorMap<T_16_009e7ec0>::Sort() -- sort the vector map's elements with its Compare and clear the Dirty flag
// Leaf: ?Sort@CVectorMap_T_16_009e7ec0@
// Renamed (fistp2, 2026-10-24) from CGuiVarTransferBool::TransferToGui @ 00/9e/CGuiVarTransferBool_TransferToGui_009e7ec0.cpp: body = `sort(begin (+0), end (+4), Compare (+0xC)) ; Dirty (+0xD) = false` on the retail sorted-vector-map layout (GOTCHAS: CVectorMap = 12-byte vector + empty Compare at +0xC + bool Dirty at +0xD); the callee is labelled `_global::Sort_SElement16_009e7e40` (element size 16); the CGuiVarTransferBool::TransferToGui label is propagated nonsense; element type not recoverable, method name descriptive
// CVectorMap_T_16_009e7ec0::Sort  @ 009e7ec0
// __fastcall void Sort(CVectorMap_T_16_009e7ec0* this)

struct GuiVarTarget;

// callee @ 0x9e7e40: __fastcall (ecx=p1, edx=p2, stack=byte value)
extern void __fastcall TransferBoolHelper(void* p1, void* p2, bool val);

struct CVectorMap_T_16_009e7ec0
{
    void*          m_pTarget;      // +0x00 -> ecx (this for helper)
    void*          m_pVar;         // +0x04 -> edx
    unsigned char  m_pad08[4];     // +0x08
    bool           m_value;        // +0x0c
    bool           m_dirty;        // +0x0d

    void Sort();
};

void CVectorMap_T_16_009e7ec0::Sort()
{
    TransferBoolHelper(m_pTarget, m_pVar, m_value);
    m_dirty = false;
}