// CVectorMap<T_8_004d2620>::Sort() -- sort the vector map's elements with its Compare and clear the Dirty flag
// Leaf: ?Sort@CVectorMap_T_8_004d2620@
// Renamed (fistp2, 2026-10-24) from CGuiVarTransferBool::TransferToGui @ 00/4d/CGuiVarTransferBool_TransferToGui_004d2620.cpp: body = `sort(begin (+0), end (+4), Compare (+0xC)) ; Dirty (+0xD) = false` on the retail sorted-vector-map layout (GOTCHAS: CVectorMap = 12-byte vector + empty Compare at +0xC + bool Dirty at +0xD); the callee is labelled `_global::Sort_SElement8_004d24f0` (element size 8); the CGuiVarTransferBool::TransferToGui label is propagated nonsense; element type not recoverable, method name descriptive
// CVectorMap_T_8_004d2620::Sort @ 004d2620
// helper @ 0x4d24f0: ecx=f0, edx=f4, stack=bool
extern void __fastcall helper_4d24f0(void* ecx, void* edx, bool b);

struct CVectorMap_T_8_004d2620 {
    void* f0;          // +0x00
    void* f4;          // +0x04
    char  pad8[4];     // +0x08
    bool  f0c;         // +0x0c
    unsigned char f0d; // +0x0d

    void Sort();
};

void CVectorMap_T_8_004d2620::Sort()
{
    helper_4d24f0(this->f0, this->f4, this->f0c);
    this->f0d = 0;
}