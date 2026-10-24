// CVectorMap<T_36_00993a10>::Sort() -- sort the vector map's elements with its Compare and clear the Dirty flag
// Leaf: ?Sort@CVectorMap_T_36_00993a10@
// Renamed (fistp2, 2026-10-24) from CGuiVarTransferBool::TransferToGui @ 00/99/CGuiVarTransferBool_TransferToGui_00993a10.cpp: body = `sort(begin (+0), end (+4), Compare (+0xC)) ; Dirty (+0xD) = false` on the retail sorted-vector-map layout (GOTCHAS: CVectorMap = 12-byte vector + empty Compare at +0xC + bool Dirty at +0xD); the callee is labelled `_global::Sort_SElement36_009939b0` (element size 36); the CGuiVarTransferBool::TransferToGui label is propagated nonsense; element type not recoverable, method name descriptive
// CVectorMap_T_36_00993a10::Sort  @ 0x00993a10
// __fastcall void (CVectorMap_T_36_00993a10* this)
//
// disasm:
//   push esi ; mov esi,ecx
//   mov edx,[esi+4]      ; second fastcall reg arg
//   mov ecx,[esi]        ; target object (this for the call)
//   xor eax,eax ; mov al,[esi+0xc] ; zero-extend bool
//   push eax            ; stack arg (bool)
//   call 0x9939b0       ; ((Target*)m_target)->Set(m_field, bool) [fastcall shape]
//   mov byte [esi+0xd],0
//   pop esi ; ret

// Callee: ecx=self, edx=arg2, one stack arg (bool) -> free __fastcall.
void __fastcall GuiSetBoolTarget(void* self, int arg2, bool value);

struct CVectorMap_T_36_00993a10
{
    void* m_target;        // +0x00
    int   m_field;         // +0x04
    int   m_pad8;          // +0x08
    bool  m_value;         // +0x0c
    bool  m_dirty;         // +0x0d

    void Sort();
};

void CVectorMap_T_36_00993a10::Sort()
{
    GuiSetBoolTarget(this->m_target, this->m_field, this->m_value);
    this->m_dirty = 0;
}