// CAbilityTrackLevelDef::Clone() -- new CAbilityTrackLevelDef(*this)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F8910, oracle length 34 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push 0x154 ; mov esi,ecx ; call operator new ; add esp,4 ; test eax,eax ; je N ; push esi ; mov ecx,eax ; call CAbilityTrackLevelDef::CAbilityTrackLevelDef(const CAbilityTrackLevelDef&) ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Virtual clone: allocates sizeof(CAbilityTrackLevelDef) = 0x154, null-checks and copy-constructs from
// `*this` through the out-of-line copy constructor @ 0x006F8940; the constructor's `this` return
// is the result. One of 104 bodies of this shape (the copy constructor's label names the class).
struct CCreatureActionBase {
    virtual void v00();
};

struct CAbilityTrackLevelDef : CCreatureActionBase {
    unsigned char _body[0x154 - 4];    // sizeof(CAbilityTrackLevelDef) = 0x154
    CAbilityTrackLevelDef(const CAbilityTrackLevelDef& other);             // 0x006F8940
    CCreatureActionBase* Clone();
};

CCreatureActionBase* CAbilityTrackLevelDef::Clone()
{
    return new CAbilityTrackLevelDef(*this);
}