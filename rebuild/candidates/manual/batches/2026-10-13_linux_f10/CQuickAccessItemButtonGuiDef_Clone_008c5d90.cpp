// CQuickAccessItemButtonGuiDef::Clone() -- new CQuickAccessItemButtonGuiDef(*this)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C5D90, oracle length 34 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x124 ; mov esi,ecx ; call operator new ; add esp,4 ; test eax,eax ; je N ; push esi ; mov ecx,eax ; call CQuickAccessItemButtonGuiDef::CQuickAccessItemButtonGuiDef(const CQuickAccessItemButtonGuiDef&) ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Virtual clone: allocates sizeof(CQuickAccessItemButtonGuiDef) = 0x124, null-checks and copy-constructs from
// `*this` through the out-of-line copy constructor @ 0x008C5DC0; the constructor's `this` return
// is the result. One of 104 bodies of this shape (the copy constructor's label names the class).
struct CCreatureActionBase {
    virtual void v00();
};

struct CQuickAccessItemButtonGuiDef : CCreatureActionBase {
    unsigned char _body[0x124 - 4];    // sizeof(CQuickAccessItemButtonGuiDef) = 0x124
    CQuickAccessItemButtonGuiDef(const CQuickAccessItemButtonGuiDef& other);             // 0x008C5DC0
    CCreatureActionBase* Clone();
};

CCreatureActionBase* CQuickAccessItemButtonGuiDef::Clone()
{
    return new CQuickAccessItemButtonGuiDef(*this);
}
