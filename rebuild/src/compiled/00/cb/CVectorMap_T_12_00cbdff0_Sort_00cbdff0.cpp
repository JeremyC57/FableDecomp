// CVectorMap<T_12_00cbdff0>::Sort() -- sort the vector map's elements with its Compare and clear the Dirty flag
// Leaf: ?Sort@CVectorMap_T_12_00cbdff0@
// Renamed (fistp2, 2026-10-24) from CGuiVarTransferBool::TransferToGui @ 00/cb/CGuiVarTransferBool_TransferToGui_00cbdff0.cpp: body = `sort(begin (+0), end (+4), Compare (+0xC)) ; Dirty (+0xD) = false` on the retail sorted-vector-map layout (GOTCHAS: CVectorMap = 12-byte vector + empty Compare at +0xC + bool Dirty at +0xD); the callee is labelled `_global::Sort_SElement12_00cbdf70` (element size 12); the CGuiVarTransferBool::TransferToGui label is propagated nonsense; element type not recoverable, method name descriptive
// CVectorMap_T_12_00cbdff0::Sort @ 00cbdff0

struct GuiTargetObj;

// callee @ 0xcbdf70 : ecx=obj, edx=second reg arg, one pushed stack arg (bool)
void __fastcall SetGuiBoolValue(GuiTargetObj* obj, void* edxArg, unsigned char value);

struct CVectorMap_T_12_00cbdff0
{
    GuiTargetObj* pTarget;   // +0x00
    void*         edxArg;    // +0x04
    int           pad08;     // +0x08
    unsigned char curValue;  // +0x0C
    unsigned char dirtyFlag; // +0x0D

    void Sort();
};

void CVectorMap_T_12_00cbdff0::Sort()
{
    SetGuiBoolValue(this->pTarget, this->edxArg, this->curValue);
    this->dirtyFlag = 0;
}