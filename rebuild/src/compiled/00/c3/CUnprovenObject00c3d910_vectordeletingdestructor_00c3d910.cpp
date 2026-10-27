// CUnprovenObject_00c3d910::`vector deleting destructor'(unsigned int flags) -- destroy one object or an array of 0x1dc-byte objects, freeing through the class operator delete when flags bit 0 is set
// Leaf: ??_ECUnprovenObject_00c3d910@@UAEPAXI@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00c3d910, 87 bytes, thiscall, `ret 4`.
// Identity (descriptive): slot 0 of the non-RTTI vtable 0x012B5AB0 (stored by 0x00C3D97A and the factory 0x00C3DC84; bound ??_7C3DAnimationUtil@@6B@, a stand-in); element size 0x1DC; its destructor 0x00C3D0E0 is landed under the same propagated name as 0x00C3D0D0. libpk: no PDB, no RTTI -- the class is named by this address.
// Callees: `eh vector destructor iterator' 0x00BFEED8 (??_M, with the element size 0x1dc and the destructor), the
//   destructor 0x00C3D0E0, the class operator delete / delete[] -- both the audio free 0x00C3C370 (inline forwarders).
// Shape (family L127): the compiler-generated vector deleting destructor; VC7.1 emits it (rather than an alias of the
//   scalar ??_G) only when the TU holds an array new of the class, hence the anchor below.
void __cdecl AudioFree(void* block);                   // 0x00C3C370
class CUnprovenObject_00c3d910 {                          // vtable 0x012B5AB0
public:
    CUnprovenObject_00c3d910();
    virtual ~CUnprovenObject_00c3d910();                   // 0x00C3D0E0
    static void operator delete(void* p) { AudioFree(p); }
    static void operator delete[](void* p) { AudioFree(p); }
    unsigned char _body[0x1dc - 4];
};
CUnprovenObject_00c3d910::CUnprovenObject_00c3d910()
{
}
// An array new in the TU makes VC7.1 emit the vector deleting destructor (??_E) for the vtable.
CUnprovenObject_00c3d910* NewArray_00c3d910(unsigned int count)
{
    return new CUnprovenObject_00c3d910[count];
}