// CCreatureAction_PlayConversationAnimation::Clone() -- new CCreatureAction_PlayConversationAnimation(*this) with the implicit copy constructor inlined (base copy @ 0x0062EBC0 + 2 dword members)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0084C490, oracle length 71 bytes.
// Grade: candidate -- still DIFFER(34v71) in the Linux lane (Wine + DDK 13.10.4035 cl.exe); generated for a
//        repeated body shape, see the batch README for the open question.
//
// Retail bytes:  push esi ; push edi ; push 0xb0 ; mov edi,ecx ; call operator new ; mov esi,eax ; add esp,4 ; test esi,esi ; je N ; push edi ; mov ecx,esi ; call CBase_0062ebc0::CBase_0062ebc0(const CBase_0062ebc0&) ; mov dword ptr [esi],offset CCreatureAction_PlayConversationAnimation::`vftable' ; mov al,[edi+0xa8] ; mov [esi+0xa8],al ; mov eax,[edi+0xac] ; mov [esi+0xac],eax ; pop edi ; mov eax,esi ; pop esi ; ret ; N: pop edi ; xor eax,eax ; pop esi ; ret
// Virtual clone of a class that adds 2 members to its base: the implicit copy constructor
// is inlined as the base copy constructor @ 0x0062EBC0, this class's vtable store (0x012737CC,
// rtti-vtables.tsv) and the member copies. sizeof = 0xb0. Member types are not recoverable
// beyond their size (`long` / `unsigned char`). One of 12 bodies of this shape.
struct CBase_0062ebc0 {
    virtual void v00();
    unsigned char _body[0xa8 - 4];     // sizeof(CBase_0062ebc0) = 0xa8
    CBase_0062ebc0(const CBase_0062ebc0& other);     // 0x0062EBC0
};

struct CCreatureAction_PlayConversationAnimation : CBase_0062ebc0 {
    unsigned char m_a8;                 // +0xa8
    unsigned char _gap1[0x3];
    long m_ac;                          // +0xac
    virtual void v00();                    // own vtable; copy constructor implicit
    CBase_0062ebc0* Clone();
};

CBase_0062ebc0* CCreatureAction_PlayConversationAnimation::Clone()
{
    return new CCreatureAction_PlayConversationAnimation(*this);
}
