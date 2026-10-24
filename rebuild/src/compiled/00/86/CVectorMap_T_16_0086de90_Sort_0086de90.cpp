// CVectorMap<T_16_0086de90>::Sort() -- sort the vector map's elements with its Compare and clear the Dirty flag
// Leaf: ?Sort@CVectorMap_T_16_0086de90@
// Renamed (fistp2, 2026-10-24) from CGuiVarTransferBool::TransferToGui @ 00/86/CGuiVarTransferBool_TransferToGui_0086de90.cpp: body = `sort(begin (+0), end (+4), Compare (+0xC)) ; Dirty (+0xD) = false` on the retail sorted-vector-map layout (GOTCHAS: CVectorMap = 12-byte vector + empty Compare at +0xC + bool Dirty at +0xD); the callee is labelled `_global::Sort_SElement16_0086dd80` (element size 16); the CGuiVarTransferBool::TransferToGui label is propagated nonsense; element type not recoverable, method name descriptive
// CVectorMap_T_16_0086de90::Sort  @ 0086de90
// Retail disasm:
//   push esi; mov esi,ecx
//   mov edx,[esi+4]; mov ecx,[esi]
//   xor eax,eax; mov al,[esi+0xc]; push eax
//   call 0x86dd80
//   mov byte [esi+0xd],0
//   pop esi; ret
//
// The callee at 0x86dd80 receives this=[esi] in ecx, [esi+4] in edx and the
// zero-extended byte [esi+0xc] as a stack argument -> it is a __fastcall
// helper taking (target, value, bool).

extern void __fastcall CGuiVarTransferBool_setter(int target, int value, unsigned char flag);

struct CVectorMap_T_16_0086de90
{
    int           m_target;   // +0x00 -> ecx
    int           m_value;    // +0x04 -> edx
    int           m_pad08;    // +0x08
    unsigned char m_flag;     // +0x0c -> pushed byte
    unsigned char m_dirty;    // +0x0d -> cleared

    void Sort();
};

void CVectorMap_T_16_0086de90::Sort()
{
    CGuiVarTransferBool_setter(this->m_target, this->m_value, this->m_flag);
    this->m_dirty = 0;
}