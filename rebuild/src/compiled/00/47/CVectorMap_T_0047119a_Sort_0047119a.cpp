// CVectorMap<T_0047119a>::Sort() -- sort the vector map's elements with its Compare and clear the Dirty flag
// Leaf: ?Sort@CVectorMap_T_0047119a@
// Renamed (fistp2, 2026-10-24) from CGuiVarTransferBool::TransferToGui @ 00/47/CGuiVarTransferBool_TransferToGui_0047119a.cpp: body = `sort(begin (+0), end (+4), Compare (+0xC)) ; Dirty (+0xD) = false` on the retail sorted-vector-map layout (GOTCHAS: CVectorMap = 12-byte vector + empty Compare at +0xC + bool Dirty at +0xD); the callee is labelled `None`; the CGuiVarTransferBool::TransferToGui label is propagated nonsense; element type not recoverable, method name descriptive
// CVectorMap_T_0047119a::Sort @ 0047119a

// helper @ 0x4711b3: ecx=arg0, edx=arg1, stack=bool
extern void __fastcall helper_4711b3(void* ecx, void* edx, bool b);

struct CVectorMap_T_0047119a {
    void* f0;          // +0x00
    void* f4;          // +0x04
    char  pad8[4];     // +0x08
    bool  f0c;         // +0x0c
    unsigned char f0d; // +0x0d

    void Sort();
};

void CVectorMap_T_0047119a::Sort()
{
    helper_4711b3(this->f0, this->f4, this->f0c);
    this->f0d = 0;
}