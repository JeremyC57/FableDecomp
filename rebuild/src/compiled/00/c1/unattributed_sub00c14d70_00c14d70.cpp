// CClass_00c14d70::sub_00c14d70(CObject_00c2cb80*) -- calls CObject_00c2cb80::Method_00c2cb80 on the argument; this unused
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C14D70, oracle length 12 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov ecx,[esp+4] ; call CObject_00c2cb80::Method_00c2cb80 ; ret 4
// Virtual forwarder: the one argument becomes `this` of a __fastcall call @ 0x00C2CB80 with no
// further arguments; this object is not read. Callee names are kept from the manifest only when the
// label is unique. One of 11 bodies of this shape.
struct CObject_00c2cb80 {
    void Method_00c2cb80();                         // 0x00C2CB80
};

struct CClass_00c14d70 {
    virtual void v00();
    virtual void sub_00c14d70(CObject_00c2cb80* object);
};

void CClass_00c14d70::sub_00c14d70(CObject_00c2cb80* object)
{
    object->Method_00c2cb80();
}