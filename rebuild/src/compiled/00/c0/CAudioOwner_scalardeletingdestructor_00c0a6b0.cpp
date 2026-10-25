// CAudioOwner::`scalar deleting destructor'(unsigned int flags) -- run ~CAudioOwner (0x00C09380), free the object when bit 0 of the flags is set, return it
// Leaf: ??_GCAudioOwner@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C0A6B0, 30 bytes: `push esi ; mov esi,ecx ; call 0x00C09380 ; test byte [esp+8],1 ;
//   je ; push esi ; call operator delete 0x00BFE9BC ; add esp,4 ; mov eax,esi ; pop esi ; ret 4`.
// Naming (sweep1, 2026-10-25): was landed as CVtable_012b1e24 / a global `__fastcall`
//   CLandscapeBackgroundPatch_vector_deleting_destructor(self, edx_unused, flags) stand-in (the manifest's
//   CLandscapeBackgroundPatch::`vector deleting destructor' is propagated); descriptive (no PDB covers this library).
//   It is slot 14 of vtable 0x012B1E24 (xref .rdata 0x012B1E5C), the vtable stored only by the audio owner's
//   constructor 0x00C094A0 and destructor 0x00C09380 (chunk 431's CAudioOwner, renamed this pass); slot 0 is the owner's
//   pump 0x00C09270, and the interface base vtable 0x012B1A64 has `_purecall` in slots 0-13 and its own deleting
//   destructor 0x00C09780 in slot 14.
// Shape: the genuine compiler-generated ??_G (the stand-in's dummy edx is gone): a class whose virtual destructor
//   (declared out of line, 0x00C09380) is slot 14, and an inline constructor used by a `new`, so that the vtable and
//   with it ??_G are emitted in this object.
void __cdecl operator delete(void* block);       // 0x00BFE9BC

struct CAudioOwner {                             // vtable 0x012B1E24
    virtual void v00();                          // slot 0: 0x00C09270
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void v11(); virtual void v12(); virtual void v13();
    virtual ~CAudioOwner();                      // slot 14 (0x00C09380)
    CAudioOwner() { }
};

CAudioOwner* CreateAudioOwner_00c0a6b0()
{
    return new CAudioOwner;
}
