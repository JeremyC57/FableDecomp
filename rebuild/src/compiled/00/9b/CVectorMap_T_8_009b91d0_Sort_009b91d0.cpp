// CVectorMap<T_8_009b91d0>::Sort() -- sort the vector map's elements with its Compare and clear the Dirty flag
// Leaf: ?Sort@CVectorMap_T_8_009b91d0@
// Renamed (fistp2, 2026-10-24) from CGuiVarTransferBool::TransferToGui @ 00/9b/CGuiVarTransferBool_TransferToGui_009b91d0.cpp: body = `sort(begin (+0), end (+4), Compare (+0xC)) ; Dirty (+0xD) = false` on the retail sorted-vector-map layout (GOTCHAS: CVectorMap = 12-byte vector + empty Compare at +0xC + bool Dirty at +0xD); the callee is labelled `_global::Sort_SElement8_009b8ef0` (element size 8); the CGuiVarTransferBool::TransferToGui label is propagated nonsense; element type not recoverable, method name descriptive
// CVectorMap_T_8_009b91d0::Sort  @ 0x009b91d0
// __fastcall void Sort(CVectorMap_T_8_009b91d0* this)
//
// Disasm:
//   push esi
//   mov esi, ecx            ; esi = this
//   mov edx, [esi+4]        ; edx = this->m_source
//   mov ecx, [esi]          ; ecx = this->m_target
//   xor eax, eax
//   mov al, [esi+0xc]       ; (int)(unsigned char) this->m_value
//   push eax
//   call 0x9b8ef0           ; __fastcall Apply(target, source, value)
//   mov byte [esi+0xd], 0   ; this->m_dirty = 0
//   pop esi
//   ret

struct CGuiVar;      // target object type (opaque)
struct CGuiVarSrc;   // source object type (opaque)

// Callee at 0x9b8ef0: ecx=target, edx=source, stack: bool value.
// Callee-cleanup (no add esp after) => __fastcall.
// A 'bool' value argument widens via 'xor eax,eax; mov al,...' (not movzx).
void __fastcall GuiVarBool_Apply(CGuiVar* target, CGuiVarSrc* source, bool value);

struct CVectorMap_T_8_009b91d0
{
    CGuiVar*    m_target;   // +0x00  -> ecx
    CGuiVarSrc* m_source;   // +0x04  -> edx
    int         m_pad8;     // +0x08
    bool        m_value;    // +0x0c  pushed value
    bool        m_dirty;    // +0x0d  cleared to false

    void Sort();
};

void CVectorMap_T_8_009b91d0::Sort()
{
    GuiVarBool_Apply(this->m_target, this->m_source, this->m_value);
    this->m_dirty = false;
}