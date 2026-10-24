// CVectorMap<T_16_00a73c60>::Sort() -- sort the vector map's elements with its Compare and clear the Dirty flag
// Leaf: ?Sort@CVectorMap_T_16_00a73c60@
// Renamed (fistp2, 2026-10-24) from CGuiVarTransferBool::TransferToGui @ 00/a7/CGuiVarTransferBool_TransferToGui_00a73c60.cpp: body = `sort(begin (+0), end (+4), Compare (+0xC)) ; Dirty (+0xD) = false` on the retail sorted-vector-map layout (GOTCHAS: CVectorMap = 12-byte vector + empty Compare at +0xC + bool Dirty at +0xD); the callee is labelled `_global::Sort_SElement16_00a73ba0` (element size 16); the CGuiVarTransferBool::TransferToGui label is propagated nonsense; element type not recoverable, method name descriptive
// CVectorMap_T_16_00a73c60::Sort  @ 0x00a73c60

struct GuiTarget {
    // __fastcall member: this=ecx, first arg=edx, second arg on stack
    void __fastcall Set(int a, unsigned char b);
};

struct CVectorMap_T_16_00a73c60 {
    GuiTarget* target;   // +0x00
    int        param;    // +0x04
    char       pad8[4];  // +0x08
    unsigned char value; // +0x0c
    char       flag;     // +0x0d

    void Sort();
};

void CVectorMap_T_16_00a73c60::Sort()
{
    this->target->Set(this->param, this->value);
    this->flag = 0;
}