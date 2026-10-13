// CClass_00c37970::sub_00c37970(CObject_00c47300*) -- calls CObject_00c47300::CTCOnHeroPush_ProcessStateTransition on the argument; this unused
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C37970, oracle length 12 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov ecx,[esp+4] ; call CObject_00c47300::CTCOnHeroPush_ProcessStateTransition ; ret 4
// Virtual forwarder: the one argument becomes `this` of a __fastcall call @ 0x00C47300 with no
// further arguments; this object is not read. Callee names are kept from the manifest only when the
// label is unique. One of 11 bodies of this shape.
struct CObject_00c47300 {
    void CTCOnHeroPush_ProcessStateTransition();                         // 0x00C47300
};

struct CClass_00c37970 {
    virtual void v00();
    virtual void sub_00c37970(CObject_00c47300* object);
};

void CClass_00c37970::sub_00c37970(CObject_00c47300* object)
{
    object->CTCOnHeroPush_ProcessStateTransition();
}