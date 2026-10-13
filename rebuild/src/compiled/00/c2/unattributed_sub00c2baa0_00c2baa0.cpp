// CClass_00c2baa0::sub_00c2baa0(CTextSlider*) -- calls CTextSlider::OnDeactivate on the argument; this unused
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2BAA0, oracle length 12 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov ecx,[esp+4] ; call CTextSlider::OnDeactivate ; ret 4
// Virtual forwarder: the one argument becomes `this` of a __fastcall call @ 0x00C37BF0 with no
// further arguments; this object is not read. Callee names are kept from the manifest only when the
// label is unique. One of 11 bodies of this shape.
struct CTextSlider {
    void OnDeactivate();                         // 0x00C37BF0
};

struct CClass_00c2baa0 {
    virtual void v00();
    virtual void sub_00c2baa0(CTextSlider* object);
};

void CClass_00c2baa0::sub_00c2baa0(CTextSlider* object)
{
    object->OnDeactivate();
}