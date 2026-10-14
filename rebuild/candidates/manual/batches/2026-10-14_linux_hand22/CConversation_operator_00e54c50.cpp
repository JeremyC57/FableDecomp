// CConversation::operator=(const CConversation& other) -- memberwise assignment: the def base
// assigned out of line, Lines, then the four CCharString vectors assigned out of line
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E54C50, length 73 bytes (+ int3 padding).
// Callees: CDefClassBase::operator= @ 0x00431F10 (the def base, as in the landed CCardDef::Copy),
// vector<CCharString>::operator= @ 0x00432EE9 (x4; the manifest's "CCombatWheel::operator="
// label is propagated). No vtable store (assignment). Returns `this`.
// PDB layout (retail vectors are the 12-byte {first, last, end}): long Lines +0x28,
// vector<CCharString> Speaker +0x2c, Dialogue +0x38, Animation +0x44, AnimLoop +0x50.
// engine/CDefClassBase.h (PDB, 0x25 bytes padded to 0x28) is the base layout, modelled locally
// with its operator=. The manifest label "Copy" is rejected: the body is the assignment
// operator (the copy constructor twin is @ 0x00E54CA0).
struct CDefClassBase_Retail {                  // engine/CDefClassBase.h: vptr + 0x24 bytes
    virtual void v00();
    unsigned char _body[0x24];
    CDefClassBase_Retail& operator=(const CDefClassBase_Retail& other);   // 0x00431F10
};

struct CRetailVector_CCharString {             // VC6-era Dinkumware vector<CCharString>: first / last / end
    void* first;
    void* last;
    void* end;
    CRetailVector_CCharString& operator=(const CRetailVector_CCharString& other);   // 0x00432EE9
};

struct CConversation : CDefClassBase_Retail {  // vtable 0x012E5FF0
    long                      Lines;           // +0x28
    CRetailVector_CCharString Speaker;         // +0x2c
    CRetailVector_CCharString Dialogue;        // +0x38
    CRetailVector_CCharString Animation;       // +0x44
    CRetailVector_CCharString AnimLoop;        // +0x50
    virtual void v00();
    CConversation& operator=(const CConversation& other);
};

CConversation& CConversation::operator=(const CConversation& other)
{
    CDefClassBase_Retail::operator=(other);
    Lines = other.Lines;
    Speaker = other.Speaker;
    Dialogue = other.Dialogue;
    Animation = other.Animation;
    AnimLoop = other.AnimLoop;
    return *this;
}
