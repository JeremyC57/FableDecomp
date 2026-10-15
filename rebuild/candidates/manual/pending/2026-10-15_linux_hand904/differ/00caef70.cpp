// CThingFilter_IsValid::func_00caef70(IComObject* object) -- forward a COM-style object to its __stdcall vtable slot 12
// Grade: DIFFER(13v9) -- the lane re-stores the object into its own argument slot before the tail jump (`mov [esp+4],eax`) and fetches the vtable into ecx; retail reads the vtable through eax and jumps with the slot untouched. Seven forms tried (__stdcall / __cdecl free function, static member, __stdcall member with the pointer member, C-style COM tables with typed and void* entries, the size pragma): all 11v9 or 13v9. Lane gap: argument-slot reuse for a tail-called __stdcall method.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CAEF70, length 9 bytes (16 with padding). Caller 0x00CAF0C7 (the DirectShow
// base-class region). The catalogue row 00caef70 (51 bytes) held this body and three `return this` accessors at
// 0x00CAEF80 / 0x00CAEF90 / 0x00CAEFA0 (skipped ledger, chunk 904); the class label is propagated.
// Shape: `mov eax,[esp+4] ; mov eax,[eax] ; jmp [eax+30h]` -- a __stdcall free function whose only argument is the
// interface pointer, tail-jumping to the __stdcall method (which takes `this` on the stack in the same slot).
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

long __stdcall func_00caef70(IComObject_Retail* object)
{
    return object->vfunc_12();
}
