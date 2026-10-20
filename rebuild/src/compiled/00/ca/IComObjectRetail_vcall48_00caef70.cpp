// IComObject_Retail::vcall{48}(IComObject_Retail* object) -- compiler-generated vcall thunk (`??_9@$BDA@AG`, `[thunk]: __stdcall 'vcall'{48,{flat}}`): the target of a pointer to a __stdcall virtual member in slot 12 (+0x30), forwarding the stack `this` to the object's vtable slot
// Leaf: ??_9@$BDA@AG
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CAEF70, length 9 bytes (16 with padding). Caller 0x00CAF0C7 (the DirectShow
// base-class region). The catalogue row 00caef70 (51 bytes) held this body and three `return this` accessors at
// 0x00CAEF80 / 0x00CAEF90 / 0x00CAEFA0 (skipped ledger, chunk 904); the class label "CThingFilter_IsValid" is propagated
// and wrong: the body is byte-identical to the vcall thunk VC7.1 emits for `&IComObject_Retail::vfunc_12` (a __stdcall
// virtual: `this` on the stack), which no hand-written wrapper reproduces (a __stdcall free function or static member
// re-stores the object into the argument slot before the tail jump, 11-13 bytes; seven forms tried).
// Shape: `mov eax,[esp+4] ; mov eax,[eax] ; jmp [eax+30h]` -- the thunk reads `this` from its only stack argument and
// tail-jumps to vtable slot 12, leaving the argument slot in place for the __stdcall target.
// Source: a pointer-to-member-function of the __stdcall interface method anywhere in the TU makes the compiler emit the thunk
// (the interface itself is a 13-slot COM-style vtable; only slot 12 is named).
struct IComObject_Retail {
    virtual long __stdcall v00();
    virtual long __stdcall v01();
    virtual long __stdcall v02();
    virtual long __stdcall v03();
    virtual long __stdcall v04();
    virtual long __stdcall v05();
    virtual long __stdcall v06();
    virtual long __stdcall v07();
    virtual long __stdcall v08();
    virtual long __stdcall v09();
    virtual long __stdcall v10();
    virtual long __stdcall v11();
    virtual long __stdcall vfunc_12();         // slot 12 (+0x30)
};
typedef long (__stdcall IComObject_Retail::*PFN_IComObject_vfunc_12)();
PFN_IComObject_vfunc_12 g_pfnIComObject_vfunc_12 = &IComObject_Retail::vfunc_12;   // instantiates ??_9@$BDA@AG