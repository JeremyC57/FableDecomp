// CUnprovenObject_00c3d8b0::`vector deleting destructor'(unsigned int flags) -- destroy one object or an array of 0x1bc-byte objects, freeing through the class operator delete when flags bit 0 is set
// Leaf: ??_ECUnprovenObject_00c3d8b0@@UAEPAXI@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00c3d8b0, 87 bytes, thiscall, `ret 4`.
// Identity (descriptive): slot 0 of the non-RTTI vtable 0x012B7B88 (stored by 0x00C44580; bound ??_7CAIStateGroup_Derived_00c44580@@6B@, a stand-in); element size 0x1BC; its destructor 0x00C3D0D0 is landed under the propagated name ??1CAIStateGroup_CreatureUseCoverBase@ (which 0x00C3D0E0 also carries). libpk: no PDB, no RTTI -- the class is named by this address.
// Callees: `eh vector destructor iterator' 0x00BFEED8 (??_M, with the element size 0x1bc and the destructor), the
//   destructor 0x00C3D0D0, the class operator delete / delete[] -- both the audio free 0x00C3C370 (inline forwarders).
// Shape (family L127): the compiler-generated vector deleting destructor; VC7.1 emits it (rather than an alias of the
//   scalar ??_G) only when the TU holds an array new of the class, hence the anchor below.
void __cdecl AudioFree(void* block);                   // 0x00C3C370
class CUnprovenObject_00c3d8b0 {                          // vtable 0x012B7B88
public:
    CUnprovenObject_00c3d8b0();
    virtual ~CUnprovenObject_00c3d8b0();                   // 0x00C3D0D0
    static void operator delete(void* p) { AudioFree(p); }
    static void operator delete[](void* p) { AudioFree(p); }
    unsigned char _body[0x1bc - 4];
};
CUnprovenObject_00c3d8b0::CUnprovenObject_00c3d8b0()
{
}
// An array new in the TU makes VC7.1 emit the vector deleting destructor (??_E) for the vtable.
CUnprovenObject_00c3d8b0* NewArray_00c3d8b0(unsigned int count)
{
    return new CUnprovenObject_00c3d8b0[count];
}