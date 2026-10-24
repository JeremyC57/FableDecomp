// CVectorMap<T_40_0081e170>::Sort() -- sort the vector map's elements with its Compare and clear the Dirty flag
// Leaf: ?Sort@CVectorMap_T_40_0081e170@
// Renamed (fistp2, 2026-10-24) from CGuiVarTransferBool::TransferToGui @ 00/81/CGuiVarTransferBool_TransferToGui_0081e170.cpp: body = `sort(begin (+0), end (+4), Compare (+0xC)) ; Dirty (+0xD) = false` on the retail sorted-vector-map layout (GOTCHAS: CVectorMap = 12-byte vector + empty Compare at +0xC + bool Dirty at +0xD); the callee is labelled `_global::Sort_SElement40_0081e090` (element size 40); the CGuiVarTransferBool::TransferToGui label is propagated nonsense; element type not recoverable, method name descriptive
// CVectorMap_T_40_0081e170::Sort  @ 0x0081e170
// __fastcall void Sort(CVectorMap_T_40_0081e170* this)

struct CGuiTarget;

// Callee at 0x0081e090: __fastcall(ecx=obj, edx=field, stackarg=bool)
void __fastcall GuiVarStore(CGuiTarget* obj, void* field, bool value);

struct CVectorMap_T_40_0081e170
{
    CGuiTarget* m_obj;   // +0x00
    void*       m_field; // +0x04
    void*       m_pad08; // +0x08
    bool        m_value; // +0x0c
    bool        m_dirty; // +0x0d

    void Sort();
};

void CVectorMap_T_40_0081e170::Sort()
{
    GuiVarStore(this->m_obj, this->m_field, this->m_value);
    this->m_dirty = false;
}