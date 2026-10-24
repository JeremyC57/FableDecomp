// CVectorMap<T_8_00a42040>::Sort() -- sort the vector map's elements with its Compare and clear the Dirty flag
// Leaf: ?Sort@CVectorMap_T_8_00a42040@
// Renamed (fistp2, 2026-10-24) from CGuiVarTransferBool::TransferToGui @ 00/a4/CGuiVarTransferBool_TransferToGui_00a42040.cpp: body = `sort(begin (+0), end (+4), Compare (+0xC)) ; Dirty (+0xD) = false` on the retail sorted-vector-map layout (GOTCHAS: CVectorMap = 12-byte vector + empty Compare at +0xC + bool Dirty at +0xD); the callee is labelled `_global::Sort_SElement8_00a41e20` (element size 8); the CGuiVarTransferBool::TransferToGui label is propagated nonsense; element type not recoverable, method name descriptive
// CVectorMap_T_8_00a42040::Sort  @ 0x00a42040
// Layout reconstructed from disasm:
//   [esi+0]   target object pointer (ecx for the call)
//   [esi+4]   handle/id value (edx for the call)
//   [esi+0xc] bool value being transferred (stack arg)
//   [esi+0xd] "changed" flag, cleared after transfer

// The callee at 0xa41e20 receives ecx = obj, edx = handle, and a pushed bool.
// That is a __fastcall(obj, handle, value) signature.
extern void __fastcall GuiVarSetBool(void* obj, int handle, bool value);

struct CVectorMap_T_8_00a42040
{
    void* target;   // +0x00
    int   handle;   // +0x04
    int   pad8;     // +0x08
    bool  value;    // +0x0c
    bool  changed;  // +0x0d

    void Sort();
};

void CVectorMap_T_8_00a42040::Sort()
{
    GuiVarSetBool(this->target, this->handle, this->value);
    this->changed = false;
}