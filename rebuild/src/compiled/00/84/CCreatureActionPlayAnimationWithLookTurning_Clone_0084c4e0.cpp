// CCreatureAction_PlayAnimationWithLookTurning::Clone() -- new CCreatureAction_PlayAnimationWithLookTurning(*this) with the implicit copy constructor inlined
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084C4E0, oracle length 47 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push edi ; push 0xb4 ; mov edi,ecx ; call operator new ; mov esi,eax ; add esp,4 ; test esi,esi ; je N ; push edi ; mov ecx,esi ; call CCreatureAction_PlayAnimation::CCreatureAction_PlayAnimation(const CCreatureAction_PlayAnimation&) ; pop edi ; mov dword ptr [esi],offset CCreatureAction_PlayAnimationWithLookTurning::`vftable' ; mov eax,esi ; pop esi ; ret ; N: pop edi ; xor eax,eax ; pop esi ; ret
// Virtual clone of a class that adds no data to its base: the implicit copy constructor is
// inlined as the base copy constructor @ 0x0084C340 plus this class's vtable store
// (0x012740F4, named through rtti-vtables.tsv). sizeof = 0xb4. One of 196 bodies of this shape.
struct CCreatureAction_PlayAnimation {
    virtual void v00();
    unsigned char _body[0xb4 - 4];    // sizeof(CCreatureAction_PlayAnimation) = sizeof(CCreatureAction_PlayAnimationWithLookTurning) = 0xb4
    CCreatureAction_PlayAnimation(const CCreatureAction_PlayAnimation& other);     // 0x0084C340
};

struct CCreatureAction_PlayAnimationWithLookTurning : CCreatureAction_PlayAnimation {
    virtual void v00();                    // own vtable; copy constructor implicit
    CCreatureAction_PlayAnimation* Clone();
};

CCreatureAction_PlayAnimation* CCreatureAction_PlayAnimationWithLookTurning::Clone()
{
    return new CCreatureAction_PlayAnimationWithLookTurning(*this);
}