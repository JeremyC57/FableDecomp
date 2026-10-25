// CCountedPointer<CEntityScriptBindingBase>::DeleteData(void* data) -- the counted-pointer deleter for script entity bindings: free the binding with the scalar operator delete
// Leaf: ?DeleteData@?$CCountedPointer@VCEntityScriptBindingBase@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB8D30, 8 bytes (+ int3 padding): `push ecx ; call 0x00BFE9BC ; pop ecx ; ret` -- a static
//   __fastcall member (data in ecx) in a size-optimised TU (`pop ecx` cleans the cdecl argument).
// Fix (sweep1, 2026-10-25; FINDINGS chunk 451 "0xCB8D30 models a call to helper_bfeb1c as operator_new but retail calls 0xBFE9BC
//   operator delete"): the landed source was the 0x004C73B0 row's text (`operator_new` forwarding to 0x00BFEB1C, the delete[]
//   import), so its only relocation pointed at the wrong CRT function, and its test printed the pass line unconditionally.
//   Rewritten as the CCountedPointer family's deleter (`delete (T*)data;`, as the landed CCountedPointer<CMusicManager>::
//   DeleteData 0x004AE510 and Ego_r.pdb's `?DeleteData@?$CCountedPointer@V...@@@@SIXPAX@Z` rows): the deleter address is stored
//   into a count block at 0x00CB825C (CScriptBase::AddEntityScriptBinding) and 0x00CB9656.
// Model: retail runs no destructor and no null test, so the binding type is modelled trivially destructible (no Ego_r layout
//   or destructor symbol exists for CEntityScriptBindingBase).
#pragma optimize("s", on)

void __cdecl operator delete(void* block);         // 0x00BFE9BC

class CEntityScriptBindingBase {
};

template <class T> class CCountedPointer {
public:
    static void __fastcall DeleteData(void* data) { delete (T*)data; }
};

template class CCountedPointer<CEntityScriptBindingBase>;
