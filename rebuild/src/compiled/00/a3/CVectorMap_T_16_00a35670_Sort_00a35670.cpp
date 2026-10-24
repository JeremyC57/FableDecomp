// CVectorMap<T_16_00a35670>::Sort() -- sort the vector map's elements with its Compare and clear the Dirty flag
// Leaf: ?Sort@CVectorMap_T_16_00a35670@
// Renamed (fistp2, 2026-10-24) from CGuiVarTransferBool::TransferToGui @ 00/a3/CGuiVarTransferBool_TransferToGui_00a35670.cpp: body = `sort(begin (+0), end (+4), Compare (+0xC)) ; Dirty (+0xD) = false` on the retail sorted-vector-map layout (GOTCHAS: CVectorMap = 12-byte vector + empty Compare at +0xC + bool Dirty at +0xD); the callee is labelled `_global::Sort_SElement16_00a355f0` (element size 16); the CGuiVarTransferBool::TransferToGui label is propagated nonsense; element type not recoverable, method name descriptive
// CVectorMap_T_16_00a35670::Sort  @ 0x00a35670  (__fastcall)

struct CVectorMap_T_16_00a35670
{
    void*         field_0;   // +0x00  (ecx to callee)
    int           field_4;   // +0x04  (edx to callee)
    int           pad_8;     // +0x08
    unsigned char field_C;   // +0x0c  (pushed arg)
    unsigned char field_D;   // +0x0d  (cleared)

    void Sort();
};

// callee at 0x00a355f0: __fastcall taking (ecx, edx, byte-on-stack)
extern void __fastcall CGuiVarTransferBool_Emit(void* a, int b, unsigned char c);

void CVectorMap_T_16_00a35670::Sort()
{
    CGuiVarTransferBool_Emit(this->field_0, this->field_4, this->field_C);
    this->field_D = 0;
}