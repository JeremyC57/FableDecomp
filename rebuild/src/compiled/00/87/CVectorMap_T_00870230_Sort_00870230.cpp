// CVectorMap<T_00870230>::Sort() -- sort the vector map's elements with its Compare and clear the Dirty flag
// Leaf: ?Sort@CVectorMap_T_00870230@
// Renamed (fistp2, 2026-10-24) from CGuiVarTransferBool::TransferToGui @ 00/87/CGuiVarTransferBool_TransferToGui_00870230.cpp: body = `sort(begin (+0), end (+4), Compare (+0xC)) ; Dirty (+0xD) = false` on the retail sorted-vector-map layout (GOTCHAS: CVectorMap = 12-byte vector + empty Compare at +0xC + bool Dirty at +0xD); the callee is labelled `std::sort`; the CGuiVarTransferBool::TransferToGui label is propagated nonsense; element type not recoverable, method name descriptive
// CVectorMap_T_00870230::Sort  (retail 0x00870230)

struct CGuiTarget;

// callee at 0x8701d0: __fastcall(this=ecx, arg2=edx, bool byte on stack)
void __fastcall GuiSetBool(CGuiTarget* self, void* edx, unsigned char v);

struct CVectorMap_T_00870230
{
    CGuiTarget* target;   // +0x00
    void*       cookie;   // +0x04
    int         pad08;    // +0x08
    unsigned char value;  // +0x0c
    unsigned char dirty;  // +0x0d

    void Sort();
};

void CVectorMap_T_00870230::Sort()
{
    GuiSetBool(this->target, this->cookie, this->value);
    this->dirty = 0;
}