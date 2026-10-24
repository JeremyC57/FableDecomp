// <unattributed>::ForwardMember10ToSubobject() -- Owner(+0x04)->Sub(+0x70)->Method(&this->+0x10)
// Leaf: AddLearnedExpression
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F53A0, oracle length 16 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
// The manifest label for this body is propagated across byte-identical bodies and is NOT trusted;
// the class is modelled from the bytes only and the name below describes what the code does.
//
// Retail bytes:  lea eax,[ecx+10h] ; mov ecx,[ecx+4] ; mov ecx,[ecx+70h] ; push eax ; call Method_007f5560 ; ret
// The address of the member at +0x10 is passed to a one-argument __fastcall method @ 0x007F5560 on
// the object reached through +0x04 then +0x70; the result is returned.
struct CSubobject_007f5560 {
    bool Method_007f5560(void* member);     // 0x007F5560
};

struct COwnerWithSub70 {
    unsigned char       _pad_0x00[0x70];
    CSubobject_007f5560* Sub;               // +0x70
};

struct CForwarder_Retail {
    void*            __vftable;             // +0x00
    COwnerWithSub70* Owner;                 // +0x04
    unsigned char    _pad_0x08[0x08];
    unsigned char    Member[4];             // +0x10
    bool ForwardMember10ToSubobject();
};

bool CForwarder_Retail::ForwardMember10ToSubobject()
{
    return this->Owner->Sub->Method_007f5560(&this->Member[0]);
}