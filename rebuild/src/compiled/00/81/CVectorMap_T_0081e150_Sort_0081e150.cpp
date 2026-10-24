// CVectorMap<T_0081e150>::Sort() -- sort the vector map's elements with its Compare and clear the Dirty flag
// Leaf: ?Sort@CVectorMap_T_0081e150@
// Renamed (fistp2, 2026-10-24) from CGuiVarTransferBool::TransferToGui @ 00/81/CGuiVarTransferBool_TransferToGui_0081e150.cpp: body = `sort(begin (+0), end (+4), Compare (+0xC)) ; Dirty (+0xD) = false` on the retail sorted-vector-map layout (GOTCHAS: CVectorMap = 12-byte vector + empty Compare at +0xC + bool Dirty at +0xD); the callee is labelled `std::sort`; the CGuiVarTransferBool::TransferToGui label is propagated nonsense; element type not recoverable, method name descriptive
// CVectorMap_T_0081e150::Sort  @ 0x0081e150
// __fastcall void Sort(CVectorMap_T_0081e150* this)
//
// Disasm:
//   push esi
//   mov  esi, ecx
//   mov  edx, [esi+4]
//   mov  ecx, [esi]
//   xor  eax, eax
//   mov  al,  [esi+0xc]
//   push eax
//   call 0x81e030
//   mov  byte [esi+0xd], 0
//   pop  esi
//   ret
//
// The callee at 0x81e030 takes this=[esi] in ecx, a second pointer=[esi+4]
// in edx, and a zero-extended value from [esi+0xc] on the stack.  Modeling it
// as __fastcall(ecx, edx, stack) with an unsigned char param reproduces the
// retail xor eax,eax / mov al / push eax widening idiom exactly.

struct GuiObjA;
struct GuiObjB;

// helper @ 0x81e030 : __fastcall(A* self [ecx], B* other [edx], unsigned char val [stack])
void __fastcall CGuiVarTransferBool_helper(GuiObjA* self, GuiObjB* other, unsigned char val);

struct CVectorMap_T_0081e150
{
	GuiObjA*      m_pSource;   // +0x00 -> ecx
	GuiObjB*      m_pTarget;   // +0x04 -> edx
	char          pad8[4];     // +0x08
	signed char   m_value;     // +0x0c
	unsigned char m_dirty;     // +0x0d

	void Sort();
};

void CVectorMap_T_0081e150::Sort()
{
	CGuiVarTransferBool_helper(m_pSource, m_pTarget, m_value);
	m_dirty = 0;
}