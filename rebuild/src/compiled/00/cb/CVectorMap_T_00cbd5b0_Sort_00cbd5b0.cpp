// CVectorMap<T_00cbd5b0>::Sort() -- sort the vector map's elements with its Compare and clear the Dirty flag
// Leaf: ?Sort@CVectorMap_T_00cbd5b0@
// Renamed (fistp2, 2026-10-24) from CGuiVarTransferBool::TransferToGui @ 00/cb/CGuiVarTransferBool_TransferToGui_00cbd5b0.cpp: body = `sort(begin (+0), end (+4), Compare (+0xC)) ; Dirty (+0xD) = false` on the retail sorted-vector-map layout (GOTCHAS: CVectorMap = 12-byte vector + empty Compare at +0xC + bool Dirty at +0xD); the callee is labelled `None`; the CGuiVarTransferBool::TransferToGui label is propagated nonsense; element type not recoverable, method name descriptive
// CVectorMap_T_00cbd5b0::Sort @ 0x00cbd5b0
// __fastcall void Sort(CVectorMap_T_00cbd5b0* this)

// Callee at 0xcbd370: ecx=[this+0], edx=[this+4], stack bool arg.
// Modeled as a __fastcall free function (ecx=self, edx=arg2, stack=value).
void __fastcall GuiVarBool_Apply(void* self, int arg2, bool value);

struct CVectorMap_T_00cbd5b0
{
    void*         pTarget;   // +0x00  -> ecx of callee
    int           arg2;      // +0x04  -> edx of callee
    int           pad8;      // +0x08
    bool          value;     // +0x0c  -> pushed (xor eax; mov al)
    unsigned char dirty;     // +0x0d  -> cleared to 0

    void Sort();
};

void CVectorMap_T_00cbd5b0::Sort()
{
    GuiVarBool_Apply(this->pTarget, this->arg2, this->value);
    this->dirty = 0;
}