// CClass_00c2a450::sub_00c2a450(CObject_00c377a0*) -- calls CObject_00c377a0::Method_00c377a0 on the argument; this unused
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2A450, oracle length 12 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov ecx,[esp+4] ; call CObject_00c377a0::Method_00c377a0 ; ret 4
// Virtual forwarder: the one argument becomes `this` of a __thiscall call @ 0x00C377A0 with no
// further arguments; this object is not read. Callee names are kept from the manifest only when the
// label is unique. One of 11 bodies of this shape.
struct CObject_00c377a0 {
    void Method_00c377a0();                         // 0x00C377A0
};

struct CClass_00c2a450 {
    virtual void v00();
    virtual void sub_00c2a450(CObject_00c377a0* object);
};

void CClass_00c2a450::sub_00c2a450(CObject_00c377a0* object)
{
    object->Method_00c377a0();
}
