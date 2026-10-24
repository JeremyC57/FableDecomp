// CVectorMap<T_009b94f0>::Sort() -- sort the vector map's elements with its Compare and clear the Dirty flag
// Leaf: ?Sort@CVectorMap_T_009b94f0@
// Renamed (fistp2, 2026-10-24) from CGuiVarTransferBool::TransferToGui @ 00/9b/CGuiVarTransferBool_TransferToGui_009b94f0.cpp: body = `sort(begin (+0), end (+4), Compare (+0xC)) ; Dirty (+0xD) = false` on the retail sorted-vector-map layout (GOTCHAS: CVectorMap = 12-byte vector + empty Compare at +0xC + bool Dirty at +0xD); the callee is labelled `std::sort<ClassInfoPair24>`; the CGuiVarTransferBool::TransferToGui label is propagated nonsense; element type not recoverable, method name descriptive
// CVectorMap_T_009b94f0::Sort  @ 009b94f0

// The call at 0x9b9490 receives ecx and edx explicitly set plus one stack arg:
//   ecx = self->m_pFirst, edx = self->m_pSecond, stack = (unsigned char)self->m_value
// That is a __fastcall free function with two register params and a stack param.
void __fastcall GuiVarSetBool(void* pFirst, int pSecond, unsigned char v);

struct CVectorMap_T_009b94f0
{
    void*         m_pFirst;   // +0x00
    int           m_pSecond;  // +0x04
    // +0x08 padding/other
    int           m_pad08;    // +0x08
    unsigned char m_value;    // +0x0c
    unsigned char m_flag;     // +0x0d

    void Sort();
};

void CVectorMap_T_009b94f0::Sort()
{
    GuiVarSetBool(this->m_pFirst, this->m_pSecond, this->m_value);
    this->m_flag = 0;
}