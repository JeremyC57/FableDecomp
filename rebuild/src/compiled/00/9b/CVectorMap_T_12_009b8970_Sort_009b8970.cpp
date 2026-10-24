// CVectorMap<T_12_009b8970>::Sort() -- sort the vector map's elements with its Compare and clear the Dirty flag
// Leaf: ?Sort@CVectorMap_T_12_009b8970@
// Renamed (fistp2, 2026-10-24) from CGuiVarTransferBool::TransferToGui @ 00/9b/CGuiVarTransferBool_TransferToGui_009b8970.cpp: body = `sort(begin (+0), end (+4), Compare (+0xC)) ; Dirty (+0xD) = false` on the retail sorted-vector-map layout (GOTCHAS: CVectorMap = 12-byte vector + empty Compare at +0xC + bool Dirty at +0xD); the callee is labelled `_global::Sort_SElement12_009b84c0` (element size 12); the CGuiVarTransferBool::TransferToGui label is propagated nonsense; element type not recoverable, method name descriptive
// CVectorMap_T_12_009b8970::Sort @ 009b8970

struct GuiTarget;

// helper at 0x9b84c0: __fastcall on GuiTarget*, with an extra edx arg and a stack byte arg.
// Model as __fastcall: ecx=self, edx=idx, stack=val.
void __fastcall GuiTargetSet(GuiTarget* self, int idx, unsigned char val);

struct CVectorMap_T_12_009b8970
{
    GuiTarget* target;   // +0
    int        idx;      // +4
    int        pad8;     // +8
    unsigned char value; // +0xc
    unsigned char flag;  // +0xd

    void Sort();
};

void CVectorMap_T_12_009b8970::Sort()
{
    GuiTargetSet(this->target, this->idx, this->value);
    this->flag = 0;
}