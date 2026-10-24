// CVectorMap<T_00befc80>::Sort() -- sort the vector map's elements with its Compare and clear the Dirty flag
// Leaf: ?Sort@CVectorMap_T_00befc80@
// Renamed (fistp2, 2026-10-24) from CGuiVarTransferBool::TransferToGui @ 00/be/CGuiVarTransferBool_TransferToGui_00befc80.cpp: body = `sort(begin (+0), end (+4), Compare (+0xC)) ; Dirty (+0xD) = false` on the retail sorted-vector-map layout (GOTCHAS: CVectorMap = 12-byte vector + empty Compare at +0xC + bool Dirty at +0xD); the callee is labelled `None`; the CGuiVarTransferBool::TransferToGui label is propagated nonsense; element type not recoverable, method name descriptive
// CVectorMap_T_00befc80::Sort  (retail 0x00befc80)
// __fastcall void Sort(CVectorMap_T_00befc80* this)
//
// esi=this
//   edx = [this+4]
//   ecx = [this+0]        -> callee's 'this' (fastcall arg1)
//   al  = (unsigned char)[this+0xc], pushed as stack arg
//   call 0x00befc20  (fastcall: arg1=ecx=[this+0], arg2=edx=[this+4], stack byte)
//   [this+0xd] = 0
//   ret

struct CGuiVarBoolTarget;

// callee at 0x00befc20: fastcall member-style helper taking (self, edx-arg, byte)
void __fastcall CGuiVarBool_Apply(CGuiVarBoolTarget* self, int arg2, unsigned char val);

struct CVectorMap_T_00befc80
{
    CGuiVarBoolTarget* m_pTarget;  // +0x00
    int                m_arg;      // +0x04
    int                m_pad08;    // +0x08
    unsigned char      m_value;    // +0x0c
    unsigned char      m_dirty;    // +0x0d

    void Sort();
};

void CVectorMap_T_00befc80::Sort()
{
    CGuiVarBool_Apply(this->m_pTarget, this->m_arg, this->m_value);
    this->m_dirty = 0;
}