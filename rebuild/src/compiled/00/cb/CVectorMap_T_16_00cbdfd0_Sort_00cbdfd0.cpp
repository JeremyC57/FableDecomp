// CVectorMap<T_16_00cbdfd0>::Sort() -- sort the vector map's elements with its Compare and clear the Dirty flag
// Leaf: ?Sort@CVectorMap_T_16_00cbdfd0@
// Renamed (fistp2, 2026-10-24) from CGuiVarTransferBool::TransferToGui @ 00/cb/CGuiVarTransferBool_TransferToGui_00cbdfd0.cpp: body = `sort(begin (+0), end (+4), Compare (+0xC)) ; Dirty (+0xD) = false` on the retail sorted-vector-map layout (GOTCHAS: CVectorMap = 12-byte vector + empty Compare at +0xC + bool Dirty at +0xD); the callee is labelled `_global::Sort_SElement16_00cbdef0` (element size 16); the CGuiVarTransferBool::TransferToGui label is propagated nonsense; element type not recoverable, method name descriptive
// CVectorMap_T_16_00cbdfd0::Sort  @ 0x00cbdfd0
// Forwards (f0 -> ecx, f4 -> edx, bool fc -> stack) to a fastcall sink,
// then clears the flag at +0xd.

void __fastcall CGuiVarTransferBool_sink(void *self, int val, bool flag);

struct CVectorMap_T_16_00cbdfd0
{
    void *f0;      // +0x00
    int   f4;      // +0x04
    char  pad8[4]; // +0x08
    bool  fc;      // +0x0c  bool value
    bool  fd;      // +0x0d  dirty flag

    void Sort();
};

void CVectorMap_T_16_00cbdfd0::Sort()
{
    CGuiVarTransferBool_sink(this->f0, this->f4, this->fc);
    this->fd = false;
}