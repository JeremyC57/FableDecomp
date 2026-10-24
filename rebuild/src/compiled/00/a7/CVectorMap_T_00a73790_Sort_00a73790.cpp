// CVectorMap<T_00a73790>::Sort() -- sort the vector map's elements with its Compare and clear the Dirty flag
// Leaf: ?Sort@CVectorMap_T_00a73790@
// Renamed (fistp2, 2026-10-24) from CGuiVarTransferBool::TransferToGui @ 00/a7/CGuiVarTransferBool_TransferToGui_00a73790.cpp: body = `sort(begin (+0), end (+4), Compare (+0xC)) ; Dirty (+0xD) = false` on the retail sorted-vector-map layout (GOTCHAS: CVectorMap = 12-byte vector + empty Compare at +0xC + bool Dirty at +0xD); the callee is labelled `None`; the CGuiVarTransferBool::TransferToGui label is propagated nonsense; element type not recoverable, method name descriptive
// CVectorMap_T_00a73790::Sort  @ 00a73790
struct GuiTarget;

// callee 0xa73600: ecx=GuiTarget*, edx=int, stack=bool
void __fastcall GuiTarget_Set_00a73600(GuiTarget* self, int edx, bool value);

struct CVectorMap_T_00a73790 {
    GuiTarget* target;   // +0
    int        edxArg;   // +4
    char       pad8[4];  // +8
    bool       value;    // +0xc
    bool       dirty;    // +0xd

    void Sort();
};

void CVectorMap_T_00a73790::Sort() {
    GuiTarget_Set_00a73600(this->target, this->edxArg, this->value);
    this->dirty = false;
}