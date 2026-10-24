// CVectorMap<T_8_0088d1b0>::Sort() -- sort the vector map's elements with its Compare and clear the Dirty flag
// Leaf: ?Sort@CVectorMap_T_8_0088d1b0@
// Renamed (fistp2, 2026-10-24) from CGuiVarTransferBool::TransferToGui @ 00/88/CGuiVarTransferBool_TransferToGui_0088d1b0.cpp: body = `sort(begin (+0), end (+4), Compare (+0xC)) ; Dirty (+0xD) = false` on the retail sorted-vector-map layout (GOTCHAS: CVectorMap = 12-byte vector + empty Compare at +0xC + bool Dirty at +0xD); the callee is labelled `_global::Sort_SElement8_0088d130` (element size 8); the CGuiVarTransferBool::TransferToGui label is propagated nonsense; element type not recoverable, method name descriptive
// CVectorMap_T_8_0088d1b0::Sort  @ 0x0088d1b0
// Retail disasm:
//   push esi; mov esi,ecx
//   mov edx,[esi+4]; mov ecx,[esi]
//   xor eax,eax; mov al,[esi+0xc]; push eax
//   call 0x88d130
//   mov byte [esi+0xd],0
//   pop esi; ret

// The callee takes ecx=[esi], edx=[esi+4], and one stack arg (a bool).
// Model it as a __fastcall(ecx, edx, stackarg).
extern "C" void __fastcall CGuiVarTransferBool_Setter(void* pThis, void* edxVal, unsigned char value);

struct CVectorMap_T_8_0088d1b0
{
    void*         field_0;   // +0x00  -> ecx for callee
    void*         field_4;   // +0x04  -> edx for callee
    int           field_8;   // +0x08  (padding/other)
    unsigned char field_C;   // +0x0C  bool value read
    unsigned char field_D;   // +0x0D  byte cleared to 0

    void Sort();
};

void CVectorMap_T_8_0088d1b0::Sort()
{
    CGuiVarTransferBool_Setter(this->field_0, this->field_4, this->field_C);
    this->field_D = 0;
}