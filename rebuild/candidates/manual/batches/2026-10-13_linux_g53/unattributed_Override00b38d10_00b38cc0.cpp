// [thunk] CMulti_00b38d10::Override_00b38d10 `adjustor{616}' -- second-base override entry: this -= 616, jump to CMulti_00b38d10::Override_00b38d10 @ 0x00B38D10
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B38CC0, oracle length 11 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  sub ecx,616 ; jmp CMulti_00b38d10::Override_00b38d10
// Compiler-generated adjustor thunk: CMulti_00b38d10 inherits two polymorphic bases that both declare the
// virtual; the override sits in the primary vtable and the SECOND base's vtable (at +616) gets this thunk; calls through that base's vtable arrive with `this` pointing at
// the base subobject, so the thunk rebases `this` and tail-jumps to the real override
// @ 0x00B38D10 (named from the manifest when its label is unique). The override itself is a
// separate catalog row; the empty body here only makes the thunk exist. One of 18 bodies.
// Both bases declare the virtual: the override lands directly in the primary (first-base) vtable
// and the second base's vtable gets this adjustor thunk.
struct CFirstBase {
    virtual void Override_00b38d10();
    unsigned char _body[0x264];           // the second base sits at +0x268
};

struct CSecondBase {
    virtual void Override_00b38d10();
};

struct CMulti_00b38d10 : CFirstBase, CSecondBase {
    virtual void Override_00b38d10();
};

void CMulti_00b38d10::Override_00b38d10()
{
}

// The vtables (and with them the adjustor thunk) are emitted in a translation unit that
// constructs the object; this helper is that construction, nothing more. The lander grades the
// largest symbol carrying the method name: the 8-byte thunk, not the 1-byte empty override.
void* MakeOne()
{
    return new CMulti_00b38d10;
}
