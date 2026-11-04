// CClass_00c36230::sub_00c36230(CAmbientEmitter*) -- calls CAmbientEmitter::Update on the argument; this unused
// Vendor naming V2 (plan phase 1, one address, one name): 0x00C44E80 ?Update@CAmbientEmitter@@QAEXXZ (was ?Method_00c44e80@CObject_00c44e80@@QAEXXZ; the held row 0x00C44E80, the emitter's per-frame update)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C36230, oracle length 12 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  mov ecx,[esp+4] ; call CAmbientEmitter::Update ; ret 4
// Virtual forwarder: the one argument becomes `this` of a __fastcall call @ 0x00C44E80 with no
// further arguments; this object is not read. Callee names are kept from the manifest only when the
// label is unique. One of 11 bodies of this shape.
class CAmbientEmitter { public:
    void Update();                                  // 0x00C44E80
};

struct CClass_00c36230 {
    virtual void v00();
    virtual void sub_00c36230(CAmbientEmitter* object);
};

void CClass_00c36230::sub_00c36230(CAmbientEmitter* object)
{
    object->Update();
}