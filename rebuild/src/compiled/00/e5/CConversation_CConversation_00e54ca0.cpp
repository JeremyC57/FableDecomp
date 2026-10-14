// CConversation::CConversation(const CConversation& other) -- copy constructor: the def base
// copied out of line, own vtable, Lines, then the four CCharString vectors copied out of line
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E54CA0, length 79 bytes (+ int3 padding).
// Callees: CDefClassBase::CDefClassBase(const CDefClassBase&) @ 0x0046644C (as in the landed
// CPrizeScoreDef copy constructor), vector<CCharString>::vector(const vector&) @ 0x007ACEA0 (x4;
// the manifest's "Vector_Copy" label). Vtable 0x012E5FF0 (script module, no RTTI; the class is
// the manifest's and the PDB's CConversation). PDB layout (108 bytes with 16-byte debug vectors;
// retail vectors are the 12-byte {first, last, end}): long Lines +0x28, vector<CCharString>
// Speaker +0x2c, Dialogue +0x38, Animation +0x44, AnimLoop +0x50 (0x5c bytes). engine/CDefClassBase.h
// (PDB, 0x25 bytes padded to 0x28) is the base layout; it is modelled locally as a polymorphic
// base so the vtable store appears. The manifest label "Copy" is rejected: the body returns `this`
// and runs constructors (the operator= twin is @ 0x00E54C50).
struct CDefClassBase_Retail {                  // engine/CDefClassBase.h: vptr + 0x24 bytes (PDefManager, GlobalIndex, flag bits)
    virtual void v00();
    unsigned char _body[0x24];
    CDefClassBase_Retail(const CDefClassBase_Retail& other);   // 0x0046644C
};

struct CRetailVector_CCharString {             // VC6-era Dinkumware vector<CCharString>: first / last / end
    void* first;
    void* last;
    void* end;
    CRetailVector_CCharString(const CRetailVector_CCharString& other);   // 0x007ACEA0
};

struct CConversation : CDefClassBase_Retail {  // vtable 0x012E5FF0
    long                      Lines;           // +0x28
    CRetailVector_CCharString Speaker;         // +0x2c
    CRetailVector_CCharString Dialogue;        // +0x38
    CRetailVector_CCharString Animation;       // +0x44
    CRetailVector_CCharString AnimLoop;        // +0x50
    virtual void v00();
    CConversation(const CConversation& other);
};

CConversation::CConversation(const CConversation& other)
    : CDefClassBase_Retail(other)
    , Lines(other.Lines)
    , Speaker(other.Speaker)
    , Dialogue(other.Dialogue)
    , Animation(other.Animation)
    , AnimLoop(other.AnimLoop)
{
}