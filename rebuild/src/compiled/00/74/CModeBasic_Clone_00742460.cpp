// CModeBasic::Clone() -- new CModeBasic(*this) with the implicit copy constructor inlined
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00742460, oracle length 44 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push edi ; push 0x1c ; mov edi,ecx ; call operator new ; mov esi,eax ; add esp,4 ; test esi,esi ; je N ; push edi ; mov ecx,esi ; call CModeBase::CModeBase(const CModeBase&) ; pop edi ; mov dword ptr [esi],offset CModeBasic::`vftable' ; mov eax,esi ; pop esi ; ret ; N: pop edi ; xor eax,eax ; pop esi ; ret
// Virtual clone of a class that adds no data to its base: the implicit copy constructor is
// inlined as the base copy constructor @ 0x0073EAC0 plus this class's vtable store
// (0x012675BC, named through rtti-vtables.tsv). sizeof = 0x1c. One of 28 bodies of this shape.
struct CModeBase {
    virtual void v00();
    unsigned char _body[0x1c - 4];    // sizeof(CModeBase) = sizeof(CModeBasic) = 0x1c
    CModeBase(const CModeBase& other);     // 0x0073EAC0
};

struct CModeBasic : CModeBase {
    virtual void v00();                    // own vtable; copy constructor implicit
    CModeBase* Clone();
};

CModeBase* CModeBasic::Clone()
{
    return new CModeBasic(*this);
}